#pragma once
#include "types.h"

struct mplay_table_entry {
	void *song;
	u8 field_4[20];
	u32 memacc_area;
	u8 field_1C[8];
	u16 field_24;
	u16 field_26;
	u16 field_28;
	u8 field_2A[10];
	u32 magic_number;
	u32 field_38;
	u32 field_3C;
};

struct mplay_table_entry mplay_te_03007300; // 03007300
struct mplay_table_entry mplay_te_03007340; // 03007340
struct mplay_table_entry mplay_te_03007380; // 03007380
