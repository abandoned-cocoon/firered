#pragma once
#include "uncategorized.h"

struct trainer_pokemon {
	u16 species;
	u16 held_item;
	u16 moves[4];
	u8 field_C[20];
	u8 nickname[11];
	u8 field_2B;
};

struct trainer_tower_trainer {
	u8 name[11];
	u8 field_B;
	u8 field_C;
	u8 field_D;
	struct easy_chat_quote challenge_quote;
	struct easy_chat_quote win_quote;
	struct easy_chat_quote lose_quote;
	u16 field_3E;
	struct trainer_pokemon pokemon[6];
};

struct trainer_tower_card {
	u8 cardnumber;
	u8 field_1;
	u8 mode;
	u8 reward_id;
	struct trainer_tower_trainer trainers[3];
	int field_3DC;
};
