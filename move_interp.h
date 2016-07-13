#pragma once
#include "types.h"

// moved to ../battle.h:345:12: extern u32 b_movescr_cursor; // 02023D74
// extern u8 *b_move_cursor;
#include "battle.h"

extern void (*move_cmds[])(void); // -> move_cmds.h

void bc_move_exec_toplevel();
void b_stack_push_move_addr(u8 *value);
void b_stack_push_move_cursor();
void b_stack_pop_move_cursor();
void sub_0801BC24(u8 *target);
