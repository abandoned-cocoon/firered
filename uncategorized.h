#pragma once
#include "types.h"

// TODO: Move away
struct coords8 {
	u8 x, y;
};

// TODO: Move away
struct coords16 {
	u16 x, y;
};

// TODO: Move away
struct warpdata {
	u8 bank;
	u8 map;
	u8 warpid;
	u8 unknown;
	u16 enter_x;
	u16 enter_y;
};

// TODO: Move away
struct rom_npc {
	u8  nr;
	u8  type_nr;
	u8  rival;
	u8  field_3;
	u16 x;
	u16 y;
	u8  height;
	u8  running_behaviour;
	u8  movement_area;
	u8  field_B;
	u8  trainer_or_mapnumber;
	u8  field_D;
	u16 sight_distance_or_mapbank;
	void *script;
	u16 id_in_script;
	u16 field_16;
};

void sub_805F5E8(u16 tag, u8 pal_slot);
void dma3_transfer_queue_clear();
u16  dma3_queue_add_transfer(u32 *loc_a, u32 *loc_b, u16 size, u8 zz);
u32  sub_8000FE8(u16 r);
