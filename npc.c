#define MAX_NPCS 0x10

struct ptr_and_size {
    void *data;
    u32 size;
};
typedef struct ptr_and_size *gfxtable_t;

typedef struct { u16 x, y; } coords;

struct npc_pos_h {
    u16 x, y;
    u8 height;
};

struct npc_state {
    u8 bitfield1;
    u8 bitfield2;
    u8 field_2;
    u8 field_3;
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
        if (npc_states[i].bits & 1 && \
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
void *npc_spawn_with_provided_template(byte, void*, byte, byte, short, short) {
    // TODO
    return 0;
}

// 0805E8E8
void npc_to_template(u8 npc_type_id, void *objcallback, struct proto_t *p, u32 *npc_type_14) {
    struct npc_type *nt;
    nt = npc_get_type(npc_type_id);

    p->tiles_tag      = nt->tiles_tag;
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
void npc_coord_step(struct npc_state *n, u16 x, u16 y) {
    n->from.x = n->to.x;
    n->from.y = n->to.y;
    n->to.x = x;
    n->to.y = y;
}

// 0805F8FC
bool npc_does_height_match(struct npc_state *npc, u8 height) {
    u8 nh = npc->height & 0xF;
    if (nh == 0) return true;
    if (height == 0) return true;
    if (nh == height) return true;
    return false;
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

#define AN(name) bool an_##name(struct npc_state *npc, struct obj *obj)
typedef bool (*anptr)(struct npc_state *npc, struct obj *obj);

AN(xxx0) {                        return true; } // 08067930
AN(xxx1) { obj.field_2C |= 0x40; return true; } // 08067934

AN(look1_dn) { an_look_any(1); return true; } // 08064638
AN(look1_up) { an_look_any(2); return true; } // 08064648
AN(look1_lf) { an_look_any(3); return true; } // 08064658
AN(look1_rt) { an_look_any(4); return true; } // 08064668

anptr an_look1_dn[] = { an_look1_dn, an_xxx1 }; // 083A6864
anptr an_look1_up[] = { an_look1_up, an_xxx1 }; // 083A686C
anptr an_look1_lf[] = { an_look1_lf, an_xxx1 }; // 083A6874
anptr an_look1_rt[] = { an_look1_rt, an_xxx1 }; // 083A6874

AN(look2_dn) { /* ... */ } // 080655D4
AN(look2_up) { /* ... */ }
AN(look2_lf) { /* ... */ }
AN(look2_rt) { /* ... */ }

anptr an_look2_dn[] = { an_look2_dn, an_xxx0 }; // 083A6A30
anptr an_look2_up[] = { an_look2_up, an_xxx0 }; // 083A6A38
anptr an_look2_lf[] = { an_look2_lf, an_xxx0 }; // 083A6A40
anptr an_look2_rt[] = { an_look2_rt, an_xxx0 }; // 083A6A48

anptr an_walk_dn[] = { an_walk_dn_1, an_walk_dn_2, an_xxx1 }; // 083A68C8
anptr an_walk_up[] = { an_walk_up_1, an_walk_up_2, an_xxx1 }; // 083A68D4
anptr an_walk_lf[] = { an_walk_lf_1, an_walk_lf_2, an_xxx1 }; // 083A68E0
anptr an_walk_rt[] = { an_walk_rt_1, an_walk_rt_2, an_xxx1 }; // 083A68EC

AN(run_dn_1) { an_run_any(npc, obj, 1, 1); return an_run_dn_2(npc, obj); } // 080652CC
AN(run_up_1) { an_run_any(npc, obj, 2, 1); return an_run_up_2(npc, obj); } // 080652EC
AN(run_lf_1) { an_run_any(npc, obj, 3, 1); return an_run_lf_2(npc, obj); } // 080652FC
AN(run_rt_1) { an_run_any(npc, obj, 4, 1); return an_run_rt_2(npc, obj); } // 0806530C

AN(run_dn_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 080652EC
AN(run_up_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 0806530C
AN(run_lf_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 0806532C
AN(run_rt_2) { return npc_obj_ministep_stop_on_arrival(npc, obj) && (obj->priv2 = 2, true); } // 0806534C

anptr an_run_dn[] = { an_run_dn_1, an_run_dn_2, an_xxx1 }; // 083A69D0
anptr an_run_up[] = { an_run_up_1, an_run_up_2, an_xxx1 }; // 083A69DC
anptr an_run_lf[] = { an_run_lf_1, an_run_lf_2, an_xxx1 }; // 083A69E8
anptr an_run_rt[] = { an_run_rt_1, an_run_rt_2, an_xxx1 }; // 083A69F4

#define Q(n) n##_dn, n##_up, n##_lf, n##_rt

// 083A65BC
anptr (*an_table[]) = {
    Q(&an_look1),  // 0x00
    Q(&an_look2),  // 0x04
    Q(&an_walk),   // 0x08
    Q(&an_go),     // 0x0C; can't remember the difference between "walk" and "go"
    Q(&an_pulse),  // 0x10
    &off_83A6964, &off_83A6970, &off_83A697C, &off_83A6988, // 0x14
    &off_83A6994, &off_83A69A0, &off_83A69AC, &off_83A69B8, // 0x18
    &off_83A69C4,
        Q(&an_run),
                  &off_83A6A50, &off_83A6A5C, &off_83A6A68, // 0x20
    &off_83A6A74, &off_83A6A80, &off_83A6A8C, &off_83A6A98, // 0x24
    &off_83A6AA4, &off_83A6AB0, &off_83A6ABC, &off_83A6AC8, // 0x28
    &off_83A6AD4, &off_83A6AE0, &off_83A6AEC, &off_83A6AF8, // 0x2C
    &off_83A6B04, &off_83A6B10, &off_83A6B1C, &off_83A6B28, // 0x30
    &off_83A6B34, &off_83A6B40, &off_83A6B4C, &off_83A6B58, // 0x34
    &off_83A6B64, &off_83A6B70, &off_83A6B7C, &off_83A6B88, // 0x38
    &off_83A6B94, &off_83A6BA0, &off_83A6BAC, &off_83A6BB8, // 0x3C
    &off_83A6BC4, &off_83A6BD0, &off_83A6BDC, &off_83A6BE8, // 0x40
    &off_83A6BF4, &off_83A6C00, &off_83A6C0C, &off_83A6C18, // 0x44
    &off_83A6C24, &off_83A6C30, &off_83A6C6C, &off_83A6C74, // 0x48
    &off_83A6C7C, &off_83A6C84, &off_83A6C8C, &off_83A6C98, // 0x4C
    &off_83A6CA4, &off_83A6CB0, &off_83A6CBC, &off_83A6CC8, // 0x50
    &off_83A6CD4, &off_83A6CE0, &off_83A6CEC, &off_83A6CF8, // 0x54
    &off_83A6D04, &off_83A6D10, &off_83A6D1C, &off_83A6D24, // 0x58
    &off_83A6D30, &off_83A6D38, &off_83A6D40, &off_83A6D48, // 0x5C
    &off_83A6D50, &off_83A6D58, &off_83A6D60, &off_83A6D68, // 0x60
    &off_83A6D70, &off_83A6D78, &off_83A6D80, &off_83A6D88, // 0x64
    &off_83A6D94, &off_83A6DA4, &off_83A6DB4, &off_83A6DBC, // 0x68
    &off_83A6DC4, &off_83A6DCC, &off_83A6DD4, &off_83A6DE0, // 0x6C
    &off_83A6DEC, &off_83A6DF4, &off_83A6DFC, &off_83A6E04, // 0x70
    &off_83A6E0C, &off_83A6E18, &off_83A6E24, &off_83A6E30, // 0x74
    &off_83A6E3C, &off_83A6E48, &off_83A6E54, &off_83A6E60, // 0x78
    &off_83A6E9C, &off_83A6EA8, &off_83A6EB4, &off_83A6EC0, // 0x7C
    &off_83A6ECC, &off_83A6ED8, &off_83A6EE4, &off_83A6EF0, // 0x80
    &off_83A6EFC, &off_83A6F08, &off_83A6F14, &off_83A6F20, // 0x84
    &off_83A6F2C, &off_83A6F38, &off_83A6F44, &off_83A6F50, // 0x88
    &off_83A6F5C, &off_83A6F68, &off_83A6F74, &off_83A6F80, // 0x8C
    &off_83A6F8C, &off_83A6F98, &off_83A6FA4, &off_83A6FB0, // 0x90
    &off_83A6FBC, &off_83A6FC8, &off_83A6FD4, &off_83A6FE0, // 0x94
    &off_83A6FEC, &off_83A6FF4, &off_83A6FFC, &off_83A6898, // 0x98
    &off_83A68A4, &off_83A68B0, &off_83A68BC, &off_83A7004, // 0x9C
    &off_83A6A00, &off_83A6A0C, &off_83A6A18, &off_83A6A24, // 0xA0
    &off_83A700C, &off_83A7018, &off_83A6C3C, &off_83A6C48, // 0xA4 
    &off_83A6C54, &off_83A6C60                              // 0xA8
};

// 08064788
bool npc_obj_ministep_stop_on_arrival(struct npc_states *npc, struct obj_t *o) {
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
bool npc_ministep(struct npc_states *npc, struct obj_t *o) {
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
void step1(struct obj_t *o, u8 d) {
    o->pos_1.x += directions_i16[d].x;
    o->pos_1.y += directions_i16[d].y;
}

// 08068AAC
void step2(struct obj_t *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 2;
    o->pos_1.y += directions_i16[d].y * 2;
}

// 08068AD0
void step3(struct obj_t *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 3;
    o->pos_1.y += directions_i16[d].y * 3;
}

// 08068AF8
void step4(struct obj_t *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 4;
    o->pos_1.y += directions_i16[d].y * 4;
}

// 08068B1C
void step8(struct obj_t *o, u8 d) {
    o->pos_1.x += directions_i16[d].x * 8;
    o->pos_1.y += directions_i16[d].y * 8;
}

// 08068B40
void obj_npc_ministep_reset(struct obj_t *o, u16 speed, u16 phase) {
    o->priv3 = 0;
    o->priv4 = speed;
    o->priv5 = phase;
}

// 083A710C
void (*stepspeed1[])(a, b) = {
    step1, step1, step1, step1,
    step1, step1, step1, step1,
    step1, step1, step1, step1,
    step1, step1, step1, step1
};

// 083A714C
void (*stepspeed2[])(a, b) = {
    step2, step2, step2, step2,
    step2, step2, step2, step2
};

// 083A7184
void (*stepspeed4[])(a, b) = {
    step4, step4, step4, step4
};

// 083A7194
void (*stepspeed5[])(a, b) = {
    step8, step8
};

// 083A719C
void (**stepspeeds[])(struct obj_t *, u8) = {
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
bool obj_npc_ministep(struct obj_t *o) {
    u8   z =  o->private3;
    u16  s =  o->private4;
    u16 *i = &o->private5;
    u16  l = stepspeed_seq_length[s];
    if (*i >= l) return false;
    stepspeeds[s][i](o, z);
    if (*i++ >= l) return true;
    return false;
}

// 08068BBC
void obj_npc_ministep_set_p5(struct obj_t *o, u16 _) {
    o->private3 = _;
    o->private4 = 0;
    o->private5 = 0;
}

// 08063554
void sub_8063554(struct npc_state *npc, struct obj *obj, u8 anim_number) {
    if (npc.field_1 & 0x8) return;
    obj.anim_number = anim_number;
    u8 *tp = animtable_get_tp(obj.anim_table);
    if (tp) {
             if (obj.anim_frame == tp[4]) obj.anim_frame = tp[7];
        else if (obj.anim_frame == tp[5]) obj.anim_frame = tp[6];
    }
    sub_80083C0(obj, obj.anim_frame); // decrements anim_frame
}

// 0806DE28
void npc_hide_and_trainer_flag_clear_on_role_x66_at_pos(stuct npc_state *npc) {
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
