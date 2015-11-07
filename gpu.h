#pragma once
#include "types.h"

struct bg_config {
	uint active     : 1;
	uint _          : 1;
	uint screensize : 2;
	uint priority   : 2;
	uint mosaic     : 1;
	uint wraparound : 1;

	// 
	uint charbase   : 2;
	uint screenbase : 5;
	uint fullcolor  : 1;

	u16 padding;
};

struct bg_config2 {
	u16 target_tile;
	u16 field2;
	u16 *tilemap;
	u32 BG_X;
	u32 BG_Y;
};

// 03000000
extern u8 lcd_io_buffer[96];
// 03000060
extern u8 lcd_io_copy_queue[96];
// 030000C0
extern u8 lcd_io_copy_queue_edit_lock;
// 030008C1
extern u8 IE_dirty;
// 030008C2
extern u8 IE_target;
// 030008D0
extern struct bg_config gpu_bg_config[4];
// 030008E0
extern u16 bg_visibility_and_mode; // bitlayout: ____3210 _____mmm
// 030008E8
extern struct bg_config2 bg_config2[4];
// 03000928
extern u8 dmas_busy_with_gpu_copy_bitfield[16];
// 03000938
extern u8 gpu_tile_allocation_map_bg[256];

void lcd_io_copy_queue_clear(void);
void lcd_io_buffer_to_hardware(u8 a1);
void lcd_io_copy_queue_process(void);
void lcd_io_set(u8 reg, u16 value);
u16 lcd_io_get(u8 reg);
void lcd_io_set_bits(u8 reg, u16 flags);
void lcd_io_clear_bits(u8 reg, u16 flags);

void gpu_init_bgs(void);
void gpu_set_tile_mode(u8 mode);
u8   gpu_get_tile_mode(void);
void gpu_reset_bg_configs(void);
void gpu_bg_config_set_fields(u8 bg_id, u8 i1, u8 i2, u8 i3, u8 i4, u8 i5, u8 i6, u8 i7);
u8   gpu_bg_config_get_field(u8 bg_id, u8 field);
s8   gpu_copy_to_vram_by_bgid(u8 bg_id, u16 *src, size_t size, u16 offset, u8 mode);
void gpu_bg_show(u8 bg_id);
void gpu_bg_hide(u8 bg_id);
void gpu_sync_bg_visibility_and_mode(void);
void gpu_sync_text_mode_and_hide_bgs(void);
s16  gpu_copy_to_tileset(u8 bg_id, u16 *src, size_t size, size_t offset);
void gpu_sync_bg_show(u8 bg_id);
void gpu_sync_bg_hide(u8 bg_id);
void gpu_bg_config_set_field(u8 bg_id, u8 field, u8 value);
void gpu_bg_config_set_field(u8 bg_id, u8 field, u8 value);
void bgid_mod_x_offset(u8 bgid, s32 value, u8 mode);
void tilemap_rect_copy_translate(
	u8 bgid,
	void *src,
	u8 dst_x,
	u8 dst_y,
	u8 width,
	u8 height,
	u8 palindex_global);
void tilemap_rect_copy_crop_translate_rebase(
	u8 bgid,
	void *src,
	u8 src_x, u8 src_y,
	u8 src_width, u8 src_height,
	u8 dst_x, u8 dst_y,
	u8 win_width, u8 win_height,
	u8 palindex_global,
	u16 tileindex_offset_global,
	u16 palindex_offset_global);
u8   bgid_dimensions_textmode(u8 bgid, u32 property);
u8   bgid_dimensions_rotscalemode(u8 bgid, u32 property);
u32  tilecoord_remap_for_screensize(u32 x, u32 y, u32 screensize, u32 htiles, u32 vtiles);
