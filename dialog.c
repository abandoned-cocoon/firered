#include "dialog.h"

#ifndef NO_RAM
// 03003E50
u32 textflags;
#endif

struct dialog dialogs[32];

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

uint dialog_update(struct dialog *dialog) {

	typesetter *tys  = &dialog->tys;
	dialogsub *dsub = &dialog->sub;
	
	switch (dialog->mode) {
		// Parse mode
		case 0u:
			if (super.buttons2_held_remapped & (keypad_b|keypad_a)) {
				if (dsub->user_can_skip)
					dialog->wait_frames = 0;
			}

			if (!dialog->wait_frames || dialog->text_speed_inv == 0) {
				if ( textflags & 4 )
					dialog->wait_frames = 1;
				else
					dialog->wait_frames = dialog->text_speed_inv;

				int c = *tys->ccursor++;
				switch ( c )
				{
					case 0xFE:              // Line break
						tys->cursor_x = tys->reset_x;
						tys->cursor_y += tys->stride_y + fboxes_ptr[tys->fbox_id].x;
						return 2;
					case 0xFD:              // Variable (?)
						tys->ccursor++;
						return 2;
					case 0xFC:              // Special control
						c = *tys->ccursor++;
						switch ( c )
						{
							// cases 0x01..0x18
							case 1:     // Color foreground
								tys->color1 = *tys->ccursor++;
								font_lookup_table_calc(tys->color1, tys->color2, tys->color3);
								return 2;
							case 2:     // Color background
								tys->color2 = *tys->ccursor++;
								font_lookup_table_calc(tys->color1, tys->color2, tys->color3);
								return 2;
							case 3:     // Color shadow
								tys->color3 = *tys->ccursor++;
								font_lookup_table_calc(tys->color1, tys->color2, tys->color3);
								return 2;
							case 4:     // Color fg and bg and shadow
								tys->color1 = *tys->ccursor++;
								tys->color2 = *tys->ccursor++;
								tys->color3 = *tys->ccursor++;
								font_lookup_table_calc(tys->color1, tys->color2, tys->color3);
								return 2;
							case 5:     // Nop
								tys->ccursor++;
								return 2;
							case 6:     // Select font
								dsub->font = *tys->ccursor++;
								return 2;
							case 7:     // Nop (but stay)
								return 2;
							case 8:     // Pause
								dialog->wait_frames = *tys->ccursor++;
								dialog->mode = 6;
								return 2;
							case 9:     // Wait for key
								dialog->mode = 1;
								if ( textflags & 4 )
									LOBYTE(dsub->frames_visible_counter) = 0;
								return 3;
							case 0xA:   // Wait for audio
								dialog->mode = 5;
								return 3;
							case 0xB:   // Play song
								u8 lo = *tys->ccursor++;
								u8 hi = *tys->ccursor++;
								if (prev_quest_mode < 2 || prev_quest_mode >= 4)
									song_play_for_text((hi << 8) | lo);
								return 2;
							case 0x10:  // Play sound
								u8 lo = *tys->ccursor++;
								u8 hi = *tys->ccursor++;
								audio_play((hi << 8) | lo);
								return 2;
							case 0xC:   // Escape
								c = *tys->ccursor++;
								break;
							case 0xD:   // Set x
								tys->cursor_x = tys->reset_x + *tys->ccursor++;
								return 2;
							case 0xE:   // Set y
								tys->cursor_y = tys->reset_y + *tys->ccursor++;
								return 2;
							case 0xF:   // Clear
								rboxid_clear_pixels(
									tys->rbox_id,
									(tys->color2 << 4) | tys->color2);
								return 2;
							case 0x17:  // Pause music
								MPlayStop_rev01(&stru_3007300);
								return 2;
							case 0x18:  // Resume music
								m4aMPlayContinue(&stru_3007300);
								return 2;
							case 0x11:  // ???
								int t = *tys->ccursor++;
								if ( t <= 0 )
									return 2;
								tys->cursor_x += t;
								return 0;
							case 0x12:  // ???
								tys->cursor_x = tys->reset_x + *tys->ccursor++;
								return 2;
							case 0x13:  // ???
								int t = tys->reset_x + *tys->ccursor++;
								t = t - tys->cursor_x;
								if ( t <= 0 )
									return 2;
								tys->cursor_x += t;
								return 0;
							case 0x14:  // ???
								dialog->field_20 = *tys->ccursor++;
								return 2;
							case 0x15:  // Japanese on (Spread text)
								dialog->japanese = 1;
								return 2;
							case 0x16:  // Japanese off (default)
								dialog->japanese = 0;
								return 2;
							default:
								break;
						}
						break;
					case 0xFB:              // Next page
						dialog->mode = 2;
						dialog_80054C8(dialog);
						return 3;
					case 0xFA:              // Scroll line
						dialog->mode = 3;
						dialog_80054C8(dialog);
						return 3;
					case 0xF9:              // Allows you to select characters from a second page
						c = *tys->ccursor++ | 0x100;
						break;
					case 0xF8:
						int t = *tys->ccursor++;
						byte_3003E20 = font_render_F8(
										   tys->rbox_id,
										   t,
										   tys->cursor_x,
										   tys->cursor_y);

						tys->cursor_x += byte_3003E20 + tys->stride_x;
						return 0;
					case 0xFF:              // End
						return 1;
					default:
						break;
				}
				switch (dsub->font)
				{
					case 0:
						font_render_tiny_en_jp(c, dialog->japanese);
						break;
					case 1:
						font_render_other_en_jp(c, dialog->japanese);
						break;
					case 2:
						font_render_black_en_jp(c, dialog->japanese);
						break;
					case 3:
						sub_8006724(c, dialog->japanese);
						break;
					case 4:
						font_render_blue_en_jp(c, dialog->japanese);
						break;
					case 5:
						font_render_red_en_jp(c, dialog->japanese);
						break;
					default:
						break;
				}
				sub_8003014(dialog);
				char new_x = tys->cursor_x;
				if ( dialog->field_20 ) {
					tys->cursor_x += byte_3003E20;
					int t = dialog->field_20 - (unsigned __int8)byte_3003E20;
					if ( t <= 0 )
						return 0;
					nullsub_2();
					new_x += byte_3003E20 + t;
				} else {
					new_x += byte_3003E20;
					if (dialog->japanese) {
						new_x += = tys->stride_x
					}
				}
				tys->cursor_x = new_x;
				return 0;
			}
			--dialog->wait_frames;
			if ( textflags & 1 ) {
				if ( super.buttons3_new_remapped & 3 ) {
					dsub->user_can_skip = 0;
					dialog->wait_frames = 0;
				}
			}
			return 3;
		case 1u:
			if (dialog_is_it_time_to_continue(dialog))
				dialog->mode = 0;
			return 3;
		case 2u:
			if ( dialog_is_it_time_to_continue_plus_80054F8(dialog) << 16 ) {
				rboxid_clear_pixels(
					tys->rbox_id,
					(tys->color2 << 4) | tys->color2);
				tys->cursor_x = tys->reset_x;
				tys->cursor_y = tys->reset_y;
				dialog->mode = 0;
			}
			return 3;
		case 3u:
			if ( dialog_is_it_time_to_continue_plus_80054F8(dialog) << 16 ) {
				sub_80055D4(dialog);
				dialog->remaining_scroll_lines = tys->stride_y + fboxes_ptr[tys->fbox_id].x;
				tys->cursor_x = tys->reset_x;
				dialog->mode = 4;
			}
			return 3;
		case 4u:
			uint rlines = dialog->remaining_scroll_lines;
			u8 speed_index = saveblock2_trainerdata->options_text_speed_maybe & 7;
			if (rlines > 0) {
				if (rlines >= text_speeds[speed_index]) {
					rboxid_vertical_scroll(
						tys->rbox_id,
						0,
						text_speeds[speed_index],
						(tys->color2 << 4) | tys->color2);
					dialog->remaining_scroll_lines -= text_speeds[speed_index];
				} else {
					rboxid_vertical_scroll(
						tys->rbox_id,
						0,
						rlines,
						(tys->color2 << 4) | tys->color2);
					dialog->remaining_scroll_lines = 0;
				}
				rboxid_upload_a(tys->rbox_id, 2u);
			} else {
				dialog->mode = 0;
			}
			return 3;
		case 5u:
			if ( !mplay_has_finished_maybe() )
				dialog->mode = 0;
			return 3;
		case 6u:
			if ( dialog->wait_frames )
				--dialog->wait_frames;
			else
				dialog->mode = 0;
			return 3;
		default:
			return 1;
	}
}
