#include "battle.h"
#include "battle_ai_trainer.h"

#ifndef NO_RAM
struct b_resources_t *b_resources;
void (*tai_move_consideration_cmds[0x5E])();

//02039A00
u8 *tai_cursor;
#endif

// 080C6F44
u8 tai_pick_move() {
	struct trainer_ai *t = b_resources->tai_state;
	b_movehistory_add_defenders_move();

	while (t->script_usage_mask) {
		t->moveset_index = 0;

		if ( t->script_usage_mask & 1 ) {
			t->phase = 0;
			tai_run_script();
		}

		t->script_usage_mask >>= 1;
		t->script_id++;
	}

	if (t->flags & TAI_FLAG_USE_MOVE_4)
		return 4;

	if (t->flags & TAI_FLAG_USE_MOVE_5)
		return 5;

	int count = 1;
	u8 highest_score = t->viabilities[0];
	u8 moveset_indices[4] = {0};

	for (int i=1; i<4; i++) {
		i8 score = t->viabilities[i];

		if (highest_score < score)
			count = 0;

		if (highest_score <= score) {
			highest_score = score;
			moveset_indices[count++] = i;
		}
	}

	return moveset_indices[rand() % count];
}

// 080C7038
void tai_run_script(void) {

	struct trainer_ai *t = b_resources->tai_state;

	while (t->phase != TAI_PHASE_QUIT) {

		if (t->phase == TAI_PHASE_NEXT_MOVE) {
			tai_cursor = tai_scripts[t->script_id];

			struct battle_data *attacker = &b_data[b_attacker];
			u8 moveset_index = t->moveset_index;
			t->move_id
				= attacker->current_pp[moveset_index]
				? attacker->move_ids[moveset_index]
				: 0;

			t->phase = TAI_PHASE_EVALUATE_MOVE;

		} else if (t->phase == TAI_PHASE_EVALUATE_MOVE) {

			if ( t->move_id ) {
				tai_move_consideration_cmds[*tai_cursor]();

			} else {
				t->viabilities[t->moveset_index] = 0;
				t->flags |= TAI_FLAG_EVAL_COMPLETE;
			}

			if (t->flags & TAI_FLAG_EVAL_COMPLETE) {
				t->moveset_index++;

				if (t->moveset_index >= 4 || t->flags & TAI_FLAG_QUIT)
					t->phase = TAI_PHASE_QUIT;
				else
					t->phase = TAI_PHASE_NEXT_MOVE;

				t->flags &= ~TAI_FLAG_EVAL_COMPLETE;
			}
		}
	}
}

// 080C7164
void b_movehistory_add_defenders_move() {
	signed int v0;
	char *v1;

	v0 = 0;
	while ( 1 ) {
		v1 = (char *)b_resources->history + 16 * ((unsigned int)(u8)b_defender >> 1) + 2 * v0;
		if ( !*(u16 *)v1 )
			break;
		++v0;
		if ( v0 > 7 )
			return;
	}
	*(u16 *)v1 = move_to_execute_B_pbs[b_defender];
}

// 080C71A8 (unused)
int b_movehistory_clear(int a1) {
	int v1;
	signed int v2;
	int v4;

	v1 = 8 * ((unsigned int)(a1 << 24) >> 25);
	v2 = 7;
	do {
		b_resources->history->movehistory_1[v1] = 0;
		++v1;
		--v2;
	}
	while ( v2 >= 0 );
	return v4;
}

// 080C71D0
void b_history__record_ability_usage_of_player(u8 a1, char a2) {
	if ( !(battle_side_get_owner(a1) << 24) )
		b_resources->history->abilites_used[battle_get_per_side_status(a1) & 1] = a2;
}

// 080C7208
void b_history__record_item_x12_of_player(u8 a1, char a2) {
	u8 v2;
	char v3;

	v2 = a1;
	v3 = a2;
	if ( !(battle_side_get_owner(a1) << 24) )
		b_resources->history->item_x12[battle_get_per_side_status(v2) & 1] = v3;
}

// nowhere
u32 unaligned32(u8 *p) {
	// unaligned read
	return (*p++) + ((*p++)<<8) + ((*p++)<<16) + ((*p++)<<24);
}

// nowhere
#define TAI_COND_JUMP(cond, skip) \
	if (cond) { tai_cursor = (u8*)unaligned32(tai_cursor+skip-4); } \
	else { tai_cursor += (skip); }

// 080C7240
void tai_cmd_random_goto__high_param_likely() {
	u8 r = rand();
	u8 p = tai_cursor[1];
	TAI_COND_JUMP (r < p, 6)
}

// 080C7280
void tai_cmd_random_goto__low_param_likely() {
	u8 r = rand();
	u8 p = tai_cursor[1];
	TAI_COND_JUMP (r >= p, 6)
}

// 080C72C0
void tai_cmd_random_goto__1_in_256_chance() {
	u8 r = rand();
	u8 p = tai_cursor[1];
	TAI_COND_JUMP (r == p, 6)
}

// 080C7300
void tai_cmd_random_goto__255_in_256_chance() {
	u8 r = rand();
	u8 p = tai_cursor[1];
	TAI_COND_JUMP (r != p, 6)
}

// 080C7340
void tai_cmd_viability_score() {
	struct trainer_ai *t = b_resources->tai_state;
	i8 *viability = &t->viabilities[t->moveset_index];
	i8 delta = tai_cursor[1];

	*viability = max(0, *viability + delta);

	tai_cursor += 2;
}

// 080C7384
void tai_cmd_jump_if_health_percentage_lt() {
	u8 who = tai_cursor[1] ? b_attacker : b_defender;
	u8 cutoff = tai_cursor[2];
	u8 perc = 100 * b_data[who].current_hp / b_data[who].max_hp;
	TAI_COND_JUMP (perc < cutoff, 7)
}

// 080C73EC
void tai_cmd_jump_if_health_percentage_ge() {
	u8 who = tai_cursor[1] ? b_attacker : b_defender;
	u8 cutoff = tai_cursor[2];
	u8 perc = 100 * b_data[who].current_hp / b_data[who].max_hp;
	TAI_COND_JUMP (perc >= cutoff, 7)
}

// 080C7454
void tai_cmd_jump_if_health_percentage_eq() {
	u8 who = tai_cursor[1] ? b_attacker : b_defender;
	u8 cutoff = tai_cursor[2];
	u8 perc = 100 * b_data[who].current_hp / b_data[who].max_hp;
	TAI_COND_JUMP (perc == cutoff, 7)
}

// 080C74BC
void tai_cmd_jump_if_health_percentage_ne() {
	u8 who = tai_cursor[1] ? b_attacker : b_defender;
	u8 cutoff = tai_cursor[2];
	u8 perc = 100 * b_data[who].current_hp / b_data[who].max_hp;
	TAI_COND_JUMP (perc != cutoff, 7)
}

// 080C7524
void tai_cmd_jump_if_any_status1_bit() {
	u8 who = tai_cursor[1] ? b_attacker : b_defender;
	TAI_COND_JUMP (b_data[who].status1 & unaligned32(tai_cursor+2), 6)
}

// 080C7598
void tai_cmd_jump_if_no_status1_bit() {
	u8 who = tai_cursor[1] ? b_attacker : b_defender;
	TAI_COND_JUMP (~b_data[who].status1 & unaligned32(tai_cursor+2), 6)
}

extern u8 tai0[];
extern u8 tai1[];
extern u8 tai2[];
extern u8 tai3[];
extern u8 tai4[];
extern u8 tai5[];
extern u8 tai6[];
extern u8 tai7[];
extern u8 tai8[];
extern u8 tai9[];
extern u8 taiA[];

extern u8 tai1D[];
extern u8 tai1E[];
extern u8 tai1F[];

// 081D9BF4
u8 *tai_scripts[32] = {
	tai0, tai1, tai2, tai3, tai4, tai5, tai6, tai7,
	tai8, tai9, taiA, taiA, taiA, taiA, taiA, taiA,
	taiA, taiA, taiA, taiA, taiA, taiA, taiA, taiA,
	taiA, taiA, taiA, taiA, taiA, tai1D, tai1E, tai1F
};
