// 08005680
bool text_is_it_time_to_continue() {
	if (textflags & TEXT_AUTO_CONTINUE)
		return text_is_it_time_to_continue_autopilot();
	if (super.buttons_new_remapped & (KEYPAD_A|KEYPAD_B)) {
		audio_play(sound_generic_clink);
		return true;
	}
	return false;
}

// ...

// 08006448
// font_render_tiny_en_jp

// 080064D0
// get_font_width_tiny_en_jp

// 080064F0
// font_render_other_en_jp

// 08006598
// get_font_width_other_en_jp

// 080065B8
// font_render_black_en_jp

// 080066F8
// get_font_width_black_en_jp

// 080067CC
// get_font_width_black_en

// 080067EC
// font_render_blue_en_jp

// 0800692C
// get_font_width_blue_en_jp

// 08006958
// font_render_red_en_jp

// 08006A98
// get_font_width_red_en_jp

// 08006AC4
// font_render_tiny_jp_2
