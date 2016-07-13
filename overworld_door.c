struct door_frame_t {
	u8 field_0;
	u16 byteoffset_in_gfx;
};

struct door_t {
	u16 blockid;
	u8 sound;
	u8 twotiled;
	void *gfxtiles;
	u8 *palindices;
};

extern u8 unk_8353088[];
extern u8 unk_8353228[];
extern u8 unk_83533C8[];
extern u8 unk_835B4D8[];
extern u8 unk_835B4E0[];
extern u8 unk_835B4E8[];

// 0835B5D8
struct door_t door_blocks[] = {
	{ 0x03D, 0, 0, unk_8353088, unk_835B4D8 }, // house door
	{ 0x062, 1, 0, unk_8353228, unk_835B4E0 }, // pokecenter door
	{ 0x15B, 1, 0, unk_83533C8, unk_835B4E8 }  // gym door
	/* ... */
};

struct door_frame_t door_frames_open_singletile[] = {
	{4, 0xffff}, {4, 0x0}, {4, 0x80}, {4, 0x100}, {0}
};

struct door_frame_t door_frames_open_twotiled[] = {
	{4, 0xffff}, {4, 0x0}, {4, 0x100}, {4, 0x200}, {0}
};

struct door_frame_t door_frames_close_singletile[] = {
	{4, 0x100}, {4, 0x80}, {4, 0x0}, {4, 0xffff}, {0}
};

struct door_frame_t door_frames_close_twotiled[] = {
	{4, 0x200}, {4, 0x100}, {4, 0x0}, {4, 0xffff}, {0}
};

// TODO: Header (for whichever of these are public)
void door_anim_frame_apply(struct door_t *door, struct door_frame_t *frame, u32 x, u32 y);
void door_restore_tilemap(struct door_t *door, u32 x, u32 y);
void door_upload_tiles(void *data);
void door_patch_tilemap(struct door_t *door, u32 x, u32 y, u8 *palindices);
void door_build_blockdef(u16 *blockdef, u16 tileid, u8 *palindices);
struct door_frame_t *door_frame_last(struct door_frame_t *frames);
task_id task_overworld_door_add_for_opening_door_at(struct door_t *doors, s16 x, s16 y);
task_id task_overworld_door_add_for_closing_door_at(struct door_t *doors, s16 x, s16 y);

// 0805AF14
void door_anim_frame_apply(struct door_t *door, struct door_frame_t *frame, u32 x, u32 y) {
	u16 offset = frame->byteoffset_in_gfx;
	if (offset != 0xFFFF) {
		door_upload_tiles(door->gfxtiles + offset);
		door_patch_tilemap(door, x, y, door->palindices);
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
	memcpy((void*)0x06007F00, data, 0x100); // using CpuFastSet
}

// 0805AF94
void door_patch_tilemap(struct door_t *door, u32 x, u32 y, u8 *palindices) {
	u16 blockdef[8];
	u16 tileid = 0x3F8;
	switch (door->twotiled) {
		case true: default:
			door_build_blockdef(blockdef, tileid, palindices);
			overworld_draw_block_type1_on_map_coord(x, y-1, blockdef);
			tileid += 4;
		case false:
			door_build_blockdef(blockdef, tileid, palindices);
			overworld_draw_block_type1_on_map_coord(x, y, blockdef);
	}
}

// 0805AFE8
void door_build_blockdef(u16 *blockdef, u16 tileid, u8 *palindices) {
	// BUG WARNING
	// This function assumes that 'palindices' points to an eight byte region.
	// In 'door_patch_tilemap' it's called with (palindices+4), so this
	// function reads four bytes beyond the end of the array.
	for (int i=0; i<4; i++)
		blockdef[i] = (palindices[i]<<12) | (tileid + i);
	for (int i=4; i<8; i++)
		blockdef[i] = (palindices[i]<<12);
}

// 0805B028 task50_overworld_door
// 0805B060 sub_805B060

// 0805B0C4
struct door_t *door_find(struct door_t *doors, u16 blockid);

// 0805B1A4
struct door_frame_t *door_frame_last(struct door_frame_t *frames) {
	while ((frames++)->field_0);
	return --frames;
}

// 0805B0E8 task_overworld_door_add_if_inactive
// 0805B14C door_restore_tilemap_
// 0805B158 sub_805B158
// 0805B1A4 door_frame_last

// 0805B1B8
task_id task_overworld_door_add_for_opening_door_at(struct door_t *doors, s16 x, s16 y) {
	u16 blockid = cur_mapdata_get_blockid_at(x, y);
	struct door_t *door = door_find(doors, blockid);
	struct door_frame_t *frames;
	if (!door) return -1;
	frames = door->twotiled
	       ? door_frames_open_twotiled
	       : door_frames_open_singletile;
	return task_overworld_door_add_if_inactive(door, frames, x, y);
}

// 0805B210
task_id task_overworld_door_add_for_closing_door_at(struct door_t *doors, s16 x, s16 y) {
	u16 blockid = cur_mapdata_get_blockid_at(x, y);
	struct door_t *door = door_find(doors, blockid);
	struct door_frame_t *frames;
	if (!door) return -1;
	frames = door->twotiled
	       ? door_frames_close_twotiled
	       : door_frames_close_singletile;
	return task_overworld_door_add_if_inactive(door, frames, x, y);
}
