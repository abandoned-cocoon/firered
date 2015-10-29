#include "task.h"

#ifndef NO_RAM
struct task_t task[16];
#endif

// 0807741C
task_id task_add(void (*funcptr)(task_id), u8 priority) {
	for (task_id c=0; c<NUM_TASKS; c++) {
		if (task[c].in_use) continue;
		task[c].funcptr = funcptr;
		task[c].priority = priority;
		task_insert_sorted(c);
		memset(&task[c].priv, 0, sizeof(task[c].priv));
		//((void *(*)(void *, int, size_t))0x081E5E79)(&task[c].priv, 0, sizeof(task[c].priv));
		task[c].in_use = 1;
		return c;
	}
	return 0;
}

// 08077470
void task_insert_sorted(task_id ins) {
	task_id c = task_get_first();
	if (c == NUM_TASKS) {
		task[ins].prev = 0xFE;
		task[ins].next = 0xFF;
		return;
	}

	for (; task[c].priority < task[ins].priority; c = task[c].next)
		if (task[c].next == 0xFF) {
			task[c].next = ins;
			task[ins].prev = c;
			task[ins].next = 0xFF;
			return;
		}

	task_id prev = task[c].prev;
	task_id next = c;
	task[ins].prev = prev;
	task[ins].next = next;
	if (prev != 0xFE) task[prev].next = ins;
	if (next != 0xFF) task[next].prev = ins;
}

// 08077508
void task_del(task_id c) {
	if (task[c].in_use == false) return;
	task[c].in_use = false;
	task_id prev = task[c].prev;
	task_id next = task[c].next;
	if (prev != 0xFE) task[prev].next = next;
	if (next != 0xFF) task[next].prev = prev;
}

// 08077578
void task_exec() {
	task_id c = task_get_first();
	if (c == NUM_TASKS) return;
	while (c != 0xFF) {
		task[c].funcptr(c);
		c = task[c].next;
	}
}

// 080775A8
task_id __attribute__((noinline)) task_get_first() {
	task_id c;
	for (c=0; c<NUM_TASKS; c++)
		if (task[c].in_use && task[c].prev == 0xFE) break;
	return c;
}

// 080775E4
// void task_null(task_id c) {
// 	// nothing
// }

// two functions missing here

// 08077650
bool task_is_running(void (*funcptr)(task_id)) {
	for (task_id c=0; c<NUM_TASKS; c++)
		if (task[c].in_use && task[c].funcptr == funcptr)
			return true;
	return false;
}
	
// 08077688
task_id task_find_id_by_funcptr(void (*funcptr)(task_id)) {
	for (task_id c=0; c<NUM_TASKS; c++)
		if (task[c].in_use && task[c].funcptr == funcptr)
			return c;
	return 0xFF;
}
