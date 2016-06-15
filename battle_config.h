#pragma once
#include "types.h"

void task_add_01_battle_start(u8 priv1, u16 music_id);
void task_add_01_battle_start_with_music_and_stats();
char *battle_configure_by_script(char *cursor);
void battle_80801F0();
// u8 *battle_load_arguments(struct battle_config_entry *bce, u8 *cursor);
bool trainer_flag_check(u16 trainer_id);
void trainer_flag_set(u16 trainer_id);
void trainer_flag_clear(u16 trainer_id);
void trainer_battle_start();
void c2_exit_battle_switch();
u8 *battle_get_continuation_script();
void battle_intro_launch(u8 environment_id);
