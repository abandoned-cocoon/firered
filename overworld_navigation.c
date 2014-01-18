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

// 080830B8
bool repel_per_step() {
    if (in_trade_center())
        return 0;
    if (byte_203ADFA == 2)
        return 0;

    u16 remaining_steps = var_load(VAR_REPEL_STEPS);
    if (remaining_steps == 0)
        return 0;

    var_set(VAR_REPEL_STEPS, remaining_steps-1)
    if (remainings_steps > 0)
        return 0;

    script_env_12_start_and_stuff(scr_repel_wore_off);
    return 1;
}
