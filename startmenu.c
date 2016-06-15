#include "flags.h"
#include "main.h"
#include "save.h"
#include "startmenu.h"

typedef void (*vvptr)(void);

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

// 083A7394
const char* sm_descr[] = {
    "A device that records POKeMON secrets upon meeting or catching them.",
    "Check and organize POKeMON that are traveling with you in your party.",
    "Equipped with pockets for storing items you bought, received, or found.",
    "Check your money and other game data.",
    "Save your game with a complete record of your progress to take a break.",
    "Adjust various game settings such as text speed, game rules, etc.",
    "Close this MENU window.",
    "Retire from the SAFARI GAME and return to the registration counter.",
    "Check your money and other game data."
};

#ifndef NO_RAM
// 020370F0
void *start_menu_active_context;
// 020370F4
u8 start_menu_cursor;
// 020370F5
u8 start_menu_item_indices_cursor;
// 020370F6
u8 start_menu_item_indices[9];
// 020370FF
u8 start_menu_state_tracker;
#endif

void append_byte(u8 *buffer, u8 *cursor, u8 elem);

// 0806ED54
void start_menu_compose() {
    start_menu_item_indices_cursor = 0;
    if (is_c1_x8057884())
        start_menu_compose_for_unknown();
    else if (in_trade_center())
        start_menu_compose_for_link_room();
    else if (flag_check_is_in_safari_zone())
        start_menu_compose_for_safari();
    else
        start_menu_compose_normal();
}

// 0806ED94
void start_menu_append_item(u8 item_index) {
    append_byte(start_menu_item_indices,
               &start_menu_item_indices_cursor,
                item_index);
}

// 0806EDB0
void start_menu_compose_normal() {
    if (flag_check(FLAG_HAVE_POKEDEX))
        start_menu_append_item(0);
    if (flag_check(FLAG_HAVE_POKEMON))
        start_menu_append_item(1);
    start_menu_append_item(2);
    start_menu_append_item(3);
    start_menu_append_item(4);
    start_menu_append_item(5);
    start_menu_append_item(6);
}

// 0806EE04
void start_menu_compose_for_safari() {
    start_menu_append_item(7);
    start_menu_append_item(1);
    start_menu_append_item(2);
    start_menu_append_item(3);
    start_menu_append_item(5);
    start_menu_append_item(6);
}

// 0806EE34
void start_menu_compose_for_unknown() {
    start_menu_append_item(1);
    start_menu_append_item(2);
    start_menu_append_item(8);
    start_menu_append_item(5);
    start_menu_append_item(6);
}

// 0806EE58
void start_menu_compose_for_link_room() {
    start_menu_append_item(1);
    start_menu_append_item(2);
    start_menu_append_item(3);
    start_menu_append_item(5);
    start_menu_append_item(6);
}

// 0806F280
bool start_menu_handle_input() {

    // modified for readability

    if (super.buttons_new_remapped & KEYPAD_UP) {
        music_play(5);
        start_menu_cursor = start_menu_cursor_move(-1);
    } else if (super.buttons_new_remapped & KEYPAD_DOWN) {
        music_play(5);
        start_menu_cursor = start_menu_cursor_move(1);
    } else {
        goto no_descr_update;
    }

    if (sub_80BF708() == 0 && in_trade_center() != 1 && saveblock2_trainerdata->options_button_style == 0) {
        u8 selected_entry = start_menu_item_indices[start_menu_cursor];
        const char *entry_descr = sm_descr[selected_entry];
        start_menu_entry_descr_show(entry_descr, 2);
    }

no_descr_update:
    if (super.buttons_new_remapped & KEYPAD_A) {
        music_play(5);
        if (start_menu_is_selected_entry_valid()) {
            u8 selected_entry = start_menu_item_indices[start_menu_cursor];
            start_menu_active_context = sm_entries[selected_entry].action;
            sub_806F394();
        }
        return 0;
    }

    if (super.buttons_new_remapped & (KEYPAD_B | KEYPAD_START) == 0)
        return 0;

    sub_806EF18();
    sub_80F7998();
    sub_806FEA0();
    return 1;
}

// 0806FEC8
void append_byte(u8 *buffer, u8 *cursor, u8 elem) {
    buffer[(*cursor)++] = elem;
}
