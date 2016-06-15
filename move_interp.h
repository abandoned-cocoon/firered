#pragma once
#include "types.h"

extern u8 *b_move_cursor;

void b_stack8_push(void *values);
void b_stack_push_move_cursor();
void b_stack_pop_move_cursor();
void sub_0801BC24(u8 *target);
