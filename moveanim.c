// 02037ED4
u8 *ma_cursor;
// 02037ED8
u8 *ma_return_addr;
// 02037EE2
u8 ma_active_coros;

// 02037F02
u16 ma_argument_buffer[4];

void maXX_transfer_arguments() {
	u8 n = ma_read_byte(&ma_cursor);
	u16 *target = &ma_argument_buffer;	
	while(n--)
		*target++ = ma_read_half(&ma_cursor);
}

void ma02_instanciate_template() {
	u8 skip        = ma_read_byte(&ma_cursor);
	void *template = ma_read_word(&ma_cursor);
	u8 ctrl        = ma_read_byte(&ma_cursor);
	maXX_transfer_arguments();

	u8 battle_side = (ctrl & 0x80)
		? battle_side_of_attacker_ll
		: battle_side_of_defender_ll;

	ctrl &= ~80;

	s8 iv = ((ctrl >= 0x40) ? (ctrl-0x40) : (-ctrl)) + sub_0807685C(battle_side);
	if (iv >= 3) iv = 3;

	u8 t1 = sub_08074480(battle_side_of_defender_ll, 2);
	u8 t2 = sub_08074480(battle_side_of_defender_ll, 3);

	template_instanciate(template, t1, t2, iv);

	ma_active_coros++;
}

void ma03_launch_coro() {
	u8 skip       = ma_read_byte(&ma_cursor);
	void *funcptr = ma_read_word(&ma_cursor);
	u8 prio       = ma_read_byte(&ma_cursor);
	maXX_transfer_arguments();

	u8 cid = coro_add(funcptr, prio);
	funcptr(cid);

	ma_active_coros++;
}
