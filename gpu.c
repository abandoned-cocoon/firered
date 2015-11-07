#include "gpu.h"
#include "lcd_registers.h"

#ifndef NO_RAM
// 03000000
u8 lcd_io_buffer[96];
// 03000060
u8 lcd_io_copy_queue[96];
// 030000C0
u8 lcd_io_copy_queue_edit_lock;
// 030008C1
u8 IE_dirty;
// 030008C2
u8 IE_target;
// 030008D0
struct bg_config gpu_bg_config[4];
// 030008E0
u16 bg_visibility_and_mode; // bitlayout: ____3210 _____mmm
// 030008E8
struct bg_config2 bg_config2[4];
// 03000928
u8 dmas_busy_with_gpu_copy_bitfield[16];
// 03000938
u8 gpu_tile_allocation_map_bg[256];
#endif

// 08000968
void lcd_io_copy_queue_clear() {

	for (u32 i=0; i<96; i++) {

		lcd_io_buffer[i] = 0;
		lcd_io_copy_queue[i] = 0xff;
	}

	lcd_io_copy_queue_edit_lock = 0;
	IE_dirty = 0;
	IE_target = 0;
}

// 080009C0
void lcd_io_buffer_to_hardware(u8 reg) {
	u16 *phys = (u16*)(0x04000000 + reg);
	u16 *buff = (u16*)(lcd_io_buffer + reg);
	if (reg == 4)
		*phys = *buff | *phys & 0xFFE7;
	else
		*phys = *buff;
}

// 08000A04
void lcd_io_copy_queue_process() {

	if (lcd_io_copy_queue_edit_lock)
		return;

	for (u32 i=0; i<96; i++) {

		u8 reg = lcd_io_copy_queue[i];
		if (reg == 0xFF)
			break;

		lcd_io_buffer_to_hardware(reg);
		lcd_io_copy_queue[i] = 0xFF;
	}
}

// 08000A38
void lcd_io_set(u8 reg, u16 value) {

	if (reg >= 96)
		return;

	*(u16*)(lcd_io_buffer + reg) = value;

	if (VCOUNT >= 161 && VCOUNT < 225 || DISPCNT & 0x80) {
		// access hardware directly during vblank
		lcd_io_buffer_to_hardware(reg);

	} else {
		// else enqueue register changes
		lcd_io_copy_queue_edit_lock = 1;

		u32 i;
		for (i=0; i<96; i++) {
			if (lcd_io_copy_queue[i] == 0xFF)
				break;

			if (lcd_io_copy_queue[i] == reg)
				break;
		}

		lcd_io_copy_queue[i] = reg;
		lcd_io_copy_queue_edit_lock = 0;
	}
}

// 08000AC4
u16 lcd_io_get(u8 reg) {
	if (reg == 4 || reg == 6) {
		return *(u16*)(0x04000000 + reg);
	} else {
		return *(u16*)(lcd_io_buffer + reg);
	}
}

// 08000AF4
void lcd_io_set_bits(u8 reg, u16 flags) {
	lcd_io_set(reg, *(u16*)(lcd_io_buffer + reg) | flags);
}

// 08000B14
void lcd_io_clear_bits(u8 reg, u16 flags) {
	lcd_io_set(reg, *(u16*)(lcd_io_buffer + reg) & ~flags);
}

// 08001028
void gpu_init_bgs() {
	gpu_reset_bg_configs();
	bg_visibility_and_mode = 0;
	gpu_sync_text_mode_and_hide_bgs();
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
	if (bg_id >= 4 || !conf->active) return -1;
	if (mode == GPU_COPY_TILESET)
		offset += conf->charbase * 0x4000;
	else if(mode == GPU_COPY_TILEMAP)
		offset += conf->screenbase * 0x800;
	else
		return -1;
	u16 *dst = (u16*)(0x06000000 + offset);
	return dma3_add_to_copy_queue(src, dst, size, /*16 bit DMA*/0);
}

// 08001320
void gpu_bg_show(u8 bg_id) {
	struct bg_config *conf = &gpu_bg_config[bg_id];
	if (bg_id >= 4 || !conf->active) return;
	u16 bgcnt = 0;
	bgcnt |= conf->priority   <<  0;
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
	lcd_io_set(0, (dispcnt & 0xF0F8) | bg_visibility_and_mode);
}

// 080013F4
void gpu_sync_text_mode_and_hide_bgs() {
	u16 dispcnt = lcd_io_get(0);
	lcd_io_set(0, dispcnt & 0xF0F8);
}

// // 080017D0
// s16 gpu_copy_to_tileset(u8 bg_id, u16 *src, size_t size, size_t offset) {
// 	// TODO
// 	bool isFullcolor = gpu_bg_config_get_field(5);
// 	u16 q_offset = offset+???;
// 	s8 dma_id = gpu_copy_to_vram_by_bgid(bg_id, src, size, q_offset, GPU_COPY_TILESET);
// 	if (dma_id == -1)
// 		return -1;
// 
// 	dmas_busy_with_gpu_copy_bitfield[dma_id>>5] |= 1 << (dma_id & 0x1F);
// 
// 	if (dword_03003D8C == 1)
// 		gpu_tile_allocation_bg_edit(bg_id, q_offset>>5, size>>5	, GPU_TAE_OCCUPY);
// 
// 	return dma_id;
// }

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

// // 080019E4
// void gpu_bg_config_set_field_real(u8 bg_id, u8 field, u8 value) {
// 	// real implementation
// 	switch (field) {
// 		case 1: gpu_bg_config_set_fields(bg_id, value, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF); break;
// 		case 2: gpu_bg_config_set_fields(bg_id, 0xFF, value, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF); break;
// 		case 3: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, value, 0xFF, 0xFF, 0xFF, 0xFF); break;
// 		case 4: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, value, 0xFF, 0xFF, 0xFF); break;
// 		case 5: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, value, 0xFF); break;
// 		case 6: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, value); break;
// 		case 7: gpu_bg_config_set_fields(bg_id, 0xFF, 0xFF, 0xFF, 0xFF, value, 0xFF, 0xFF); break;
// 		default: return;
// 	}
// }

// 080019E4
void gpu_bg_config_set_field(u8 bg_id, u8 field, u8 value) {
	// effective implementation
	struct bg_config *bg = gpu_bg_config + bg_id;
	if (bg_id >= 4) return;
	switch (field) {
		case 1:  bg->charbase   = value; break;
		case 2:  bg->screenbase = value; break;
		case 3:  bg->screensize = value; break;
		case 4:  bg->fullcolor  = value; break;
		case 5:  bg->mosaic     = value; break;
		case 6:  bg->wraparound = value; break;
		case 7:  bg->priority   = value; break;
		default: return;
	}
	bg->active = 1;
	bg->padding = 0;
}

#define MOD_SET 0
#define MOD_ADD 1
#define MOD_SUB 2

// // 08001B90
// void bgid_mod_x_offset(u8 bgid, s32 value, u8 mode) {
// 	// TODO
// }

// 08002228
void tilemap_rect_copy_translate(
	u8 bgid,
	void *src,
	u8 dst_x,
	u8 dst_y,
	u8 width,
	u8 height,
	u8 palindex_global
) {
	tilemap_rect_copy_crop_translate_rebase(
		bgid,
		src,
		0,
		0,
		width,
		height,
		dst_x,
		dst_y,
		width,
		height,
		palindex_global,
		0,
		0);
}

// 0800226C
void tilemap_rect_copy_crop_translate_rebase(
	u8 bgid,
	void *src,

	u8 src_x, u8 src_y,
	u8 src_width, u8 src_height,

	u8 dst_x, u8 dst_y,
	u8 win_width, u8 win_height,

	u8 palindex_global,
	u16 tileindex_offset_global,
	u16 palindex_offset_global
) {
	//if (is_invalid_bg_id(bgid))
	if (bgid >= 4)
		return;

	if (gpu_something_is_tilemap_outside_of_ram(bgid))
		return;

	u16 *srcmap16 = (u16*)src;
	u8  *srcmap8  = (u8*) src;

	u16 *dstmap16 = (u16*)bg_config2[bgid].tilemap;
	u8  *dstmap8  = (u8*) bg_config2[bgid].tilemap;

	s16 tilemode = bgid_get_tile_mode_attr(bgid);

	if ( tilemode == 0 ) { // text mode (2 bytes per tile)
		u32 htiles = 32 * bgid_dimensions_textmode(bgid, 1) & 0xFFFF;
		u32 vtiles = 32 * bgid_dimensions_textmode(bgid, 2) & 0xFFFF;
		u16 screensize = gpu_bg_config_get_field(bgid, 4);

		for (u32 win_y = 0; win_y < win_height; win_y++) {
			for (u32 win_x = 0; win_x < win_width; win_x++) {

				u16 src_index = (src_y+win_y) * src_width + (src_x+win_x);
				u16 dst_index = tilecoord_remap_for_screensize(
					dst_x+win_x,
					dst_y+win_y,
					screensize,
					htiles,
					vtiles);

				tile_rebase_indices(
					&srcmap16[src_index],
					&dstmap16[dst_index],
					palindex_global,
					tileindex_offset_global,
					palindex_offset_global);
			}
		}

	} else if ( tilemode == 1 ) { // rotscale mode (1 byte per tile)
		u16 dst_width = bgid_dimensions_rotscalemode(bgid, 1); // 1 = width

		for (u32 win_y = 0; win_y < win_height; win_y++) {
			for (u32 win_x = 0; win_x < win_width; win_x++) {

				u16 src_index = (src_y+win_y) * src_width + (src_x+win_x);
				u16 dst_index = (dst_y+win_y) * dst_width + (dst_x+win_x);

				dstmap8[dst_index] = srcmap8[src_index] + tileindex_offset_global;
			}
		}
	}
}

// 0800273C
u8 bgid_dimensions_textmode(u8 bgid, u32 property) {
	u8 screensize = gpu_bg_config_get_field(bgid, 4);
	switch (property) {
		case 0: return (u8[]){1, 2, 2, 4}[screensize];
		case 1: return (u8[]){1, 2, 1, 2}[screensize];
		case 2: return (u8[]){1, 1, 2, 2}[screensize];
		default: return 0;
	}
}

// 080027AC
u8 bgid_dimensions_rotscalemode(u8 bgid, u32 property) {
	u8 screensize = gpu_bg_config_get_field(bgid, 4);
	switch (property) {
		case 0: return (u8[]){ 1,  4, 16,  64}[screensize];
		case 1: return (u8[]){16, 32, 64, 128}[screensize];
		case 2: return (u8[]){16, 32, 64, 128}[screensize];
		default: return 0;
	}
}

// 08002804
u32 tilecoord_remap_for_screensize(u32 x, u32 y, u32 screensize, u32 htiles, u32 vtiles) {
	x &= htiles-1;
	y &= vtiles-1;

	switch (screensize) {
		case 0: // 256 x 256
		case 2: // 256 x 512
			break;
		case 3: // 512 x 512
			if (y >= 32)
				y += 32;
		case 1: // 512 x 256
			if (x >= 32) {
				x -= 32;
				y += 32;
			}
			break;
	}
	return 32 * y + x;
}
