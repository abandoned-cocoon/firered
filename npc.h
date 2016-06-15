#pragma once
#include "types.h"

#define MAX_NPCS 0x10

struct proto_t;

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


enum npcbits {
    // enum or #defines?
    npc_active = 0x1,
#define NPC_BIT_ACTIVE 0x01
    npc_nopause_1 = 0x2,
    npc_ground_analysis_1_2 = 0x4,
    npc_ground_analysis_3__on_grid = 0x8,
    npc_4 = 0x10,
    npc_enable_08067F2C = 0x20,
    npc_nopause_2__noclear_an = 0x40,
    npc_has_touched_54_58_tile = 0x80,
    npc_pause_flags_are_stashed = 0x100,
    npc_use_upper_direction_nibble = 0x200,
#define NPC_BIT_USE_UPPER_DIRECTION_NIBBLE 0x200
    npc_block_justifies_pause = 0x400,
    npc_unpause_pending = 0x800,
    npc_12 = 0x1000,
    npc_invisble = 0x2000,
    npc_offscreen = 0x4000,
    npc_15 = 0x8000,
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
    u8 objid_surfing;
    u8 objid_1B;
    u8 state; // = an_index__movement_behaviour
    u8 sight_distance;
    u8 role_block_to;
    u8 role_block_from;
    u8 field_20;
    u8 field_21_decrement_on_step;
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
    struct obj *obj;
    void *field_14;
    animtable_t *animtable1;
    gfxtable_t  *gfxtable;
    animtable_t *animtable2;
};

// TODO: Move away
struct rom_npc {
    u8  nr;
    u8  type_nr;
    u8  rival;
    u8  field_3;
    u16 x;
    u16 y;
    u8  height;
    u8  running_behaviour;
    u8  movement_area;
    u8  field_B;
    u8  trainer_or_mapnumber;
    u8  field_D;
    u16 sight_distance_or_mapbank;
    void *script;
    u16 id_in_script;
    u16 field_16;
};

extern struct npc_state npc_states[16];

u8 npc_id_by_local_id(u8 local_id, u8 mapnr, u8 mapgroup);
bool npc_id_by_local_id_and_map_ret_success(u8 local_id, u8 mapnr, u8 mapgroup, u8 *result);
u8 npc_id_by_local_id_and_map(u8 local_id, u8 mapnr, u8 mapgroup);
u8 rom_npc_to_npc_state(struct rom_npc *rnpc, u8, u8);
u8 npc_id_by_local_id_ignore_map(u8 local_id);
void npc_hide(struct npc_state *npc);
void hide_sprite(u8 local_id, u8 mapnr, u8 mapgroup);
void *npc_spawn_with_provided_template(u8, void*, u8, u8, short, short);
void npc_to_template(u8 npc_type_id, void *objcallback, struct proto_t *p, u32 *npc_type_14);
void npc_turn(struct npc_state *npc, u8 direction);
void npc_turn_by_local_id_and_map(u16 local_id, u8 map, u8 bank, u8 direction);
struct npc_type *npc_get_type(u8 type_id);
void npc_pal_patch_range(u16 *ptx, u8 i, u8 j);
void npc_coords_shift(struct npc_state *n, u16 x, u16 y);
void npc_coords_set(struct npc_state *n, u16 x, u16 y);
void npc_coords_shift_still(struct npc_state *n);
void npcs_rebase();
bool npc_does_height_match(struct npc_state *npc, u8 height);
void npc_set_direction(struct npc_state *npc, u8 direction);
u16 trainerid_by_local_id_and_map(u8 local_id, u8 mapnr, u8 mapgroup);
u16 trainerid_by_npc_id(u8 npc_id);
struct rom_npc *rom_npc_by_nr_and_map(u8 local_id, u8 mapnr, u8 mapgroup);
u8 npc_reciprocate_look(struct npc_state *npc, u8 direction);
bool npc_obj_ministep_stop_on_arrival(struct npc_state *npc, struct obj *o);
bool npc_ministep(struct npc_state *npc, struct obj *o);
void step1(struct obj *o, u8 d);
void step2(struct obj *o, u8 d);
void step3(struct obj *o, u8 d);
void step4(struct obj *o, u8 d);
void step8(struct obj *o, u8 d);
void obj_npc_ministep_reset(struct obj *o, u16 speed, u16 phase);
bool obj_npc_ministep(struct obj *o);
void obj_npc_ministep_set_direction(struct obj *o, u16 direction);
void npc_apply_anim_looping(struct npc_state *npc, struct obj *obj, u8 anim_number);
void npc_hide_and_trainer_flag_clear_on_role_x66_at_pos(struct npc_state *npc);
