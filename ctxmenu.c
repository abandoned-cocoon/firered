typedef void (*cvptr)(task_id);

struct ctxmenuentry {
    char *label;
    cvptr action;
};

// 08070588
bool sub_08070588() {
    // TODO
}

// 0811FA78
void sub_0811FA78(task_id c) {
    sub_08070588(~1, ~2, 0, 0x10, 0);
    task[c].func = task_0811FAB4;
}

// 08122D4C
void ctxmenu_summary(task_id c) {
    sound_play(5);
    ptr_brmo2->field_4 = sub_08122D78;
    sub_0811FA78(c);
}

// 08122D4C
void ctxmenu_store(task_id c) {
    sound_play(5);
    var_8004 = (i16)(i8)brm->pokesel1;
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
        task[c].func = task_081203B8;
    }
}

// 0845A618
struct menuentry pokemon_context_menu[] = {
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
    { "Trade",      nop                 },
#define hm(name) \
    { #name,        ctx_hm              },
#include "hm.x"
};
