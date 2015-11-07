#pragma once
#include "types.h"

/* Usage of TASK_PRIV:
	void task_FOOBAR(task_id c) {
		typedef struct {
			u32 privdata1;
			u8  privdata2;
			u16 privdata3[4];
			void *privdata4;
		} TASK_PRIV;
		...
		foobar(priv->privdata4);
		...
	}
*/
#define TASK_PRIV priv_t; priv_t *priv = (priv_t*) &task[c]->priv
#define TASK_PRIV_ASSIGN32(t, i, v) ((t).priv[i] = (v)>>16, (t).priv[i+1] = (v)&0xFFFF)

#define NUM_TASKS 16

typedef u8 task_id;

struct task_t {
	void (*funcptr)(task_id);
	u8 in_use;
	task_id prev;
	task_id next;
	u8 priority;
	u16 priv[16];
};

extern struct task_t task[16];

task_id task_add(void (*funcptr)(task_id), u8 priority);
void    task_insert_sorted(task_id ins);
void    task_del(task_id c);
void    task_exec(void);
task_id task_get_first(void);
void    task_null(task_id c);
bool    task_is_running(void (*funcptr)(task_id));
task_id task_find_id_by_funcptr(void (*funcptr)(task_id));
