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
struct easy_chat_quote {
	int word0;
	int word1;
	int word2;
	int word3;
};

// 02021D18
extern u8 displayed_string[];
// 0203B0C4
extern void *hm_phase_2;

void sub_805F5E8(u16 tag, u8 pal_slot);
void dma3_transfer_queue_clear();
u16  dma3_queue_add_transfer(u32 *loc_a, u32 *loc_b, u16 size, u8 zz);
u32  sub_8000FE8(u16 r);

#define CPU_SET_HALF 0x00000000
#define CPU_SET_WORD 0x04000000
#define CPU_SET_FILL 0x01000000

// 081E3B64
void CpuSet(void*, void*, u32);
