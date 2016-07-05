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

// TODO: List all
void rboxid_upload_a(u8 rboxid, u8 op);
void rboxid_vertical_scroll(u8 rboxid, u8 direction, u8 delta, u8 background_doublepixel);
