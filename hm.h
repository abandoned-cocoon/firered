#pragma once
#include "types.h"

bool hm_prepare_cut(void);
bool npc_before_player_of_type(u8 npc_type);
bool hm_prepare_rocksmash(void);
void hm_rocksmash_run_scr();
bool hm_prepare_dig(void);
bool hm2_dig(void);
bool hm_prepare_flash(void);
bool ruin_valley_trigger(void);
bool hm_prepare_strength(void);
bool hm_prepare_sweet_scent(void);
bool hm_prepare_dive_probably(void);
bool hm_prepare_teleport(void);
bool hm_add_c3_launch_phase_2();
void task_launch_hm_phase_2(task_id c);
bool hm_prepare_surf(void);
bool hm_prepare_fly(void);
bool hm_prepare_waterfall(void);

// 02039874
extern u8 cut_ruin_valley;
