struct door_frame_t {
	u8 field_0;
	u8 field_1;
	u16 byteoffset_in_gfx;
};

struct door_t {
	u16 blockid;
	u8 sound;
	u8 twotiled;
	void *gfxtiles;
	u8 *palindices;
};

// 0835B5D8
struct door_t door_blocks[] = {
	{ 0x03D, 0, 0, &unk_8353088, &unk_835B4D8 }, // house door
	{ 0x062, 1, 0, &unk_8353228, &unk_835B4E0 }, // pokecenter door
	{ 0x15B, 1, 0, &unk_83533C8, &unk_835B4E8 }  // gym door
	/* ... */
};

// 0805A91C
void overworld_draw_block_type1_on_map_coord(u32 x, u32 y, u16 *blockdef) {
	i16 pos = map_pos_to_screenspace(&byte_3000E90, x, y);
	if (pos >= 0)
		overworld_draw_block(/*ttype*/1, blockdef, pos);
}

// 0805AF14
void door_anim_frame_apply(struct door_t *door, struct door_frame_t *frame, u32 x, u32 y) {
	u16 offset = frame->byteoffset_in_gfx;
	if (offset != 0xFFFF) {
		door_upload_tiles(door->gfxtiles + offset);
		door_patch_tilemap(door, x, y, door->data2);
	} else {
		door_restore_tilemap(door, x, y);
	}

}

// 0805AF54
void door_restore_tilemap(struct door_t *door, u32 x, u32 y) {
	switch (door->twotiled) {
		default:    cur_mapdata_draw_block_at(x, y-1); // no break; here
		case false: cur_mapdata_draw_block_at(x, y);
	}
}

// 0805AF80
void door_upload_tiles(void *data) {
	memcpy(0x06007F00, data, 0x100);
}

// 0805AF94
void door_patch_tilemap(struct door_t *door, u32 x, u32 y, u8 *palindices) {
	u16 blockdef[8];
	u16 tileid = 0x3F8;
	switch (door->twotiled) {
		case default:
			door_build_blockdef(blockdef, tileid, palindices);
			overworld_draw_block_type1_on_map_coord(x, y-1, blockdef);
			tileid += 4, data += 4;
		case false:
			door_build_blockdef(blockdef, tileid, palindices);
			overworld_draw_blolindicesck_type1_on_map_coord(x, y, blockdef);
	}
}

// 0805AFE8
void door_build_blockdef(u16 *blockdef, u16 tileid, u8 *palindices) {
	// BUG WARNING
	// this function assumes that 'palindices' points to a eight byte region
	// in 'door_patch_tilemap' it's called with (palindices+4), so this
	// function reads four bytes beyond the end of the array.
	for (int i=0; i<4; i++)
		blockdef[i] = (palindices[i]<<12) | (tileid + i);
	for (int i=4; i<8; i++)
		blockdef[i] = (palindices[i]<<12);
}

// 0805B1A4
struct door_frame_t *door_frame_last(struct door_frame_t *frames) {
	while ((frames++)->field_0);
	return frame-1;
}