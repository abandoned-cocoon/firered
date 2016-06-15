#include "move_anim.h"

// 02037ED4
u8 *ma_cursor;
// 02037ED8
u8 *ma_return_addr;
// 02037EE2
u8 ma_active_tasks;

// 02037F02
u16 ma_argument_buffer[4];

void maXX_transfer_arguments() {
	u8 n = ma_read_byte(&ma_cursor);
	u16 *target = &ma_argument_buffer;	
	while(n--)
		*target++ = ma_read_half(&ma_cursor);
}

// 0807291C
void ma02_instanciate_template() {
	u8 skip        = ma_read_byte(&ma_cursor);
	void *template = ma_read_word(&ma_cursor);
	u8 ctrl        = ma_read_byte(&ma_cursor);
	maXX_transfer_arguments();

	u8 battle_side = (ctrl & 0x80)
		? b_attacker_side_ll
		: b_defender_side_ll;

	ctrl &= ~80;

	s8 iv = ((ctrl >= 0x40) ? (ctrl-0x40) : (-ctrl)) + sub_0807685C(battle_side);
	if (iv >= 3) iv = 3;

	u8 t1 = sub_08074480(b_defender_side_ll, 2);
	u8 t2 = sub_08074480(b_defender_side_ll, 3);

	template_instanciate(template, t1, t2, iv);

	ma_active_tasks++;
}

// 08072A10
void ma03_launch_task() {
	u8 skip       = ma_read_byte(&ma_cursor);
	void *funcptr = ma_read_word(&ma_cursor);
	u8 prio       = ma_read_byte(&ma_cursor);
	maXX_transfer_arguments();

	u8 cid = task_add(funcptr, prio);
	funcptr(cid);

	ma_active_tasks++;
}

// 080742A4
void ma2B_make_side_invisible() {
	u8 skip   = ma_read_byte(&ma_cursor);
	u8 target = ma_read_byte(&ma_cursor);
	u8 oid = obj_id_for_side_relative_to_move(target);
	if (oid != 0xFF)
		objects[oid].bitfield2 |= OBJ_BIT2_INVISIBLE;
}

// 080742E0
void ma2C_make_side_visible() {
	u8 skip   = ma_read_byte(&ma_cursor);
	u8 target = ma_read_byte(&ma_cursor);
	u8 oid = obj_id_for_side_relative_to_move(target);
	if (oid != 0xFF)
		objects[oid].bitfield2 &= ~OBJ_BIT2_INVISIBLE;
}

// 083ADF5C
void (*move_anim_cmds[])(void) = {
	ma00_load_graphics,
	ma01_080728D0,
	ma02_instanciate_template,
	ma03_launch_coro,
	ma04_wait_countdown,
	ma05_wait_animation,
	ma06_nullsub,
	ma07_nullsub,
	ma08_finish,
	ma09_play_sound,
	ma0A_08072C30,
	ma0B_0807324C,
	ma0C_set_BLDCNT_to_x3F40_and_BLDALPHA_to_argument,
	ma0D_reset_BLDCNT_and_BLDALPHA,
	ma0E_call,
	maOF_return,
	ma10_080736AC,
	ma11_if_else,
	ma12_cond_if,
	ma13_goto,
	ma14_load_background,
	ma15_load_battle_screen_elements,
	ma16_wait_for_battle_screen_elements_s2,
	ma17_wait_for_battle_screen_elements_s0,
	ma18_load_background_probably,
	ma19_08073BC8,
	ma1A_08073C00,
	ma1B_08073C2C,
	ma1C_08073ED0,
	ma1D_08073FB4,
	ma1E_set_BLDCNT,
	ma1F_0807406C,
	ma20_wait_for_something,
	ma21_08074164,
	ma22_0807339C,
	ma23_08073484,
	ma24_skip5,
	ma25_080737E4,
	ma26_play_song_maybe,
	ma27_08073E10,
	ma28_080741C8,
	ma29_08074214,
	ma2A_0807423C,
	ma2B_make_side_invisible,
	ma2C_make_side_visible,
	ma2D_08074320,
	ma2E_080743C8,
	ma2F_stop_music
};
