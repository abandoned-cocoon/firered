#pragma once
#include "types.h"

enum evolution_type {
	friendship = 0x1,
	friendship_day = 0x2,
	friendship_night = 0x3,
	level_up = 0x4,
	trade = 0x5,
	trade_item = 0x6,
	stone = 0x7,
	atk_gt_def = 0x8,
	atk_eq_def = 0x9,
	atk_lt_def = 0xA,
	pid_silcoon = 0xB,
	pid_cascoon = 0xC,
	spawn = 0xD,
	spawned = 0xE,
	beauty = 0xF,
};

struct evolution_entry {
	u16 type; // enum evolution_type
	u16 argument;
	u16 species; // enum poke_indices
	u16 unknown;
};

struct poke_evolutions {
	struct evolution_entry first;
	struct evolution_entry second;
	struct evolution_entry third;
	struct evolution_entry fourth;
	struct evolution_entry fifth;
};
