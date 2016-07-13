#include "overworld_navigation.h"
#include "uncategorized.h"

u16 ptx1[] = { 0x1101, 0x1101, 0x1101, 0x1101, 0x1111, 0x1111, 0x1111, 0x1111 };
u16 ptx2[] = { 0x1116, 0x1116, 0x1116, 0x1116 };

struct bla { // Rename?
    u32 tag;
    u16 *ptx;
};

struct bla dp06_1[] = {
    {0x1100, ptx1},
    {0x1110, ptx1},
    {0x1116, ptx2},
    {0x11FF, 0}
};

// 0835B934
u16 npc_pal_second_slot_by_first_slot[] = {
    1, 1, 6, 7, 8, 9, 6, 7, 8, 9, 0xB, 0xB, 0, 0, 0, 0
};

// 805F5E8
void sub_805F5E8(u16 tag, u8 pal_slot) {
    // tag = 0x1103
    // pal_slot = 0..15
    pal_patch_for_npc (tag, pal_slot);
    u16 second_slot = npc_pal_second_slot_by_first_slot[pal_slot];
    for (u32 c=0; dp06_1[c].tag != 0x11FF; c++) {
        if (dp06_1[c].tag == tag) {
            pal_patch_for_npc (dp06_1[c].ptx[npc_palette_mode], second_slot);
            return;
        }
    }
}

struct transfer {
    void *loc_a;
    void *loc_b;
    union {
        struct {
            u16  size;
            u16  mode;
        };
        u32 rest;
    };
};

// 030000C8
struct transfer dma3_transfer_queue[0x80];

extern u32 dma3_transfer_queue_edit_lock;
extern u32 byte_30008C9;

// 08000BFC
void dma3_transfer_queue_clear() {
    dma3_transfer_queue_edit_lock = 1;
    byte_30008C9 = 0;
    u32 id = 0x80;
    while (id--) {
        dma3_transfer_queue[id].loc_a = 0;
        dma3_transfer_queue[id].loc_b = 0;
        dma3_transfer_queue[id].rest = 0;
    }
    dma3_transfer_queue_edit_lock = 0;
}

// 08000EAC
u16 dma3_queue_add_transfer(void *loc_a, void *loc_b, u16 size, u8 zz) {

    // modified for readability

    dma3_transfer_queue_edit_lock = 1;

    u32 offset = byte_30008C9;
    for (u32 id=0; id<0x80; id++) {
        struct transfer *tr = &dma3_transfer_queue[(id+offset)%0x80];
        if (tr->size) continue;
        tr->loc_a = loc_a;
        tr->loc_b = loc_b;
        tr->size = size;
        tr->mode = (zz==1 ? 1 : 3);
        dma3_transfer_queue_edit_lock = 0;
        return (id+offset)%0x80;
    }
    dma3_transfer_queue_edit_lock = 0;
    return -1;
}

// 08000FE8
u32 sub_8000FE8(u16 r) {
    if (r == -1) {
        if (dma3_transfer_queue[0].size)
            return -1;
    } else for (u32 id=0; id<0x80; id++) {
        if (dma3_transfer_queue[id].size)
            return -1;
    }
    return 0;
}
