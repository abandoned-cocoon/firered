#include "battle.h" // dependency removable?
#include "move_interp.h"

static inline void stack_push(struct br_stack *s, u8 *value) {
	s->values[s->height++] = (u32)value;
}

static inline u8 *stack_pop(struct br_stack *s) {
	return (u8*)s->values[--s->height];
}

static inline b_stack_push_bc(void *value) {
	stack_push(b_resources->bc_stack, value);
}


// 08015C74
void bc_move_exec_toplevel() {
    if (b_buffers_awaiting_execution_bitfield == 0)
        (*move_cmds[*b_movescr_cursor])();
}

// 08017520
void b_stack_push_move_addr(u8 *value) {
	stack_push(b_resources->move_script_stack, value);
}

// 08017544
void b_stack_push_move_cursor() {
	stack_push(b_resources->move_script_stack, b_move_cursor);
}

// 0801756C
void b_stack_pop_move_cursor() {
	b_move_cursor = stack_pop(b_resources->move_script_stack);
}

// 0801BC24
void b_push_move_exec(u8 *target) {
	b_stack_push_move_cursor(b_move_cursor);
	b_move_cursor = target;

	b_stack_push_bc(bc);
	bc = &bc_move_exec_toplevel;
}
