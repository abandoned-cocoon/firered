#pragma once
#include "types.h"

enum interrupt_bits {
    i_vblank = 0x1,
    i_hblank = 0x2,
    i_vcount = 0x4,
    i_timer0 = 0x8,
    i_timer1 = 0x10,
    i_timer2 = 0x20,
    i_timer3 = 0x40,
    i_serial = 0x80,
    i_dma0 = 0x100,
    i_dma1 = 0x200,
    i_dma2 = 0x400,
    i_dma3 = 0x800,
    i_keypad = 0x1000,
    i_gamepak = 0x2000,
    i_unused0 = 0x4000,
    i_unused1 = 0x8000,
};

enum keypad {
    KEYPAD_A = 0x1,
    KEYPAD_B = 0x2,
    KEYPAD_SELECT = 0x4,
    KEYPAD_START = 0x8,
    KEYPAD_RIGHT = 0x10,
    KEYPAD_LEFT = 0x20,
    KEYPAD_UP = 0x40,
    KEYPAD_DOWN = 0x80,
    KEYPAD_SHOULDER_RIGHT = 0x100,
    KEYPAD_SHOULDER_LEFT = 0x200,
    KEYPAD_ANYDIR = 0x3C,

    // TODO: Cleanup
    BUTTON_A = 0x1,
    BUTTON_B = 0x2,
    BUTTON_SELECT = 0x4,
    BUTTON_START = 0x8,
    BUTTON_RIGHT = 0x10,
    BUTTON_LEFT = 0x20,
    BUTTON_UP = 0x40,
    BUTTON_DOWN = 0x80,
    BUTTON_R = 0x100,
    BUTTON_L = 0x200
};

struct sprite {
    u8 y;
    u8 flags1;
    u8 x;
    u8 msb_of_x_and_flags2;
    u16 attr2;
    u16 rotscaleinfo;
};

struct superstate {
    void (*callback1)(void);
    void (*callback2)(void);
    void (*callback2backup)(void);
    void (*vblank_callback)(void);
    void (*hblank_callback)(void);
    u32 field_14;
    u32 field_18;
    u32 bits_to_wait_for;
    u32 ptr_vblank_counter;
    u32 local_vblank_counter;
    u16 buttons_held;
    u16 buttons_new;
    u16 buttons_held_remapped;
    u16 buttons_new_remapped;
    u16 buttons_new_and_keyrepeat;
    u16 keypad_countdown;
    u32 unused_apparently;
    struct sprite sprites[128];
    u8  multi_purpose_state_tracker;
    u8  gpu_sprites_upload_skip;
};

extern struct superstate super;

void mainloop();
void exec_c1_and_c2();
void load_keys();
void wait_for_superbit_eg_vmatch();
