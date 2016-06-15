#pragma once
#include "types.h"

// 020370F0
extern void *start_menu_active_context;
// 020370F4
extern u8 start_menu_cursor;
// 020370F5
extern u8 start_menu_item_indices_cursor;
// 020370F6
extern u8 start_menu_item_indices[9];
// 020370FF
extern u8 start_menu_state_tracker;

// TODO

void sm_pokedex(void);
void sm_pokemon(void);
void sm_bag(void);
void sm_trainer_1(void);
void sm_save(void);
void sm_pokemon(void);
void sm_exit(void);
void sm_retire(void);
void sm_trainer_2(void);

void start_menu_compose();
void start_menu_append_item(u8 item_index);
void start_menu_compose_normal();
void start_menu_compose_for_safari();
void start_menu_compose_for_unknown();
void start_menu_compose_for_link_room();
bool start_menu_handle_input();
// void append_byte(u8 *buffer, u32 *cursor, u8 elem);
