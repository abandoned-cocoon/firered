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

// 0805B3E0
void player_step(u8 running2, u16 keypad_new, u16 keypad_held) {
    struct npc_state *avatar = npc_states[walkrun.npc_id];

    sub_0805CC40(avatar);

    if (walkrun.lock)
        return;
    if (player_lock_for_tile_x54_x55_x56_x57()) // always 1 when (walkrun.bitfield & 0x40)
        return;
    if (sub_0805B45C(avatar, running2))
        return;

    npc_clear_strange_bits(avatar);
    bike_related();

    if (player_override_call()) // always 1 when (walkrun.bitfield & 0x20)
        return;

    player_step_by_keypad(running2, keypad_new, keypad_held);
    walkrun_clear_x20_when_running_fast();
}

// 0805C538
void player_get_pos_to(u16 x, u16 y) {
    *x = npc_states[walkrun.npc_id].x;
    *y = npc_states[walkrun.npc_id].y;
}

// 0805C6C4
u8 player_get_direction() {
    return npc_states[walkrun.npc_id].direction & 0xF;
}

// 0805C700
u8 player_get_height() {
    return npc_states[walkrun.npc_id].height >> 4;
}

// 0806CA4C
void script_env_2_apply_keypad_override(u8 *ignored, u16 *keypad_new, u16 *keypad_held) {
    u8 q[] = {0x40, 0x80, 0x20, 0x10, 0x200, 0x100, 0x8, 0x4};
    u8 i = script_env_2_keypad_override_direction_query();
    if (0<i && i<=8)
        *keypad_new = *keypad_held = q[i-1];
}

//* 08069A54
void sub_8069A54() {
    context_npc_set_0();
    set_03000FA1_to_0();
}

// 0806C8BC
void input_process(u8 *d, u16 keypad_new, u16 keypad_held) {
    u8 role = cur_mapdata_block_role_at_player_pos();
    bool override = is_tile_that_overrides_player_control(role);
    bool now = prev_quest_mode < 2;

    if (!script_env_2_context_is_normal() && keypad_override_through_script_env_2_enabled())
        script_env_2_apply_keypad_override(d, &keypad_new, &keypad_held);

    if (((running1 == 2 && !override) || running1 == 0) && (walkrun.bitfield & 0x40)) {
        if (sub_80BD674() != 4) {
            if ((keypad_new & KEYPAD_START)) d[0] |= 0x04;

            if (now) {
                if ((keypad_new & KEYPAD_SELECT)) d[0] |= 0x08;
                if ((keypad_new & KEYPAD_A))      d[0] |= 0x01;
                if ((keypad_new & KEYPAD_B))      d[0] |= 0x80;
                if ((keypad_new & KEYPAD_R))      d[1] |= 0x01;
            }
        }
        if (now)
            if (keypad_held & KEYPAD_ANYDIR) d[0] |= 0x30;
    }
    if (!override) {
        if (running1 == 2 && running2 == 2)
            d[0] |= 0x40;
        if (running1 == 2)
            d[0] |= 0x02;
    }
    if (now) {
             if (keypad_held & KEYPAD_UP)    d[2] = 2;
        else if (keypad_held & KEYPAD_DOWN)  d[2] = 1;
        else if (keypad_held & KEYPAD_LEFT)  d[2] = 3;
        else if (keypad_held & KEYPAD_RIGHT) d[2] = 4;
        else return;
    }
}

// 0806CAC8
bool sub_806CAC8(u8 *d) {
    sub_8069A54();

    struct npc_position n;
    u8 ndir = player_get_direction();
    player_get_pos_to_and_height(&n);
    block b = cur_mapdata_block_get_field_at(n.x, n.y, 0xFF);
    u8 role = cur_mapdata_block_role_at(n.x, n.y);

    sub_806C888(&byte_3005078);
    byte_3005078[2] = d[2];
    if (sub_8081B30()) return 1;

    if (mapheader_run_first_tag2_script_list_match_conditionally()) return 1;

    // executed every frame
    if (walkrun.bitfield & 0x40) {
        // executed every step
        sub_8054E90(5);
        sub_8146CA4();
        increment_var_x4023_until_1500();
        increment_var_x4025_until_500();
        sub_80CB054();
        increment_var_x4026_on_birth_island_modulo_100();
        if (per_step_2(&n, role, ndir)) {
            byte_3005078[0] |= 0x40;
            return 1;
        }
    }

    if (d[0] & 0x2) { // set in sub_806C8BC when !override && running1 == 2
        if (d[2] == 0 || d[2] == ndir) {
            sub_806CE38(n); // shifts coordinates in the direction the player is looking, updates height
            role = cur_mapdata_block_role_at(n.x, n.y);
            if (launch_signpost_script(&n, role, ndir)) {
                byte_3005078 |= 2;
                return 0;
            }
            // restore old values
            player_get_pos_to_and_height(&n);
            role = cur_mapdata_block_role_at(n.x, n.y);
        }
        //if (d[0] & 0x2) { // always true
        if (trigger_battle(b)) {
            byte_3005078 |= 2;
            return 0;
        }
        //}
    }

    if ((d[0] & 0x10) && d[2] == ndir) {
        if (map_warp_consider(&n, role, ndir)) {
            byte_3005078 |= 0x10;
            return 0;
        }
    }

    // TODO

}

// 0806CD30
void sub_806CD30(u8 *d) {
    if (script_env_2_context_is_normal())
        return;

    if (script_env_2_keypad_sync_lock_countdown) {
        script_env_2_keypad_sync_lock_countdown--;
        return;
    }

    if (script_env_2_x76_query() == 0)
        return;

    u8 direction_held = d[2];

    if (direction_held == 0 || direction_held == player_get_direction()) {

        if (!(walkrun.bitfield & 0x40))
            return;

        script_env_2_start_and_stuff(scr_special_15A);
        script_env_2_enable();

        if (!task_is_running(&task_0806CDF8))
            task_add(&task_0806CDF8, 0x8);

    } else {

        if (script_env_2_x75_query() == 1)
            return;

        u8 dir;
             if (direction_held == 2) dir = 1;
        else if (direction_held == 1) dir = 2;
        else if (direction_held == 3) dir = 3;
        else if (direction_held == 4) dir = 4;
        else return;

        script_env_2_keypad_override_direction_set(dir);
        script_env_2_start_and_stuff(scr_special_15A);
        script_env_2_enable();

    }
}

// 0806CE20
void player_get_pos_to_and_height(struct npc_position *n) {
    player_get_pos_to(&n->x, &n->y);
    n->height = player_get_height();
}

// 0806CE74
u8 cur_mapdata_block_role_at_player_pos() {
    u16 x, y;
    player_get_pos_to(&x, &y);
    return cur_mapdata_block_role_at(x, y);
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

// 0806D5E8
bool per_step_2(struct npc_state *npc, u16 role, u8 direction) {
    // role = player_pos_to_block_role
    if (sub_806D660(npc) == 1)
        return 1;
    if (sub_806DA10(npc, role) == 1)
        return 1;
    if (is_tile_XX_prevent_per_step_scripts(role) == 1)
        return 1;
    if (per_step_scripts(role) == 1)
        return 1;
    if (walkrun.bitfield & 0x40)
        return 0;
    if (is_tile_that_overrides_player_control(role))
        return 0;
    if (repel_per_step() == 1)
        return 1;
    return 0;
}

// 0806D694
bool is_tile_XX_prevent_per_step_scripts(u16 role) {
    return 0;
}

// 0805CCD0
void sub_805CCD0(u8 npc_id, u8 direction) {
    u8 cid = task_add(&taskFF_805CD0C, 0xFF);
    struct task_args_0805CD0C *args = (struct task_args_0805CD0C *) &task[cid].args;
    args.npc_id = npc_id;
    args.direction = direction;
    task_805CD0C(&task[cid]);
}

// 0805CD0C
void taskFF_805CD0C(u8 cid) {
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
