// 0814F884 fbox6_func

// ...

// 0814FD04

// 0814FD38 (wrong address?) (unused)
void bgid_upload_textbox_84566A8(u8 bgid, u16 tileindex, u8 palindex) {
	gpu_copy_to_tileset(bgid, &gfx_tiles_textbox_84566A8, 0x280u, tileindex);
	gpu_pal_apply(stdpal_get(2), palindex, 32);
}

// 0814FD38 (unused)
void bgid_upload_curved_textbox_1(u8 bgid, u16 tileindex, u8 palindex) {
	gpu_copy_to_tileset(bgid, &gfx_tiles_curved_textbox_1, 0x280u, tileindex);
	gpu_pal_apply(stdpal_get(0), palindex, 32);
}

// 0814FD6C (unused)
void bgid_upload_textbox_8470B0C(u8 bgid, u16 tileindex, u8 palindex) {
	gpu_copy_to_tileset(bgid, &gfx_tiles_textbox_8470B0C, 0x260u, tileindex);
	gpu_pal_apply(stdpal_get(1), palindex, 32);
}

// 0814FDA0
void bgid_upload_textbox_1(u8 bgid, u16 tileindex, u8 palindex) {
	gpu_copy_to_tileset(bgid, &gfx_tiles_textbox_1, 0x120u, tileindex);
	gpu_pal_apply(stdpal_get(3), palindex, 32);
}

// 0814FDD4
void bgid_upload_curved_textbox_2_no_pal(u8 bgid, u16 tileindex, u8 palindex) {
	gpu_copy_to_tileset(bgid, &gfx_tiles_curved_textbox_2, 0x280u, tileindex);
}

// 0814FDF4

// 0814FE40

// 0814FE6C

// 0814FEAC
void rboxid_upload_curved_textbox_1(u8 rboxid) {
	u8 bgid = rboxid_get_field(rboxid, 0);
	gpu_copy_to_tileset(bgid, &gfx_tiles_curved_textbox_1, 0x280u, tileindex);
	gpu_pal_apply(stdpal_get(0), palindex, 32);
}

// 0814FEEC
void rboxid_upload_textbox_8470B0C(u8 rboxid, u16 tileindex, u8 palindex) {
	u8 bgid = rboxid_get_field(rboxid, 0);
	gpu_copy_to_tileset(bgid, &gfx_tiles_textbox_8470B0C, 0x260u, tileindex);
	gpu_pal_apply(stdpal_get(1), palindex, 32);
}

// 0814FF2C
void rboxid_upload_textbox_1(u8 rboxid, u16 tileindex, u8 palindex) {
	u8 bgid = rboxid_get_field(rboxid, 0);
	gpu_copy_to_tileset(bgid, &gfx_tiles_textbox_1, 0x120u, tileindex);
	gpu_pal_apply(stdpal_get(3), palindex, 32);
}

// 0814FF6C
void rboxid_upload_textbox_1_no_pal(u8 rboxid, u16 tileindex, u8 palindex) {
	u8 bgid = rboxid_get_field(rboxid, 0);
	gpu_copy_to_tileset(bgid, &gfx_tiles_textbox_1, 0x120u, tileindex);
}

// 0814FF98
void rboxid_upload_curved_textbox_2_no_pal(u8 rboxid, u16 tileindex, u8 palindex) {
	u8 bgid = rboxid_get_field(rboxid, 0);
	gpu_copy_to_tileset(bgid, &gfx_tiles_curved_textbox_2, 0x280u, tileindex);
}

// 0814FFC4

// 0815001C

// 08150048

// 081501D0

// 0815036C
void rbox_fill_rectangle(u8 rboxid) {
	u8 bgid = rboxid_get_field(rboxid, 0);
	u8 x = rboxid_get_field(rboxid, 1) - 1;
	u8 y = rboxid_get_field(rboxid, 2) - 1;
	u8 w = rboxid_get_field(rboxid, 3) + 2;
	u8 h = rboxid_get_field(rboxid, 4) + 2;
	bgid_fill_rectangle_maybe(bgid, 0, x, y, w, h);
}

// 08150408 stdpal_get

// end
