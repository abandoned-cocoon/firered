/* Usage of CORO_PRIV:
	void coro_FOOBAR(coro_id c) {
		typedef struct {
			u32 privdata1;
			u8  privdata2;
			u16 privdata3[4];
			void *privdata4;
		} CORO_PRIV;
		...
		foobar(priv->privdata4);
		...
	}
*/
#define CORO_PRIV priv_t; priv_t *priv = (priv_t*) &coro[c]->priv

#define NUM_COROS 16

typedef u8 coro_id;

struct coro_t {
  void (*funcptr)(coro_id);
  u8 in_use;
  u8 prev;
  u8 next;
  u8 priority;
  u16 args[16];
};

struct coro_t coro[16];

// 0807741C
coro_id coro_add(void (*funcptr)(coro_id), u8 priority) {
	for (coro_id c=0; c<NUM_COROS; c++) {
		if (coro[c]->in_use) continue;
		coro[c].funcptr = funcptr;
		coro[c].priority = priority;
		coro_insert_sorted(c);
		memset(&coro[c].args, 0, sizeof(coro[c].args));
		coro[c].in_use = 1;
	}
	return 0;
}

// 08077470
void coro_insert_sorted(coro_id ins) {
	coro_id c = coro_get_first();
	if (c == NUM_COROS) {
		coro[ins].prev = 0xFE;
		coro[ins].next = 0xFF;
		return;
	}

	for (; coro[c].priority < coro[ins].priority; c = coro[c].next)
		if (coro[c].next == 0xFF) {
			coro[c].next = ins;
			coro[ins].prev = c;
			coro[ins].next = 0xFF;
			return;
		}

	coro_id prev = coro[c].prev;
	coro_id next = c;
	coro[ins].prev = prev;
	coro[ins].next = next;
	if (prev != 0xFE) coro[prev].next = ins;
	if (next != 0xFF) coro[next].prev = ins;
}

// 08077508
void coro_del(coro_id c) {
	if (coro[c].in_use == false) return;
	coro[c].in_use = false;
	coro_id prev = coro[c].prev;
	coro_id next = coro[c].next;
	if (prev != 0xFE) coro[prev].next = next;
	if (next != 0xFF) coro[next].prev = prev;
}

// 08077578
void coro_exec() {
	coro_id c = coro_get_first();
	if (c == NUM_COROS) return;
	while (c != 0xFF) {
		coro[c].funcptr(c);
		c = coro[c].next;
	}
}

// 080775A8
coro_id coro_get_first() {
	for (coro_id c=0; c<NUM_COROS; c++)
		if (coro[c].in_use && coro[c].prev == 0xFE) break;
	return c;
}

// 080775E4
void coro_null(coro_id c) {
	// nothing
}

// two functions missing here

// 08077650
bool coro_is_running(void (*funcptr)(coro_id)) {
	for (coro_id c=0; c<NUM_COROS; c++)
		if (coro[c]->in_use && coro[c]->funcptr == funcptr)
			return true;
	return false;
}
	
// 08077688
coro_id coro_find_id_by_funcptr(void (*funcptr)(coro_id)) {
	for (coro_id c=0; c<NUM_COROS; c++)
		if (coro[c]->in_use && coro[c]->funcptr == funcptr)
			return c;
	return 0xFF;
}