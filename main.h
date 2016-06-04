#pragma once
#include "types.h"

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
    void (*callback5_vblank)(void);
    void (*hblank_callback)(void);
    u32 field_14;
    u32 field_18;
    u32 bits_to_wait_for;
    u32 ptr_vblank_counter;
    u32 local_vblank_counter;
    i16 buttons0_held;
    i16 buttons1_new;
    u16 buttons2_held_remapped;
    u16 buttons3_new_remapped;
    i16 buttons4_new_and_keyrepeat;
    i16 keypad_countdown;
    u32 unused_apparently;
    struct sprite sprites[128];
    u8  multi_purpose_state_tracker;
    u8  gpu_sprites_upload_skip;
};

extern struct superstate super;
