#include "main.h"
#include "object.h"

// 080003A4
void mainloop() {
	RegisterRamReset(0xFF);
	*(u16*)0x5000000 = 0x7FFF; // bg palette 0 color 0 = white
	lcd_io_copy_queue_clean();
	WAITCNT = 0x4014;
	init_counters();
	init_irq();
	m4aSoundInit();
	lcd_enable_vcount_irq_at_150px();
	sub_080F86C4(); // something about serial port
	flash_timeout_start_on_timer_2_when_flash_present();
	init_saveblock_ptrs_and_copyright_message();
	sound_sources_off();
	dma3_transfer_queue_clear();
	gpu_init_bgs();
	init_malloc(0x02000000, 0x1C000);
	set_boxes_ptr_to_stdboxes();
	byte_03003530 = 0;
	help_system_enabled = 0;
	fullscreen_save_deactivate();
	AGBPrintInit();
	byte_030030E4 = 0;

	while (true) {
		load_keys();
		if (byte_03003530 == 0) {
			u8 pattern = KEYPAD_A
			           | KEYPAD_A
			           | KEYPAD_START
			           | KEYPAD_SELECT;
			if (super.buttons_held & pattern == pattern) {
				// something about resetting the game
				sub_081E09D4();
				sub_081E08F8();
				sub_080008D8();
			}
		}
		if (sub_080582E0() == 1) {
			byte_030030E4 = 0;
			callback_wrapper();
			byte_030030E4 = 0;
		} else {
			// again, something about link connection
			byte_030030E4 = 0;
			callback_wrapper();
			if (sub_08058274() == 1) {
				super.buttons_new_remapped = 0;
				copy_queue_clear();
				byte_030030E4 = 0;
				callback_wrapper();
				byte_030030E4 = 0;
			}
		}
		gametime_increment();
		sound_something();
		wait_for_superbit_eg_vmatch(); // wait for next frame
	}
}

// 08000510
void exec_c1_and_c2() {
    if (fullscreen_save())
        return;
    if (help_system())
        return;
    if (super.callback1) super.callback1();
    if (super.callback2) super.callback2();
}

// 080005E8
void load_keys() {
	u16 _held = (KEYINPUT ^ 0x3FF);
	u16 _new = _held & ~super.buttons_held;

	super.buttons_new               = _new;
	super.buttons_new_remapped      = _new;
	super.buttons_new_and_keyrepeat = _new;

	if (_held == 0 || _held != super.buttons_held_remapped) {
		super.keypad_countdown = keypad_initial_countdown;
	} else if (--super.keypad_countdown == 0) {
		super.buttons_new_and_keyrepeat = _held;
		super.keypad_countdown = keypad_followup_countdown;
	}

	if (sav2i->options_button_style == L_IS_A) {
		if (super.buttons_new_remapped & BUTTON_L)
			super.buttons_new_remapped |= BUTTON_A;

		if (super.buttons_held_remapped & BUTTON_L)
			super.buttons_held_remapped |= BUTTON_A;
	}
	// TODO
}

// 08000890
void wait_for_superbit_eg_vmatch() {
	volatile u16 *bits = (u16*)&super.bits_to_wait_for;
	*bits &= ~1;
	while (*bits & 1 == 0);
}
