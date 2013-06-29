typedef void (*vvptr)();

struct startmenuentry {
    char *label;
    vvptr action;
};

struct startmenuentry sm_entries[] = {
    { "POKeDEX", &sm_pokedex   },
    { "POKeMON", &sm_pokemon   },
    { "BAG",     &sm_bag       },
    { "#1",      &sm_trainer_1 },
    { "SAVE",    &sm_save      },
    { "OPTION",  &sm_pokemon   },
    { "EXIT",    &sm_exit      },
    { "RETIRE",  &sm_retire    },
    { "#1",      &sm_trainer_2 }
};

const char* sm_descr[] = {
};s

// 0806F280
bool start_menu_handle_input() {

    // modified for readability

    if (super.button_state & KEYPAD_UP) {
        music_play(5);
        start_menu_cursor = start_menu_cursor_move(-1);
    } else if (super.button_state & KEYPAD_DOWN) {
        music_play(5);
        start_menu_cursor = start_menu_cursor_move(1);
    } else {
        goto no_descr_update;
    }

    if (sub_80BF708() == 0 && in_trade_center() != 1 && saveblock2_trainerdata->options_button_style == 0) {
        u8 selected_entry = start_menu_entry_indices[start_menu_cursor];
        char *entry_descr = start_menu_entry_descr[selected_entry];
        start_menu_entry_descr_show(entry_descr, 2);
    }

no_descr_update:
    if (super.button_state & KEYPAD_A)
        music_play(5);
        if (start_menu_is_selected_entry_valid()) {
            u8 selected_entry = start_menu_entry_indices[start_menu_cursor];
            start_menu_active_context = menu_start[selected_entry].action;
            sub_806F394();
        }
        return 0;
    }

    if (super.button_state & (KEYPAD_B | KEYPAD_START) == 0)
        return 0;

    sub_806EF18();
    sub_80F7998();
    sub_806FEA0();
    return 1;
}
