struct blob_t {
	u8 *data;
	u16 size;
	u16 padding;
};

struct bg_config {
	uint active     : 1;
	uint _          : 1;
	uint screensize : 2;
	uint priority   : 2;
	uint mosaic     : 1;
	uint wraparound : 1;

	uint charbase   : 2;
	uint screenbase : 5;
	uint fullcolor  : 1;

	u16 padding;
};

// 030008D0
struct bg_config gpu_bg_config[4];
// 030008E0
u16 bg_visibility_and_mode; // bitlayout: ____3210 _____mmm

// 08001028
void gpu_init_bgs() {
	gpu_reset_bg_configs();
	bg_visibility_and_mode = 0;
	gpu_text_mode_and_hide_bgs();
}

// 08001040
void gpu_set_tile_mode(u8 mode) {
	bg_visibility_and_mode &= ~7;
	bg_visibility_and_mode |= mode;
}

// 0800105C
u8 gpu_get_tile_mode() {
	return bg_visibility_and_mode & 7;
}

// 0800106C
void gpu_reset_bg_configs() {
	((u32*)&gpu_bg_config)[0] = 0;
	((u32*)&gpu_bg_config)[1] = 0;
	((u32*)&gpu_bg_config)[2] = 0;
	((u32*)&gpu_bg_config)[3] = 0;
}

// 080010B8
void gpu_bg_config_set_fields(u8 bg_id, u8 i1, u8 i2, u8 i3, u8 i4, u8 i5, u8 i6, u8 i7) {
	struct bg_config *bg = gpu_bg_config + bg_id;
	if (bg_id >= 4) return;
	if (i1 != 0xFF) bg->charbase = i1;
	if (i2 != 0xFF) bg->screenbase = i2;
	if (i3 != 0xFF) bg->screensize = i3;
	if (i4 != 0xFF) bg->fullcolor = i4;
	if (i5 != 0xFF) bg->priority = i5;
	if (i6 != 0xFF) bg->mosaic = i6;
	if (i7 != 0xFF) bg->wraparound = i7;
	bg->active = 1;
	bg->padding = 0;
}

// 080011E4
u8 gpu_bg_config_get_field(u8 bg_id, u8 field) {
	struct bg_config *bg = gpu_bg_config + bg_id;
	if (bg_id >= 4 || bg->active == 0) return 0xFF;
	switch (field) {
		case 1:  return bg->active;
		case 2:  return bg->charbase;
		case 3:  return bg->screenbase;
		case 4:  return bg->screensize;
		case 5:  return bg->fullcolor;
		case 6:  return bg->priority;
		case 7:  return bg->mosaic;
		case 8:  return bg->wraparound;
		default: return 0xFF;
	}
}

#define GPU_COPY_TILESET 1
#define GPU_COPY_TILEMAP 2

// 08001298
s8 gpu_copy_to_vram_by_bgid(u8 bg_id, u16 *src, size_t size, u16 offset, u8 mode) {
	struct bg_config *conf = &gpu_bg_config[bg_id];
	if (bg_id >= 4 || !conf->active) return;
	if (mode == GPU_COPY_TILESET)
		offset += conf->charbase * 0x4000;
	else if(mode == GPU_COPY_TILEMAP)
		offset += conf->screenbase * 0x800;
	else
		return -1;
	u16 *dst = 0x06000000 + offset;
	return dma3_add_to_copy_queue(src, dst, size, /*16 bit DMA*/0);
}

// 08001320
void gpu_bg_show(u8 bg_id) {
	struct bg_config *conf = &gpu_bg_config[bg_id];
	if (bg_id >= 4 || !conf->active) return;
	u16 bgcnt = 0;
	bgcnt |= conf->unknown2   <<  4;
	bgcnt |= conf->charbase   <<  2;
	bgcnt |= conf->mosaic     <<  6;
	bgcnt |= conf->fullcolor  <<  7;
	bgcnt |= conf->screenbase <<  8;
	bgcnt |= conf->wraparound << 13;
	bgcnt |= conf->screensize << 14;
	lcd_io_set(8+bg_id*2, bgcnt);
	bg_visibility_and_mode |=  (1<<(8+bg_id));
}

// 0800139C
void gpu_bg_hide(u8 bg_id) {
	if (bg_id >= 4) return;
	bg_visibility_and_mode &= ~(1<<(8+bg_id));
}

// 080013D0
void gpu_sync_bg_visibility_and_mode() {
	u16 dispcnt = lcd_io_get(0);
	lcd_io_set((dispcnt & 0xF0F8) | bg_visibility_and_mode);
}

// 080013F4
void gpu_text_mode_and_hide_bgs() {
	u16 dispcnt = lcd_io_get(0);
	lcd_io_set(dispcnt & 0xF0F8);
}

// 080017D0
s16 gpu_copy_to_tileset(u8 bg_id, u16 *src, size_t size, size_t offset) {
	// TODO
	bool isFullcolor = gpu_bg_config_get_field(5);
	u16 q_offset = offset+???;
	s8 dma_id = gpu_copy_to_vram_by_bgid(bg_id, src, size, q_offset, GPU_COPY_TILESET);
	if (dma_id == -1)
		return -1;

	dmas_busy_with_gpu_copy_bitfield[dma_id>>5] |= 1 << (dma_id & 0x1F);

	if (dword_03003D8C == 1)
		gpu_tile_allocation_bg_edit(bg_id, q_offset>>5, size>>5	, GPU_TAE_OCCUPY);

	return dma_id;
}

// 080019BC
void gpu_sync_bg_show(u8 bg_id) {
	gpu_bg_show(bg_id);
	gpu_sync_bg_visibility_and_mode();
}

// 080019D0
void gpu_sync_bg_hide(u8 bg_id) {
	gpu_bg_hide(bg_id);
	gpu_sync_bg_visibility_and_mode();
}

// 080019E4
void gpu_bg_config_set_field(u8 bg_id, u8 field, u8 value) {
	// real implementation
	switch (field) {
		case 1: gpu_bg_config_set_fields(bg_id, value, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF); break;
		case 2: gpu_bg_config_set_fields(bg_id, 0xFF, value, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF); break;
		case 3: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, value, 0xFF, 0xFF, 0xFF, 0xFF); break;
		case 4: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, value, 0xFF, 0xFF, 0xFF); break;
		case 5: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, value, 0xFF); break;
		case 6: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, value); break;
		case 7: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, 0xFF, value, 0xFF, 0xFF); break;
		default: return;
	}
}

// 080019E4
void gpu_bg_config_set_field(u8 bg_id, u8 field, u8 value) {
	// effective implementation
	struct bg_config *bg = gpu_bg_config + bg_id;
	if (bg_id >= 4) return 0xFF;
	switch (field) {
		case 1:  bg->charbase   = value; break;
		case 2:  bg->screenbase = value; break;
		case 3:  bg->screensize = value; break;
		case 4:  bg->fullcolor  = value; break;
		case 5:  bg->mosaic     = value; break;
		case 6:  bg->wraparound = value; break;
		case 5:  bg->priority   = value; break;
		default: return;
	}
	bg->active = 1;
	bg->padding = 0;
}

#define MOD_SET 0
#define MOD_ADD 1
#define MOD_SUB 2

// 08001B90
void bgid_mod_x_offset(u8 bgid, s32 value, u8 mode) {
	// TODO
}

// 08007434 gpu_find_free_tiles_probably

// 080F6790
void tilemaps_sync_none() {
	bgmaps_that_need_syncing[0] = false;
	bgmaps_that_need_syncing[1] = false;
	bgmaps_that_need_syncing[2] = false;
	bgmaps_that_need_syncing[3] = false;
}

// 080F67A4
void bgid_mark_for_sync(u8 bgid) {
	bgmaps_that_need_syncing[bgid] = true;
}

// 080F67B8
void gpu_tilemaps_sync() {
	u8 *t = bgmaps_that_need_syncing;
	if (t[0]) { bgid_send_tilemap(0); t[0] = false; }
	if (t[1]) { bgid_send_tilemap(1); t[1] = false; }
	if (t[2]) { bgid_send_tilemap(2); t[2] = false; }
	if (t[3]) { bgid_send_tilemap(3); t[3] = false; }
}
