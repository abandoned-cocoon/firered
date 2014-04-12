// 0801BC24
void sub_0801BC24(u8 *target) {
	b_stack1_push(b_move_cursor);
	b_move_cursor = target;
	b_stack2_push(bc_func);
	bc_func = &bc_move_exec;
}
