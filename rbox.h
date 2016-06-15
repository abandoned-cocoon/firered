#pragma once
#include "types.h"

struct rboxbase {
	u8 bg_id;
	u8 x;
	u8 y;
	u8 w;
	u8 h;
	u8 palnum;
	u16 vram_tileset_offset;
};

struct rbox {
	u8 bg_id;
	u8 x;
	u8 y;
	u8 w;
	u8 h;
	u8 palnum;
	u16 vram_tileset_offset;
	// and
	void *pixels;
};

extern struct rbox rboxes[32]; // 020204B4
