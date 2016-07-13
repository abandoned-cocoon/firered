#include "battle_interface.h"
#include "gpu.h"
#include "object.h"
#include "task.h"

// 08010508
/* void c2_pre_battle_2_single() {
	task_exec();
	objc_exec();
	obj_sync_something();
	u8 mid = link_get_multiplayer_id();
	b_dp08_ptr->multiplayer_id_B5 = mid;

	switch (b_preparation_phase) {

	case 0:
		if (dma_08001960() == 0) {
			gpu_sync_bg_show(0);
			gpu_sync_bg_show(1);
			gpu_sync_bg_show(2);
			gpu_sync_bg_show(3);
			sub_80357C8();
			b_preparation_phase = 1;
		}
		if (link_mode_is_wireless)
			sub_80FCEA8();
		break;

	case 1:
		if (battle_type_flags & BATTLE_LINK) {
			if (byte_3003F64 == 0)
				break;
			if (sub_800A4BC()) {
				// TODO
				b_preparation_phase = 2;
			}
			if (link_mode_is_wireless)
				sub_80FCD74(0, 0);
		} else {
			battle_type_flags |= BATTLE_WILD;
			b_preparation_phase = 15;
			berry_0801017C();
		}
		break;

	// TODO

	case 15:
		b_setup_bx();
		b_preparation_phase++;
		b_preparation_subphase = 0;
		b_preparation_subsubphase = 0;
		break;

	case 16:
		b_preparation_subphase++;
		if (!battle_load_something(/* TODO * /))
			break;

		b_callback1_backup = super.callback1;
		super.callback1 = &c1_battle_exec_bc_and_bx;
		set_callback2(&c2_battle);

		if (battle_type_flags & BATTLE_LINK)
			battle_type_flags |= 0x20;
		break;

	}
} */

// 02022974
u16 vblank_cb_battle_BG0HOFS;
u16 vblank_cb_battle_BG0VOFS;
u16 vblank_cb_battle_BG1HOFS;
u16 vblank_cb_battle_BG1VOFS;
u16 vblank_cb_battle_BG2HOFS;
u16 vblank_cb_battle_BG2VOFS;
u16 vblank_cb_battle_BG3HOFS;
u16 vblank_cb_battle_BG3VOFS;
u16 vblank_cb_battle_WIN0H;
u16 vblank_cb_battle_WIN0V;
u16 vblank_cb_battle_WIN1H;
u16 vblank_cb_battle_WIN1V;

// 080116F4
void vblank_cb_battle() {
	rand();
    lcd_io_set(16, vblank_cb_battle_BG0HOFS);
    lcd_io_set(18, vblank_cb_battle_BG0VOFS);
    lcd_io_set(20, vblank_cb_battle_BG1HOFS);
    lcd_io_set(22, vblank_cb_battle_BG1VOFS);
    lcd_io_set(24, vblank_cb_battle_BG2HOFS);
    lcd_io_set(26, vblank_cb_battle_BG2VOFS);
    lcd_io_set(28, vblank_cb_battle_BG3HOFS);
    lcd_io_set(30, vblank_cb_battle_BG3VOFS);
    lcd_io_set(64, vblank_cb_battle_WIN0H);
    lcd_io_set(68, vblank_cb_battle_WIN0V);
    lcd_io_set(66, vblank_cb_battle_WIN1H);
    lcd_io_set(70, vblank_cb_battle_WIN1V);
    gpu_sprites_upload();
    copy_queue_process();
    gpu_pal_upload();
    sub_8087F54();
}
