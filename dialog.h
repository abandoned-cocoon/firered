#pragma once
#include "types.h"

#define TEXT_GRAY_ARROW    2
#define TEXT_AUTO_CONTINUE 4

struct typesetter {
	u8 *ccursor;

	u8 rbox_id;
	u8 fbox_id;

	u8 reset_x;
	u8 reset_y;

	u8 cursor_x;
	u8 cursor_y;

	u8 stride_x;
	u8 stride_y;

	uint color0 : 4;
	uint color1 : 4;
	uint color2 : 4;
	uint color3 : 4;

	char padding[2];
};
static_assert_sizeof(struct typesetter, 16);

/*struct dialogsub1 {
	union {
		char font_type;
		struct {
			u8 ft_lo : 4;
			u8 ft_hi : 4;
		};
	}
	char field_1;
	u16 frames_visible_counter;
	int field_4;
};*/

#define ft_hi user_can_skip

struct dialogsub {
	union {
		char fontbyte;
		struct {
			u8 ft_lo : 4;
			u8 ft_hi : 4; // set to 0 after skipping, set to 1 after releasing buttons
		};
	};
	u8  field_1;
	u16 frames_visible_counter;
	u8  field_4;
	u8  field_5;
	u8  field_6;
	u8  acknowledged;
};

struct dialog {
	struct typesetter tys;
	u32 field_10;
	struct dialogsub sub;
	u8 mode;
	u8 text_speed_inv; // lower is faster
	u8 wait_frames;
	u8 remaining_scroll_lines;
	u8 field_20;
	u8 japanese;
};

extern u32 textflags;
extern struct dialog dialogs[32];
