#include "move_interp.h"

struct stack {
	void *values[8];
	u8 height;
};

static inline void stack_push(struct stack *s, void *value) {
	s->value[s->height++] = value;
}

static inline void *stack_pop(struct stack *s) {
	return s->value[--s->height];
}

// 08017520
void b_stack8_push(void *values) {
	stack_push(brb->stack8, values);
}

// 08017544
void b_stack_push_move_cursor() {
	stack_push(brb->stack8, b_move_cursor);
}

// 0801756C
void b_stack_pop_move_cursor() {
	b_move_cursor = stack_pop(brb->stack8);
}

// 0801BC24
void sub_0801BC24(u8 *target) {
	b_stack1_push(b_move_cursor);
	b_move_cursor = target;
	b_stack2_push(bc_func);
	bc_func = &bc_move_exec;
}
