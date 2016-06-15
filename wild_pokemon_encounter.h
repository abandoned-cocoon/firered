#pragma once
#include "overworld.h"

extern struct struct_20386D0 {
	u32 field_0;
	u16 role_4;
	u16 encounter_probability_bonus;
	u8 field_8;
	u8 encounter_probability_modification_for_ability;
} struc_20386D0;

bool is_it_battle_time_2(block bi, u16 role);
bool repel_per_step();
bool sub_808310C(u8 limit);
bool is_it_battle_time_1(block bi);
bool is_it_battle_time_3(block bi);
