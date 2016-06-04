#pragma once
#include "types.h"

#define MAX_NPCS 0x10

struct ptr_and_size {
    void *data;
    u32 size;
};
typedef struct ptr_and_size gfxtable_t; // TODO: Review
typedef struct ptr_and_size animtable_t; // TODO: Review

typedef struct { u16 x, y; } coords; // = coords16

struct npc_pos_h {
    u16 x, y;
    u8 height;
};

struct npc_state {
    u8 bitfield1;
        // 0x01: active
        // 0x02: exempt from general anim pausing
        // 0x04: use npc_analyze_ground_{pre,post}
        // 0x08: use npc_analyze_ground_post_2; on grid
        // 0x10: ?
        // 0x20: enables 08067F2C
        // 0x40: refuse pause bit overriding
        // 0x80: ?
    u8 bitfield2;
        // 0x01: anim_delay_countdown backuped because of override
        // 0x02: affects whether npc_Set_direction affects the upper or both nibbles of npc->direction
        // 0x04: pending image anim pausing (set and cleared based on "tile_to" at 08063E80 (read at 080679AC))
        // 0x08: pending image anim unpausing
        // 0x10: ?
        // 0x20: invisible/hidden (controlled through script cmds 0x58 and 0x59)
        // 0x40: offscreen/culled
        //  ^ these two are checked by 08067B0C
        // 0x80: checked t 0805F1B4
    u8 field_2;
        // 0x80: backup of image anim pause flag
    u8 field_3;
        // 0x01: backup of rotscale anim pause flag
        // 0x04: lock obj->y_height_related
    u8 oam_id;
    u8 type_id;
    u8 running_behaviour;
    u8 is_trainer;
    u8 local_id;
    u8 local_mapnr;
    u8 local_mapgroup;
    u8 height; // stored in lower 4 bits
    coords stay_around;
    coords to;
    coords from;
    u8 direction;
    u8 movement_area;
    u8 field_1A;
    u8 oam_id2;
    u8 state;
    u8 sight_distance;
    u8 role_block_to;
    u8 role_block_from;
    u8 field_20;
    u8 field_21;
    u8 field_22;
    u8 field_23;
};

struct npc_type {
    u16 field_0;
    u16 pal_num;
    u16 pal_tag_2;
    u16 field_6;
    coords pos_neg_center;
    u8 pal_slot; // stored in lower 4 bits
    u8 field_D;
    u16 _unused;
    struct obj_t *obj;
    void *field_14;
    animtable_t *animtable1;
    gfxtable_t  *gfxtable;
    animtable_t *animtable2;
};
