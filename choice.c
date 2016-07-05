#include "audio.h"
#include "choice.h"
#include "main.h"

#ifndef NO_RAM
u8 choice_x; // 0203ADE4
u8 choice_y; // 0203ADE5
u8 choice_cur; // 0203ADE6
u8 choice_min; // 0203ADE7
u8 choice_max; // 0203ADE8
u8 choice_rboxid; // 0203ADE9
u8 choice_fboxid; // 0203ADEA
u8 choice_0203ADEB; // 0203ADEB
u8 choice_y_stride; // 0203ADEC
u8 choice_0203ADED; // 0203ADED
u8 choice_0203ADEE; // 0203ADEE
u8 choice_silent_confirm; // 0203ADEF

// u16 choice_0203ADF0; // 0203ADF0
// u8 choice_0203ADF2; // 0203ADF2

// u8 yesno_rboxid; // 0203ADF3
// u8 yesno_0203ADF4; // 0203ADF4
#endif

// 0810F774
u8 choice_setup(u8 rboxid, u8 fboxid, u8 x, u8 y, u8 y_stride, u8 num_choices, u8 preselected, u8 silent_confirm) {
	choice_x = x;
	choice_y = y;
	choice_min = 0;
	choice_max = num_choices - 1;
	choice_rboxid = rboxid;
	choice_fboxid = fboxid;
	choice_y_stride = y_stride;
	choice_silent_confirm = silent_confirm;

	if (preselected <= choice_max)
		choice_cur = preselected;
	else
		choice_cur = 0;

	start_menu_cursor_move(0);
	return choice_cur;
}

// 0810F7D8
u8 choice_setup_simple(u8 rboxid, u8 fboxid, u8 x, u8 y, u8 y_stride, u8 num_choices, u8 preselected) {
	return choice_setup(rboxid, fboxid, x, y, y_stride, num_choices, preselected, 0);
}

// 0810F818 (unused?)
u8 choice_setup_simple_2(u8 rboxid, u8 fboxid, u8 x, u8 y, u8 num_choices, u8 preselected) {
	enum { AXIS_X=0, AXIS_Y=1 };
	u8 y_stride = fbox_get_suggested_stride_for_axis(fboxid, AXIS_Y);
	return choice_setup_simple(rboxid, fboxid, x, y, y_stride, num_choices, preselected);
}

// 0810F874
void choice_cursor_something() {
	// TODO
}

// 0810F904
// 0810F948
u8 start_menu_cursor_move(u8 delta) {
	u8 choice_old = choice_cur;
	i8 choice_prospective = choice_cur + delta;

	if (choice_prospective < (i8)choice_min) // ASDFG signedness
		choice_cur = choice_max; // wrap around

	else if (choice_prospective >= choice_max)
		choice_cur = choice_min; // wrap around

	else
		choice_cur = choice_prospective;

	choice_cursor_something(choice_old, choice_cur);
	return choice_cur;
}


// 0810F98C
void choice_set(u8 choice) {
	choice_cur = choice;
}

// 0810F998
i8 choice_update_from_keypad() {
	if (super.buttons_new_remapped & KEYPAD_A)
	{
		if (!choice_silent_confirm)
			audio_play(sound_generic_clink);

		return choice_cur;
	}

	if (super.buttons_new_remapped & KEYPAD_B)
		return -1;

	i8 delta;
	if (super.buttons_new_remapped & KEYPAD_UP) {
		audio_play(sound_generic_clink);
		delta = -1;

	} else if (super.buttons_new_remapped & KEYPAD_DOWN) {
		audio_play(sound_generic_clink);
		delta = 1;

	} else {
		return -2;
	}

	start_menu_cursor_move(delta);
	return -2;
}

// 0810FA04
i8 choice_update_from_keypad__only_sound_on_change() {
	// TODO
}

// 0810FA84
i8 choice_update_from_keypad__with_keyrepeat() {
	// TODO
}

// 0810FAF0
i8 choice_update_from_keypad__only_sound_on_change__with_keyrepeat() {
	// TODO
}
