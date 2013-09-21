struct bg_config {
	uint active     : 1;
	uint unknown0   : 1;
	uint screensize : 2;
	uint unknown2   : 2;
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
		case 6:  return bg->unknown2;
		case 7:  return bg->mosaic;
		case 8:  return bg->wraparound;
		default: return 0xFF;
	}
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