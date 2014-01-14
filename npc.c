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
    struct oam_t *oam;
    void *field_14;
    animtable_t *animtable1;
    gfxtable_t  *gfxtable;
    animtable_t *animtable2;
};

// 080083C0
void sub_80083C0(struct oamt *oamt, u8 f) {
    oamt.anim_frame = f-1;
    u8 q = oamt.field_2C & 0x40;
    oamt.field_2C &= ~0x41;
    oamt.bitfield &= ~0x17;
    anim_player_1(oamt);
    if (oamt.field_2C & 0xC0) {
    }
    oamt.field_2C |= q;
}

// 0805B4D4
void player_step_by_keypad(u8 running2, u16 R1, u16 R2) {
    // reverse engineered by min <3
    if (walkrun_state.bitfield & 6)
        dp04_initiate_movement(running2, R1);
    else
        dp04_continue_movement(running2, R2);
}

// 0805BCEC
void sub_805BCEC(u16 x, u16 y, u8 direction) {
    u8 npc_id;

    if (!flag_check(F_COLLISIONS_ENABLED))
        return 0;
    if ((npc_id = npc_id_by_pos(x, y)) == MAX_NPCS)
        return 0;

    struct npc_state *npc = &npc_states[npc_id];
    if (npc->type_id != 0x61)
        return 0;

    p = npc_states[npc_id].to;
    numbers_move_direction(direction, &next_p.x, &next_p.y);

    u8 role = cur_mapdata_block_role_at(p.x, p.y);
    if (role != 0x66) {
        enum block_reason br = npc_block_way(npc_id, x, y);
        if (block_reason != 0) return 0;
        if (is_role_x60(role)) return 0;
    }
    sub_805CCD0(npc_id, direction);
    return 1;
}

struct task_args_0805CD0C {
    enum mode_0805CD0C {
        ZERO = 0,
        ONE = 1,
        TWO = 2
    } mode;
    u8 npc_id;
    u8 direction;
};

// 0805CCD0
void sub_805CCD0(u8 npc_id, u8 direction) {
    u8 cid = task_add(&task_805CD0C, 0xFF);
    struct task_args_0805CD0C *args = (struct task_args_0805CD0C *) &task[cid].args;
    args.npc_id = npc_id;
    args.direction = direction;
    task_805CD0C(&task[cid]);
}

// 0805CD0C
void task_805CD0C(u8 cid) {
    struct npc_state *player_npc, *other_npc;
    struct task_t *c = &task[cid];
    struct task_args_0805CD0C *args = (struct task_args_0805CD0C *) &c->args;
    do {
        player_npc = npc_states[walkrun.npc_id];
         other_npc = npc_states[  args->npc_id];
    } while(off_835B8A0[args->mode](c, player_npc, other_npc));
}

// 0805CD64
bool sub_805CD64_mode_0(struct task_t* c, struct npc_state* player_npc, struct npc_state* other_npc) {
    struct task_args_0805CD0C *args = (struct task_args_0805CD0C *) &c->args;

    script_env_2_enable();
    walkrun.lock = 1;
    args->mode++; // from 0 to 1
    return 0;
}

// 0805CD84
bool sub_805CD84_mode_1(struct task_t* c, struct npc_state* player_npc, struct npc_state* other_npc) {
    // TODO
    args->mode++; // from 1 to 2
    return 0;
}

// 0805CE20
bool sub_805CE20_mode_2(struct task_t* c, struct npc_state* player_npc, struct npc_state* other_npc) {
    if (npc_get_bit7_or_const_x10_when_inactive(player_npc) == 0) return 0;
    if (npc_get_bit7_or_const_x10_when_inactive(other_npc) == 0) return 0;
    npc_destruct_when_bit7_is_set(player_npc);
    npc_destruct_when_bit7_is_set(other_npc);
    npc_hide_and_trainer_flag_clear_on_role_x66_at_pos(other_npc);
    struct coords *o_to = other_npc.to;
    if_role_x20_run_trigger_at_position_maybe(o_to->x, o_to->y);

    walkrun.lock = 0;
    script_env_2_disable();
    u8 cid = task_find_id_by_funcptr(&task_805CD0C);
    task_del(cid);

    return 0;
}

// 0835B8A0
bool (off_835B8A0[])(struct task_t*, struct npc_state*, struct npc_state*) = {
    &sub_805CD64_mode_0,
    &sub_805CD84_mode_1,
    &sub_805CE20_mode_2
};

// 0805E080
u8 rom_npc_to_npc_state(struct rom_npc *rnpc, u8, u8) {
    // TODO
}

// 0805E590
void *npc_spawn_with_provided_template(byte, void*, byte, byte, short, short) {
    // TODO
    return 0;
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

#define AN(name) bool an_##name(struct npc_state *npc, struct oamt *oamt)
typedef bool (*anptr)(struct npc_state *npc, struct oamt *oamt);

AN(xxx0) {                        return true; } // 08067930
AN(xxx1) { oamt.field_2C |= 0x40; return true; } // 08067934

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

AN(run_dn_1) { an_run_any(npc, oamt, 1, 1); return an_run_dn_2(npc, oamt); } // 080652CC
AN(run_up_1) { an_run_any(npc, oamt, 2, 1); return an_run_up_2(npc, oamt); } // 080652EC
AN(run_lf_1) { an_run_any(npc, oamt, 3, 1); return an_run_lf_2(npc, oamt); } // 080652FC
AN(run_rt_1) { an_run_any(npc, oamt, 4, 1); return an_run_rt_2(npc, oamt); } // 0806530C

AN(run_dn_2) { return npc_ministep_and_modify_priv1(npc, oamt) && (oamt->private4 = 2, true); } // 080652EC
AN(run_up_2) { return npc_ministep_and_modify_priv1(npc, oamt) && (oamt->private4 = 2, true); } // 0806530C
AN(run_lf_2) { return npc_ministep_and_modify_priv1(npc, oamt) && (oamt->private4 = 2, true); } // 0806532C
AN(run_rt_2) { return npc_ministep_and_modify_priv1(npc, oamt) && (oamt->private4 = 2, true); } // 0806534C

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

// 0805C6C4
u8 player_get_direction() {
    return npc_states[walkrun.npc_id].direction & 0xF;
}

// 0805C700
u8 player_get_height() {
    return npc_states[walkrun.npc_id].height >> 4;
}

// 0805C538
void player_get_pos_to(u16 x, u16 y) {
    *x = npc_states[walkrun.npc_id].x;
    *y = npc_states[walkrun.npc_id].y;
}

// 08064788
bool npc_ministep_and_modify_priv1(struct npc_states *npc, struct oam_t *oam) {
    if (!oamt_npc_ministep(oam))
        // stepping continues
        return false;
    // step done, npc is on it's target position
    npc->from.x = npc->to.x;
    npc->from.y = npc->to.y;
    npc->bitfield |= 0x8; // flag for 'on grid' maybe?
    oamt->private1 |= 0x40;
    return true;
}

// 080653CC
bool npc_ministep(struct npc_states *npc, struct oam_t *oam) {
    if (!oamt_npc_ministep(oam))
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
void step1(struct oam_t *oam, u8 d) {
    oam->pos_1.x += directions_i16[d].x;
    oam->pos_1.y += directions_i16[d].y;
}

// 08068AAC
void step2(struct oam_t *oam, u8 d) {
    oam->pos_1.x += directions_i16[d].x * 2;
    oam->pos_1.y += directions_i16[d].y * 2;
}

// 08068AD0
void step3(struct oam_t *oam, u8 d) {
    oam->pos_1.x += directions_i16[d].x * 3;
    oam->pos_1.y += directions_i16[d].y * 3;
}

// 08068AF8
void step4(struct oam_t *oam, u8 d) {
    oam->pos_1.x += directions_i16[d].x * 4;
    oam->pos_1.y += directions_i16[d].y * 4;
}

// 08068B1C
void step8(struct oam_t *oam, u8 d) {
    oam->pos_1.x += directions_i16[d].x * 8;
    oam->pos_1.y += directions_i16[d].y * 8;
}

// 08068B40
void oamt_npc_ministep_reset(struct oam_t *oam, u16 speed, u16 phase) {
    oam->private5 = 0;
    oam->private6 = speed;
    oam->private7 = phase;
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
void (**stepspeeds[])(struct oam_t *, u8) = {
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
bool oamt_npc_ministep(struct oam_t *oam) {
    u8   z =  oam->private5;
    u16  s =  oam->private6;
    u16 *i = &oam->private7;
    u16  l = stepspeed_seq_length[s];
    if (*i >= l) return false;
    stepspeeds[s][i](oam, z);
    if (*i++ >= l) return true;
    return false;
}

// 08068BBC
void oamt_npc_ministep_set_p5(struct oam_t *oam, u16 _) {
    oam->private5 = _;
    oam->private6 = 0;
    oam->private7 = 0;
}

// 0806CE20
void player_get_pos_to_and_height(struct npc_position *n) {
    player_get_pos_to(&n->x, &n->y);
    n->height = player_get_height();
}

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

// 0805F2C8
struct npc_type *npc_get_type(u8 type_id) {
    if (type_id >= 0xF0)
        type_id = var_load_x4010_plus(type_id-0xF0);
    if (type_id >= 152)
        type_id = 10;
    return npc_types[type_id];
}

// 0805FC5C
u16 trainerid_by_local_id_and_map(u8 local_id, u8 mapnr, u8 mapgroup) {
    struct rom_npc *rnpc = rom_npc_by_nr_and_map(local_id, mapnr, mapgroup);
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

// 08063554
void sub_8063554(struct npc_state *npc, struct oamt *oamt, u8 anim_number) {
    if (npc.field_1 & 0x8) return;
    oamt.anim_number = anim_number;
    u8 *tp = animtable_get_tp(oamt.anim_table);
    if (tp) {
             if (oamt.anim_frame == tp[4]) oamt.anim_frame = tp[7];
        else if (oamt.anim_frame == tp[5]) oamt.anim_frame = tp[6];
    }
    sub_80083C0(oamt, oamt.anim_frame); // decrements anim_frame
}

// 0806CEA0
bool onpress_a(struct npc_position *n, u8 role, u8 direction) {
    // direction: direction the player is looking

    u8 *scr = (u8*)onpress_a_get_script(n, role, direction);
    if (!scr) return false;
    if (scr != scr_pc1 && scr != scr_pc2) sound_play(5);
    script_start_1(scr);
    return true;
}

// 0806CEE0
u8 *onpress_a_get_script(struct npc_position *n, u8 role, u8 direction) {
    u32 scr;
    if (scr=onpress_a_get_script_npc     (n, role, direction)) return scr;
    if (scr=onpress_a_get_script_signpost(n, role, direction)) return scr;
    if (scr=onpress_a_get_script_block   (n, role, direction)) return scr;
    if (scr=surf                         (n, role, direction)) return scr;
    return 0;
}

// 0806CFF4
u32 onpress_a_get_script_npc(struct npc_position *n, u8 role, u8 direction) {
    // direction: direction the player is looking

    u8 npc_id = npc_id_by_pos_and_height(
        n->x,
        n->y,
        n->height
    );
    if (npc_id == MAX_NPCS || npc_states[npc_id].local_id == 0xFF) {
        if (!is_role_x80(tt)) return 0;

        npc_id = npc_id_by_pos_and_height(
            n->x + directions_i32[d*2+0],
            n->y + directions_i32[d*2+1],
            n->height
        );
        if (npc_id == MAX_NPCS || npc_states[npc_id].local_id == 0xFF) return 0;
    }
    if (in_trade_center() && !sub_08063D68(&npc_states[npc_id])) return 0;

    scripting_npc = npc_id;
    var_800F = npc_states[npc_id].local_id;
    var_800C = d;
    return sub_08069D8C(var_800F, npc_get_script_by_npc_id(npc_id));
}

// 0806D1F0
u32 onpress_a_get_script_block(struct npc_position *n, u8 role, u8 direction) {
    // direction: direction the player is looking

    var_800C = d;

    if (is_role_x83(tt)) return 0x081A6955; // (complex stuff. probably for the PC)
    if (is_role_x85(tt)) return 0x081A6C32; // (complex stuff)
    if (is_role_x81(tt)) return 0x081A7606; // It`s crammed full of POKeMON books.
    if (is_role_x82(tt)) return 0x081A760F; // Wow! Tons of POKeMON stuff!
    if (is_role_x90(tt)) return 0x081A7618; // It should be packed with all kinds of delicious things to eat!
    if (is_role_xA0(tt)) return 0x081A7633; // It`s a machine of some sort. It sure is impressive.
    if (is_role_x93(tt)) return 0x081A763C; // It`s a blueprint of some sort. It`s filled with diagrams and text.
    if (is_role_xA1(tt)) return 0x081A7621; // It`s the latest video game! It sure looks fun!
    if (is_role_xA2(tt)) return 0x081A7645; // There are obvious signs of burglary here|
    if (is_role_x97(tt)) return 0x081A762A; // It`s all complicated words and numbers that make no sense|
    if (is_role_xA3(tt)) return 0x081C549C; // (complex stuff)
    if (is_role_x86(tt)) return 0x081A764E; // There`s a POKeMON on TV! It looks like it`s having fun.
    if (is_role_x89(tt)) return 0x081A7657; // Dishes and plates are neatly lined up.
    if (is_role_x8A(tt)) return 0x081A7660; // It smells delicious! Somebody`s been cooking here.
    if (is_role_x8B(tt)) return 0x081A7669; // It`s a nicely made dresser. It will hold a lot of stuff.
    if (is_role_x8C(tt)) return 0x081A7672; // There`s a pile of snacks here.
    if (is_role_x94(tt)) return 0x081A767B; // It`s a pretty picture of a POKeMON. It looks like it`s feeling good.
    if (is_role_x95(tt)) return 0x081A7684; // What could this machine be? Better not mess around with it!
    if (is_role_x96(tt)) return 0x081A768D; // It`s a telephone. Better not use it.
    if (is_role_x98(tt)) return 0x081A7696; // It`s an advertising poster about all kinds of products.
    if (is_role_x99(tt)) return 0x081A769F; // Oh, that smells tasty! It might get the stomach growling.
    if (is_role_x9A(tt)) return 0x081A76A8; // Inside this¦ ¦¦_There`s nothing here!
    if (is_role_x9B(tt)) return 0x081A76B1; // It`s a cup with a POKeMON mark on it.
    if (is_role_NO1(tt)) return 0x081A76BA; // The window is very well polished.
    if (is_role_NO2(tt)) return 0x081A76C3; // Outside the window¦_The sky looks fantastically beautiful.
    if (is_role_x9E(tt)) return 0x081A76CC; // Lights in different colors are flashing on and off.
    if (is_role_x9F(tt)) return 0x081A76D5; // All sorts of tools are lined up neatly.
    if (is_role_x8D_and_d_is_x2(tt, d)) return 0x081BBFD8; // complex stuff.
    if (is_role_x8F(tt)) return 0x081A7702; // complex stuff.
    if (is_role_x8E_and_d_is_x2(tt, d)) return 0x081BB8A7; // complex stuff.
    if (is_role_x91(tt)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76F0; // INDIGO PLATEAU_The ultimate goal of TRAINERS! POKeMON LEAGUE HQ
    }
    if (is_role_x92(tt)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76F9; // INDIGO PLATEAU_The highest POKeMON authority! POKeMON LEAGUE HQ
    }
    if (is_role_x88_and_d_is_x2(tt, d)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76DE; // All your item needs fulfilled! POKeMON MART
    }
    if (is_role_x87_and_d_is_x2(tt, d)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76E7; // Heal Your POKeMON! POKeMON CENTER
    }
    return 0;
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
