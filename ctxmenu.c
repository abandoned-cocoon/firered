#include "ctxmenu.h"
#include "audio.h"
#include "vars.h"

typedef void (*cvptr)(u8);

struct brmo2 { // TODO
	u8 field_0;
	u8 field_1;
	u8 field_2;
	u8 field_3;
	void (*funcptr_4)(void); // TODO: Correct signature
	u8 field_8;
	u8 field_9;
	u8 field_A;
	u8 field_B;
	u8 field_C;
	u8 field_D;
	u8 field_E;
	u8 seq[8];
	u8 cur;
	u8 unknown[540];
	u16 field_234;
	u16 field_236;
	u8 field_238;
};

struct brmo { // TODO
	void *func_c2;
	u8 field_4;
	u8 field_5;
	u8 field_6;
	u8 field_7;
	u8 field_8;
	u8 pokesel1;
	u8 pokesel2;
	u8 field_B;
	u8 field_C;
	u8 field_D;
	u16 field_E;
};

extern struct brmo brm; // TODO
extern struct brmo2* ptr_brmo2;

struct ctxmenuentry {
	char *label;
	cvptr action;
};

void task_0811FAB4(task_id c);
void task_081203B8(task_id c);
void ctxmenu_summary_08122D78();

// 0811FA78
void sub_0811FA78(task_id c) {
	sub_08070588(~1, ~2, 0, 0x10, 0);
	task[c].funcptr = task_0811FAB4;
}

// 0811FAB4
// void task_0811FAB4(task_id c) {
// 	
// }

// 081203B8
// void task_081203B8(task_id c) {
// 	
// }

// 08122D4C
void ctxmenu_summary(task_id c) {
	sound_play(5);
	ptr_brmo2->funcptr_4 = ctxmenu_summary_08122D78;
	sub_0811FA78(c);
}

// 08122D78
// void ctxmenu_summary_08122D78() {
// 	
// }

// ???
void ctxmenu_store(task_id c) {
	sound_play(5);
	var_8004 = (i16)(i8)brm.pokesel1;
	sub_0811FA78(c);
}

// 081240F4
void ctxmenu_shift_sendout(task_id c) {
	sound_play(5);
	sub_08121CE4(ptr_brmo2->field_C);
	if (sub_08127AC0() == 1)
		sub_0811FA78(c);
	else {
		sub_08121CE4(ptr_brmo2->field_D);
		sub_081202F8(displayed_string, 1);
		task[c].funcptr = task_081203B8;
	}
}

// 0845A618
struct ctxmenuentry pokemon_context_menu[] = {
	{ "Summary",    ctxmenu_summary     },
	{ "Switch",     ctxmenu_switch      },
	{ "Cancel",     ctxmenu_cancel_1    },
	{ "Item",       ctxmenu_item        },
	{ "Give",       ctxmenu_give        },
	{ "Take",       ctxmenu_take_1      },
	{ "Mail",       ctxmenu_mail        },
	{ "Take",       ctxmenu_take_2      },
	{ "Read",       ctxmenu_read        },
	{ "Cancel",     ctxmenu_cancel_2    },
	{ "Shift",      ctxmenu_shift_send  },
	{ "Send out",   ctxmenu_shift_send  },
	{ "Enter",      ctxmenu_enter       },
	{ "No entry",   ctxmenu_no_entry    },
	{ "Store",      ctxmenu_store       },
	{ "Register",   ctxmenu_register    },
	{ "Trade",      ctxmenu_trade_1     },
	{ "Trade",      ctxmenu_trade_nop   },
#define hm(name) \
	{ #name,        ctxmenu_hm          },
#include "hm.x"
};
