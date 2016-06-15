#include "npc.h"
#include "object.h"

struct task_args_0805CD0C {
    enum mode_0805CD0C {
        ZERO = 0,
        ONE = 1,
        TWO = 2
    } mode;
    u8 npc_id;
    u8 direction;
};

// 0835B8A0
bool (off_835B8A0[])(struct task_t*, struct npc_state*, struct npc_state*) = {
    &sub_805CD64_mode_0,
    &sub_805CD84_mode_1,
    &sub_805CE20_mode_2
};

// 0805DF60
u8 npc_id_by_local_id(u8 local_id, u8 mapnr, u8 mapgroup) {
    if (local_id == 0xFF)
        return npc_id_by_local_id_ignore_map(local_id);
    else
        return npc_id_by_local_id_and_map(local_id, mapnr, mapgroup);
}

// 0805DF84
bool npc_id_by_local_id_and_map_ret_success(u8 local_id, u8 mapnr, u8 mapgroup, u8 *result) {
    return (*result=npc_id_by_local_id(local_id, mapnr, mapgroup)) == MAX_NPCS;
}

// 0805DFF4
u8 npc_id_by_local_id_and_map(u8 local_id, u8 mapnr, u8 mapgroup) {
    u8 i=0;
    while (i<MAX_NPCS) {
        if (npc_states[i].bits & 1 && \
            npc_states[i].local_id       == local_id && \
            npc_states[i].local_mapnr    == local_mapnr && \
            npc_states[i].local_mapgroup == local_mapgroup) break;
        i++;
    }
    return i;
}

// 0805E080
u8 rom_npc_to_npc_state(struct rom_npc *rnpc, u8, u8) {
    // TODO
}

// 0805E044
u8 npc_id_by_local_id_ignore_map(u8 local_id) {
    u8 i=0;
    while (i<MAX_NPCS) {
        if (npc_states[i].bitfield1 & NPC_BIT_ACTIVE && \
            npc_states[i].local_id == local_id) break;
        i++;
    }
    return i;
}

// 0805E4B4
void npc_hide(struct npc_state *npc) {
    npc->bits &= ~2; // clear bit 2
    sub_0805E510(npc); // TODO
}

// 0805E4C8
void hide_sprite(u8 local_id, u8 mapnr, u8 mapgroup) {
    // TODO: Find better name. Check correctness of this transcription.
    u8 npc_id;
    if (!npc_id_by_local_id_and_map_ret_success(local_id, mapnr, mapgroup, &npc_id)) {
        flag_set(trainerid_by_npc_id(npc_id));
        npc_hide(&npc_states[npc_id]);
    }
}

// 0805E590
void *npc_spawn_with_provided_template(u8, void*, u8, u8, short, short) {
    // TODO
    return 0;
}

// 0805E8E8
void npc_to_template(u8 npc_type_id, void *objcallback, struct proto_t *p, u32 *npc_type_14) {
    struct npc_type *nt;
    nt = npc_get_type(npc_type_id);

    p->tile_tag       = nt->tile_tag;
    p->pal_num        = nt->pal_num;
    p->oam            = nt->oam;
    p->image_anims    = nt->image_anims;
    p->gfx_table      = nt->gfx_table;
    p->rotscale_anims = nt->rotscale_anims;

    if (script_env_2_context_is_eq_x0() || sub_8112CAC() != 1) {
        p->callback = objcallback;
    } else {
        p->callback = &objc_npc_alternative;
    }

    *npc_type_14 = nt->field_14;
}

// 0805F218
void npc_turn(struct npc_state *npc, u8 direction) {
    npc_set_direction(npc, direction);

    if (npc->bitfield2 & (npc_invisble >> 8))
        return;

    obj_anim_image_start(o, npc_direction_to_obj_anim_image_number(npc->direction & 4));
    obj_anim_image_seek(o, 0);
}

// 0805F268
void npc_turn_by_local_id_and_map(u16 local_id, u8 map, u8 bank, u8 direction) {
    u8 npc_id;
    npc_id_by_local_id_and_map_ret_success(local_id, map, bank, &npc_id);
    npc_turn(&npc_states[npc_id], direction);
}

// 0805F2C8
struct npc_type *npc_get_type(u8 type_id) {
    if (type_id >= 0xF0)
        type_id = var_load_x4010_plus(type_id-0xF0);
    if (type_id >= 152)
        type_id = 10;
    return npc_types[type_id];
}

// 0805F574
void npc_pal_patch_range(u16 *ptx, u8 i, u8 j) {
    while (i<j)
        pal_patch_for_npc(*ptx++, i++);
}

// 0805F700
void npc_coords_shift(struct npc_state *n, u16 x, u16 y) {
    n->from = n->to;
    n->to.x = x;
    n->to.y = y;
}

// 0805F710
void npc_coords_set(struct npc_state *n, u16 x, u16 y) {
    n->from.x = x;
    n->from.y = y;
    n->to.x = x;
    n->to.y = y;
}

// 0805F818
void npc_coords_shift_still(struct npc_state *n) {
    n->from = n->to;
}

// 0805F82C
void npcs_rebase() {
    if (!translate_info.active)
        return;

    i16 dx = translate_info.delta.x;
    i16 dy = translate_info.delta.y;

    for (u8 i=0; i<MAX_NPCS; i++) {
        struct npc_state *npc = npc_states + i;
        if (!(npc->bits & 1))
            continue;

        npc->stay_around.x -= dx;
        npc->stay_around.y -= dy;
        npc->to.x -= dx;
        npc->to.y -= dy;
        npc->from.x -= dx;
        npc->from.y -= dy;
    }
}

// 0805F8FC
bool npc_does_height_match(struct npc_state *npc, u8 height) {
    u8 nh = npc->height & 0xF;
    if (nh == 0) return true;
    if (height == 0) return true;
    if (nh == height) return true;
    return false;
}

// 0805F218
void npc_set_direction(struct npc_state *npc, u8 direction) {
    npc->unknown = npc->direction.low;
    if (npc->bitfield2 & (npc_use_upper_direction_nibble >> 8) == 0)
        npc->direction.low = direction;
    npc->direction.high = direction;
}

// 0805FC5C
u16 trainerid_by_local_id_and_map(u8 local_id, u8 mapnr, u8 mapgroup) {
    struct rom_npc *rnpc = (rom_npc_by_nr_and_map(local_id, mapnr, mapgroup));
    return rnpc->trainerid;
}

// 0805FC74
u16 trainerid_by_npc_id(u8 npc_id) {
    struct npc_state *npc = &npc_states[npc_id];
    return trainerid_by_local_id_and_map(npc->local_id, npc->local_mapnr, npc->local_mapgroup);
}

// 0805FD5C
struct rom_npc *rom_npc_by_nr_and_map(u8 local_id, u8 mapnr, u8 mapgroup) {
    // To be written.
}

// 080642C8
u8 npc_reciprocate_look(struct npc_state *npc, u8 direction) {
    u8 state = d2s_look1(direction_reversed(direction));
    return npc_set_state_2(npc, state);
}

#define AN(name) bool an_##name(struct npc_state *npc, struct obj *obj)
typedef bool (*anptr)(struct npc_state *npc, struct obj *obj);

AN(loop) {                                                    return true; } // 08067930
AN(stop) { obj.anim_delay_countdown |= OBJ_ANIM_IMAGE_PAUSED; return true; } // 08067934


AN(look1_dn) { an_look_any(1); return true; } // 08064638
AN(look1_up) { an_look_any(2); return true; } // 08064648
AN(look1_lf) { an_look_any(3); return true; } // 08064658
AN(look1_rt) { an_look_any(4); return true; } // 08064668

// TODO: Don't reassing the same name. Split 'an_' prefix in two prefixes?

anptr an_look1_dn[] = { an_look1_dn, an_stop }; // 083A6864
anptr an_look1_up[] = { an_look1_up, an_stop }; // 083A686C
anptr an_look1_lf[] = { an_look1_lf, an_stop }; // 083A6874
anptr an_look1_rt[] = { an_look1_rt, an_stop }; // 083A6874


AN(look2_dn) { /* ... */ } // 080655D4
AN(look2_up) { /* ... */ }
AN(look2_lf) { /* ... */ }
AN(look2_rt) { /* ... */ }

anptr an_look2_dn[] = { an_look2_dn, an_loop }; // 083A6A30
anptr an_look2_up[] = { an_look2_up, an_loop }; // 083A6A38
anptr an_look2_lf[] = { an_look2_lf, an_loop }; // 083A6A40
anptr an_look2_rt[] = { an_look2_rt, an_loop }; // 083A6A48


AN(walk_dn_1) { an_walk_any_1(npc, obj, 1); return an_walk_dn_2(npc, obj); }
AN(walk_up_1) { an_walk_any_1(npc, obj, 2); return an_walk_up_2(npc, obj); }
AN(walk_lf_1) { an_walk_any_1(npc, obj, 3); return an_walk_lf_2(npc, obj); }
AN(walk_rt_1) { an_walk_any_1(npc, obj, 4); return an_walk_rt_2(npc, obj); }

AN(walk_dn_2) { return an_walk_any_2(npc, obj) && (obj->priv2 = 1, true); }
AN(walk_up_2) { return an_walk_any_2(npc, obj) && (obj->priv2 = 2, true); }
AN(walk_lf_2) { return an_walk_any_2(npc, obj) && (obj->priv2 = 3, true); }
AN(walk_rt_2) { return an_walk_any_2(npc, obj) && (obj->priv2 = 4, true); }

anptr an_walk_dn[] = { an_walk_dn_1, an_walk_dn_2, an_stop }; // 083A68C8
anptr an_walk_up[] = { an_walk_up_1, an_walk_up_2, an_stop }; // 083A68D4
anptr an_walk_lf[] = { an_walk_lf_1, an_walk_lf_2, an_stop }; // 083A68E0
anptr an_walk_rt[] = { an_walk_rt_1, an_walk_rt_2, an_stop }; // 083A68EC


AN(run_dn_1) { an_run_any(npc, obj, 1, 1); return an_run_dn_2(npc, obj); } // 080652CC
AN(run_up_1) { an_run_any(npc, obj, 2, 1); return an_run_up_2(npc, obj); } // 080652EC
AN(run_lf_1) { an_run_any(npc, obj, 3, 1); return an_run_lf_2(npc, obj); } // 080652FC
AN(run_rt_1) { an_run_any(npc, obj, 4, 1); return an_run_rt_2(npc, obj); } // 0806530C

AN(run_dn_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 080652EC
AN(run_up_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 0806530C
AN(run_lf_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 0806532C
AN(run_rt_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 0806534C

anptr an_run_dn[] = { an_run_dn_1, an_run_dn_2, an_stop }; // 083A69D0
anptr an_run_up[] = { an_run_up_1, an_run_up_2, an_stop }; // 083A69DC
anptr an_run_lf[] = { an_run_lf_1, an_run_lf_2, an_stop }; // 083A69E8
anptr an_run_rt[] = { an_run_rt_1, an_run_rt_2, an_stop }; // 083A69F4

#define Q(n) n##_dn, n##_up, n##_lf, n##_rt

// temporary forward decls
extern anptr off_83A6964[];
extern anptr off_83A6970[];
extern anptr off_83A697C[];
extern anptr off_83A6988[];
extern anptr off_83A6994[];
extern anptr off_83A69A0[];
extern anptr off_83A69AC[];
extern anptr off_83A69B8[];
extern anptr off_83A69C4[];
extern anptr off_83A6A50[];
extern anptr off_83A6A5C[];
extern anptr off_83A6A68[];
extern anptr off_83A6A74[];
extern anptr off_83A6A80[];
extern anptr off_83A6A8C[];
extern anptr off_83A6A98[];
extern anptr off_83A6AA4[];
extern anptr off_83A6AB0[];
extern anptr off_83A6ABC[];
extern anptr off_83A6AC8[];
extern anptr off_83A6AD4[];
extern anptr off_83A6AE0[];
extern anptr off_83A6AEC[];
extern anptr off_83A6AF8[];
extern anptr off_83A6B04[];
extern anptr off_83A6B10[];
extern anptr off_83A6B1C[];
extern anptr off_83A6B28[];
extern anptr off_83A6B34[];
extern anptr off_83A6B40[];
extern anptr off_83A6B4C[];
extern anptr off_83A6B58[];
extern anptr off_83A6B64[];
extern anptr off_83A6B70[];
extern anptr off_83A6B7C[];
extern anptr off_83A6B88[];
extern anptr off_83A6B94[];
extern anptr off_83A6BA0[];
extern anptr off_83A6BAC[];
extern anptr off_83A6BB8[];
extern anptr off_83A6BC4[];
extern anptr off_83A6BD0[];
extern anptr off_83A6BDC[];
extern anptr off_83A6BE8[];
extern anptr off_83A6BF4[];
extern anptr off_83A6C00[];
extern anptr off_83A6C0C[];
extern anptr off_83A6C18[];
extern anptr off_83A6C24[];
extern anptr off_83A6C30[];
extern anptr off_83A6C6C[];
extern anptr off_83A6C74[];
extern anptr off_83A6C7C[];
extern anptr off_83A6C84[];
extern anptr off_83A6C8C[];
extern anptr off_83A6C98[];
extern anptr off_83A6CA4[];
extern anptr off_83A6CB0[];
extern anptr off_83A6CBC[];
extern anptr off_83A6CC8[];
extern anptr off_83A6CD4[];
extern anptr off_83A6CE0[];
extern anptr off_83A6CEC[];
extern anptr off_83A6CF8[];
extern anptr off_83A6D04[];
extern anptr off_83A6D10[];
extern anptr off_83A6D1C[];
extern anptr off_83A6D24[];
extern anptr off_83A6D30[];
extern anptr off_83A6D38[];
extern anptr off_83A6D40[];
extern anptr off_83A6D48[];
extern anptr off_83A6D50[];
extern anptr off_83A6D58[];
extern anptr off_83A6D60[];
extern anptr off_83A6D68[];
extern anptr off_83A6D70[];
extern anptr off_83A6D78[];
extern anptr off_83A6D80[];
extern anptr off_83A6D88[];
extern anptr off_83A6D94[];
extern anptr off_83A6DA4[];
extern anptr off_83A6DB4[];
extern anptr off_83A6DBC[];
extern anptr off_83A6DC4[];
extern anptr off_83A6DCC[];
extern anptr off_83A6DD4[];
extern anptr off_83A6DE0[];
extern anptr off_83A6DEC[];
extern anptr off_83A6DF4[];
extern anptr off_83A6DFC[];
extern anptr off_83A6E04[];
extern anptr off_83A6E0C[];
extern anptr off_83A6E18[];
extern anptr off_83A6E24[];
extern anptr off_83A6E30[];
extern anptr off_83A6E3C[];
extern anptr off_83A6E48[];
extern anptr off_83A6E54[];
extern anptr off_83A6E60[];
extern anptr off_83A6E9C[];
extern anptr off_83A6EA8[];
extern anptr off_83A6EB4[];
extern anptr off_83A6EC0[];
extern anptr off_83A6ECC[];
extern anptr off_83A6ED8[];
extern anptr off_83A6EE4[];
extern anptr off_83A6EF0[];
extern anptr off_83A6EFC[];
extern anptr off_83A6F08[];
extern anptr off_83A6F14[];
extern anptr off_83A6F20[];
extern anptr off_83A6F2C[];
extern anptr off_83A6F38[];
extern anptr off_83A6F44[];
extern anptr off_83A6F50[];
extern anptr off_83A6F5C[];
extern anptr off_83A6F68[];
extern anptr off_83A6F74[];
extern anptr off_83A6F80[];
extern anptr off_83A6F8C[];
extern anptr off_83A6F98[];
extern anptr off_83A6FA4[];
extern anptr off_83A6FB0[];
extern anptr off_83A6FBC[];
extern anptr off_83A6FC8[];
extern anptr off_83A6FD4[];
extern anptr off_83A6FE0[];
extern anptr off_83A6FEC[];
extern anptr off_83A6FF4[];
extern anptr off_83A6FFC[];
extern anptr off_83A6898[];
extern anptr off_83A68A4[];
extern anptr off_83A68B0[];
extern anptr off_83A68BC[];
extern anptr off_83A7004[];
extern anptr off_83A6A00[];
extern anptr off_83A6A0C[];
extern anptr off_83A6A18[];
extern anptr off_83A6A24[];
extern anptr off_83A700C[];
extern anptr off_83A7018[];
extern anptr off_83A6C3C[];
extern anptr off_83A6C48[];
extern anptr off_83A6C54[];
extern anptr off_83A6C60[];
// end temporary forward decls

// 083A65BC
anptr (*an_table[]) = {
    Q(an_look1),  // 0x00
    Q(an_look2),  // 0x04
    Q(an_walk),   // 0x08
    Q(an_go),     // 0x0C; can't remember the difference between "walk" and "go"
    Q(an_pulse),  // 0x10
    off_83A6964, off_83A6970, off_83A697C, off_83A6988, // 0x14
    off_83A6994, off_83A69A0, off_83A69AC, off_83A69B8, // 0x18
    off_83A69C4,
        Q(an_run),
                 off_83A6A50, off_83A6A5C, off_83A6A68, // 0x20
    off_83A6A74, off_83A6A80, off_83A6A8C, off_83A6A98, // 0x24
    off_83A6AA4, off_83A6AB0, off_83A6ABC, off_83A6AC8, // 0x28
    off_83A6AD4, off_83A6AE0, off_83A6AEC, off_83A6AF8, // 0x2C
    off_83A6B04, off_83A6B10, off_83A6B1C, off_83A6B28, // 0x30
    off_83A6B34, off_83A6B40, off_83A6B4C, off_83A6B58, // 0x34
    off_83A6B64, off_83A6B70, off_83A6B7C, off_83A6B88, // 0x38
    off_83A6B94, off_83A6BA0, off_83A6BAC, off_83A6BB8, // 0x3C
    off_83A6BC4, off_83A6BD0, off_83A6BDC, off_83A6BE8, // 0x40
    off_83A6BF4, off_83A6C00, off_83A6C0C, off_83A6C18, // 0x44
    off_83A6C24, off_83A6C30, off_83A6C6C, off_83A6C74, // 0x48
    off_83A6C7C, off_83A6C84, off_83A6C8C, off_83A6C98, // 0x4C
    off_83A6CA4, off_83A6CB0, off_83A6CBC, off_83A6CC8, // 0x50
    off_83A6CD4, off_83A6CE0, off_83A6CEC, off_83A6CF8, // 0x54
    off_83A6D04, off_83A6D10, off_83A6D1C, off_83A6D24, // 0x58
    off_83A6D30, off_83A6D38, off_83A6D40, off_83A6D48, // 0x5C
    off_83A6D50, off_83A6D58, off_83A6D60, off_83A6D68, // 0x60
    off_83A6D70, off_83A6D78, off_83A6D80, off_83A6D88, // 0x64
    off_83A6D94, off_83A6DA4, off_83A6DB4, off_83A6DBC, // 0x68
    off_83A6DC4, off_83A6DCC, off_83A6DD4, off_83A6DE0, // 0x6C
    off_83A6DEC, off_83A6DF4, off_83A6DFC, off_83A6E04, // 0x70
    off_83A6E0C, off_83A6E18, off_83A6E24, off_83A6E30, // 0x74
    off_83A6E3C, off_83A6E48, off_83A6E54, off_83A6E60, // 0x78
    off_83A6E9C, off_83A6EA8, off_83A6EB4, off_83A6EC0, // 0x7C
    off_83A6ECC, off_83A6ED8, off_83A6EE4, off_83A6EF0, // 0x80
    off_83A6EFC, off_83A6F08, off_83A6F14, off_83A6F20, // 0x84
    off_83A6F2C, off_83A6F38, off_83A6F44, off_83A6F50, // 0x88
    off_83A6F5C, off_83A6F68, off_83A6F74, off_83A6F80, // 0x8C
    off_83A6F8C, off_83A6F98, off_83A6FA4, off_83A6FB0, // 0x90
    off_83A6FBC, off_83A6FC8, off_83A6FD4, off_83A6FE0, // 0x94
    off_83A6FEC, off_83A6FF4, off_83A6FFC, off_83A6898, // 0x98
    off_83A68A4, off_83A68B0, off_83A68BC, off_83A7004, // 0x9C
    off_83A6A00, off_83A6A0C, off_83A6A18, off_83A6A24, // 0xA0
    off_83A700C, off_83A7018, off_83A6C3C, off_83A6C48, // 0xA4 
    off_83A6C54, off_83A6C60                            // 0xA8
};

// 08064788
bool npc_obj_ministep_stop_on_arrival(struct npc_state *npc, struct obj *o) {
    if (!obj_npc_ministep(o))
        // stepping continues
        return false;
    // step done, npc is on it's target position
    npc->from.x = npc->to.x;
    npc->from.y = npc->to.y;
    npc->bitfield |= NPC_BIT_ONGRID;
    obj->anim_delay_countdown |= OBJ_ANIM_PAUSE;
    return true;
}

// 080653CC
bool npc_ministep(struct npc_state *npc, struct obj *o) {
    if (!obj_npc_ministep(o))
        // stepping continues
        return false;
    // step done, npc is on it's target position
    npc->from.x = npc->to.x;
    npc->from.y = npc->to.y;
    npc->bitfield |= 0x8; // flag for 'on grid' maybe?
    return true;
}

// 083A64C8
coords directions_i16[] {
    { 0,  0},
    { 0,  1},
    { 0, -1},
    {-1,  0},
    { 1,  0},
    {-1,  1},
    { 1,  1},
    {-1, -1},
    { 1, -1}
};

// 08068A8C
void step1(struct obj *o, u8 d) {
    o->pos_1.x += directions_i16[d].x;
    o->pos_1.y += directions_i16[d].y;
}

// 08068AAC
void step2(struct obj *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 2;
    o->pos_1.y += directions_i16[d].y * 2;
}

// 08068AD0
void step3(struct obj *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 3;
    o->pos_1.y += directions_i16[d].y * 3;
}

// 08068AF8
void step4(struct obj *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 4;
    o->pos_1.y += directions_i16[d].y * 4;
}

// 08068B1C
void step8(struct obj *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 8;
    o->pos_1.y += directions_i16[d].y * 8;
}

// 08068B40
void obj_npc_ministep_reset(struct obj *o, u16 speed, u16 phase) {
    o->priv3 = 0;
    o->priv4 = speed;
    o->priv5 = phase;
}

// 083A710C
void (*stepspeed1[])(struct obj*, u8) = {
    step1, step1, step1, step1,
    step1, step1, step1, step1,
    step1, step1, step1, step1,
    step1, step1, step1, step1
};

// 083A714C
void (*stepspeed2[])(struct obj*, u8) = {
    step2, step2, step2, step2,
    step2, step2, step2, step2
};

// 083A7184
void (*stepspeed4[])(struct obj*, u8) = {
    step4, step4, step4, step4
};

// 083A7194
void (*stepspeed5[])(struct obj*, u8) = {
    step8, step8
};

// 083A719C
void (**stepspeeds[])(struct obj *, u8) = {
    stepspeed1,
    stepspeed2,
    stepspeed3,
    stepspeed4,
    stepspeed5
};

// 083A71B0
u16 stepspeed_seq_length[] = {
    16, 8, 6, 4, 2
};

// 08068B54
bool obj_npc_ministep(struct obj *o) {
    u8   z =  o->priv3; // direction
    u16  s =  o->priv4; // speed
    u16 *i = &o->priv5; // phase
    u16  l = stepspeed_seq_length[s];
    if (*i >= l) return false;
    stepspeeds[s][*i](o, z);
    if ((*i)++ >= l) return true;
    return false;
}

// 08068BBC
void obj_npc_ministep_set_direction(struct obj *o, u16 direction) {
    o->priv3 = direction;
    o->priv4 = 0;
    o->priv5 = 0;
}

struct npc_image_anim_looping_info {
    struct image_seq *animtable;
    struct loopingpoint { u8 trigger; u8 target; } l1, l2;
};

// 08063554
void npc_apply_anim_looping(struct npc_state *npc, struct obj *obj, u8 anim_number) {
    if (npc.field_1 & 0x8) return;
    obj->anim_number = anim_number;
    struct npc_image_anim_looping_info *tp = animtable_get_tp(obj->anim_table);
    if (tp) {
             if (obj->anim_frame == tp->l1.trigger) obj->anim_frame = tp->l1.trigger;
        else if (obj->anim_frame == tp->l2.trigger) obj->anim_frame = tp->l2.trigger;
    }
    obj_anim_image_seek(obj, obj->anim_frame);
}

// 0806DE28
void npc_hide_and_trainer_flag_clear_on_role_x66_at_pos(struct npc_state *npc) {
    u8 role = cur_mapdata_block_role_at(npc->to.x, npc->to.y);
    if (role != 0x66) return;
    sound_play(0x25);

    npc_hide_by_local_id_and_map(
        npc->local_id,
        sav1->location.map,
        sav1->location.bank);

    u16 flag = rom_npc_by_local_id_and_map_get_trainer_flag(
        npc->local_id,
        sav1->location.map,
        sav1->location.bank);

    flag_clear(flag);
}

// 0826D2D8
i32 directions_i32[9*2] = {
     0,  0,
     0,  1,
     0, -1,
    -1,  0,
     1,  0,
    -1,  1,
     1,  1,
    -1, -1,
     1, -1
};
