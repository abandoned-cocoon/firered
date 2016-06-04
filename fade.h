#pragma once
#include "types.h"

typedef struct palette {
	u16 color[16];
} palette;

struct fade_ctrl {
	u16 bg_pal_bitfield;
	u16 obj_pal_bitfield;
	u16 scale;     // _____bbb bbaaaaaa  b=scale a=unknown
	u16 mix_color; // xbbbbbgg gggrrrrr  x=active?
	u8 field_8;    //          ________
	u8 field_9;    //          ______ss  s=style 0, 1 or 2
	u8 field_A;    //          _____t__  t=bg(0) or obj(1)
	u8 field_B;    //          ________
	// possibly one more u32
};

//* 020371F8
extern palette palette_bg_unfaded[16];
//* 020373F8
extern palette palette_obj_unfaded[16];
//* 020375F8
extern palette palette_bg_faded[16];
//* 020377F8
extern palette palette_obj_faded[16];
//* 020379F8
extern struct { u8 unknown[12]; } stru_20379F8[16]; // unused?
//* 02037AB8
extern struct fade_ctrl pal_fade_control;
//* 02037AC8
extern u32 copy_of_fade_ctrl_pal_bitfield;
//* 02037ACC
extern palette palette_decompress_buffer[32];
