#pragma once
#include "types.h"

extern u8 choice_x; // 0203ADE4
extern u8 choice_y; // 0203ADE5
extern u8 choice_cur; // 0203ADE6
extern u8 choice_min; // 0203ADE7
extern u8 choice_max; // 0203ADE8
extern u8 choice_rboxid; // 0203ADE9
extern u8 choice_fboxid; // 0203ADEA
extern u8 choice_0203ADEB; // 0203ADEB
extern u8 choice_y_stride; // 0203ADEC
extern u8 choice_0203ADED; // 0203ADED
extern u8 choice_0203ADEE; // 0203ADEE
extern u8 choice_silent_confirm; // 0203ADEF

u8 start_menu_cursor_move(u8 delta);
