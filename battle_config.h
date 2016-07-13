#pragma once
#include "types.h"

// using db names
extern u16 trainerbattle_battle_type;         // 020386AC
extern u16 trainerbattle_flag_id;             // 020386AE
extern u16 trainerbattle_arg2;                // 020386B0
extern u8* trainerbattle_message_intro;       // 020386B4
extern u8* trainerbattle_message_defeat;      // 020386B8
extern u8* trainerbattle_message_2;           // 020386BC
extern u8* trainerbattle_message_need_2_poke; // 020386C0
extern u8* trainerbattle_next_scr_cmd;        // 020386C4
extern u8* trainerbattle_message_4;           // 020386C8
extern u16 trainerbattle_unknown;             // 020386CC

// using c names
extern u16 battle_type;                // 020386AC
extern u16 battle_trainer_flag;        // 020386AE
extern u16 battle_arg2;                // 020386B0
extern u8* battle_message_intro;       // 020386B4
extern u8* battle_message_defeat;      // 020386B8
extern u8* battle_message_2;           // 020386BC
extern u8* battle_message_need_two;    // 020386C0
extern u8* battle_after_script;        // 020386C4
extern u8* battle_message_4;           // 020386C8
extern u16 battle_unknown;             // 020386CC

void task01_battle_start(task_id c);
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
