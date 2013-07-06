struct mapdata {
    u32 width;
    u32 height;
    u16 *borderblock;
    u16 *data;
    struct tileset *tileset1;
    struct tileset *tileset2;
    u8 bb_width;
    u8 bb_height;
    u16 field_1A;
};

struct mapscript {
    u8 tag;
    u8 *data; // unaligned
};

struct map {
    struct mapdata   *data;
    struct mapevents *events;
    struct mapscript *scripts;
    struct mapconn   *connections;
    u16 music;
    u16 mapindex;
    u8 name;
    u8 cave;
    u8 weather;
    u8 light;
    u8 field_18;
    u8 escape_rope
    u8 showname;
    u8 battletype;
}; // 0x1C

typedef u32 block;

struct tileset {
    void *ptr1;
    void *ptr2;
    void *ptr3;
    void *ptr4;
    void (*fptr)(void);
    block *blocks;
}; // 0xC

struct conditional_script {
    u16 var1; // not guaranteed to be aligned.
    u16 var2; // read bytewise.
    u8 *ptr;
}

    /* tagged pointers
    [00] end
    [01][ptr] -> script
    [02][ptr] -> [var][var][ptr->script]  [var][var][ptr] ... [0000]
    [03][ptr] -> script
    [04][ptr] -> [var][var][ptr->script]  [var][var][ptr] ... [0000]
    [05][ptr] -> script
    [06][ptr] -> script
    [07][ptr] -> script
    */

// 08069B80
u8 *mapheader_get_tagged_pointer(u8 tag) {
    for (struct mapscript *s = current_mapheader->scripts; s->tag; s++)
        if (s->tag == tag)
            return s->data;
}

// 08069BD8
u8 *mapheader_get_first_match_from_tagged_ptr_list(u8 tag) {

    struct conditional_script *c;

    if (!c) return 0;
    for (c = (struct conditional_script *)mapheader_get_tagged_pointer(tag); c->var1; c++)
        if (var_load(c->var1) == var_load(c->var2))
            return c->ptr;

    return 0;
}

// 08069C74
bool mapheader_run_first_tag2_script_list_match_conditionally() {
    u8 *ptr;
    if (byte_203ADFA==3) return 0;
    if (!(ptr = mapheader_get_first_match_from_tagged_ptr_list(2))) return 0;
    script_env_12_start_and_stuff(ptr);
    return 1;
}

static inline u16 helper1(s16 x, s16 y) {
    u8  hsize = current_mapheader.data->bb_width;
    u8  vsize = current_mapheader.data->bb_height;
    u16 *grid = current_mapheader.data->borderblock;

    u32 x2 = (hsize<<3)+x-7 % hsize;
    u32 y2 = (vsize<<3)+y-7 % vsize;

    return grid[x + y*hsize] & 0xC00;
}

static inline u16 helper2(s16 x, s16 y) {
    u32 hsize = *(u32* )0x03005040;
    u32 vsize = *(u32* )0x03005044;
    u16 *grid = *(u16**)0x03005048;

    if ((x>=0) && (x<hsize)
     && (y>=0) && (y<vsize))
        return grid[x + y*hsize];
    return helper1(x, y);
}

// 08058D44
u8 sub_8058D44(s16 x, s16 y) {
    u16 blockid = helper2(x, y);
    if (blockid == 0x3FF) return 1;
    return blockid >> 12;
}

// 08058DC4
u8 sub_8058DC4(s16 x, s16 y) {
    u16 blockid = helper2(x, y);
    if (blockid == 0x3FF) return 1;
    return (blockid >> 10) & 0x3;
}

// 08058E48
u16 cur_mapdata_get_blockid_at(s16 x, s16 y) {
    u16 blockid = helper2(x, y);
    if (blockid == 0x3FF)
        blockid = helper1(x, y);
    return blockid & 0x3FF;
}

// 08058F1C
u32 block_get_field(block b, u8 f) {
    // 08352EF0
    const u32 mask[8] = {
        0x000001FF, // role (a.k.a. behaviour byte)
        0x00003E00,
        0x0003C000,
        0x00FC0000,
        0x07000000, // is grass/water tile?
        0x18000000,
        0x60000000,
        0x80000000
    }

    // 08352F10
    const u8 shift[8] = {
        0x0,
        0x9,
        0xE,
        0x12,
        0x18,
        0x1B,
        0x1D,
        0x1F
    };

    if (b>=8) return b;
    return (b & mask[f]) >> shift[f];
}

// 08059080
u32 mapdata_block_get_field(struct mapdata *data, u16 blockid, u8 fieldid) {
    if (blockid >= 0x400) return 0xFF;
    return block_get_field((blockid < 0x280)
        ? data->tileset1->blocks[blockid]
        : data->tileset2->blocks[blockid-0x100],
        fieldid);
}

// 08058F48
u32 cur_mapdata_block_get_field_at(u16 x, u16 y, u8 fieldid) {
    return mapdata_block_get_field(
        current_mapheader.data,
        cur_mapdata_blockid_at(x, y),
        fieldid);
}

// 08058F78
u8 cur_mapdata_block_role_at(u16 x, u16 y) {
    return (u8)cur_mapdata_block_get_field_at(x, y, 0);
}

// 0806CE74
u8 cur_mapdata_block_role_at_player_pos() {
    u16 x, y;
    player_get_pos_to(&x, &y);
    return cur_mapdata_block_role_at(x, y);
}

// 0806C888
void sub_806C888(u8 *d) {
    d[0] = 0;
    d[1] &= ~0x1F;
    d[2] = 0;
}

// 08069AE4
void script_env_12_start_and_stuff(u8 *scr) {

    script_env_2_x76_clear();
    script_env_2_x75_clear();
    keypad_override_through_script_env_2_disable();

    // XXX: It's 1, not 2!
    script_env_init(script_env_1, script_cmds, script_cmd_max);
    script_mode_set_bytecode_and_goto(script_env_1, scr);

    script_env_2_enable();
    script_env_2_context = 0; // running
}

/*
    ignore this

    if (sub_80BD674() != 4) {
        if ((keypad_new & KEYPAD_START)  && (walkrun.bitfield & 0x40))
            d[0] |= 0x04;
        if ((byte_203ADFA-2) >= 2) {
            if (walkrun.bitfield & 0x40) {
                if ((keypad_new & KEYPAD_SELECT))
                    d[0] |= 0x08;
                if ((keypad_new & KEYPAD_A))
                    d[0] |= 0x01;
                if ((keypad_new & KEYPAD_B))
                    d[0] |= 0x80;

            }
        }
    }
    if ((byte_203ADFA-2) >= 2 && keypad_held & 0xF0) // any direction
        d[0] |= 0x30;
*/

// 0806CA4C
void script_env_2_apply_keypad_override(u8 *ignored, u16 *keypad_new, u16 *keypad_held) {
    u8 q[] = {0x40, 0x80, 0x20, 0x10, 0x200, 0x100, 0x8, 0x4};
    u8 i = script_env_2_keypad_override_direction_query();
    if (0<i && i<=8)
        *keypad_new = *keypad_held = q[i-1];
}

// 0806C8BC
void sub_806C8BC(u8 *d, u16 keypad_new, u16 keypad_held) {
    u8 role = cur_mapdata_block_role_at_player_pos();
    bool override = is_tile_that_overrides_player_control(role);

    if (!script_env_2_context_is_normal() && keypad_override_through_script_env_2_enabled())
        script_env_2_apply_keypad_override(d, &keypad_new, &keypad_held);

    if (((running1 == 2 && !override) || running1 == 0) && (walkrun.bitfield & 0x40)) {
        if (sub_80BD674() != 4) {
            if ((keypad_new & KEYPAD_START)) d[0] |= 0x04;

            if ((byte_203ADFA-2) >= 2) {
                if ((keypad_new & KEYPAD_SELECT)) d[0] |= 0x08;
                if ((keypad_new & KEYPAD_A))      d[0] |= 0x01;
                if ((keypad_new & KEYPAD_B))      d[0] |= 0x80;
                if ((keypad_new & KEYPAD_R))      d[1] |= 0x01;
            }
        }
        if ((byte_203ADFA-2) >= 2)
            if (keypad_held & KEYPAD_ANYDIR) d[0] |= 0x30;
    }
    if (!override) {
        if (running1 == 2 && running2 == 2)
            d[0] |= 0x40;
        if (running1 == 2)
            d[0] |= 0x02;
    }
    if ((byte_203ADFA-2) >= 2) {
             if (keypad_held & KEYPAD_UP)    d[2] = 2;
        else if (keypad_held & KEYPAD_DOWN)  d[2] = 1;
        else if (keypad_held & KEYPAD_LEFT)  d[2] = 3;
        else if (keypad_held & KEYPAD_RIGHT) d[2] = 4;
        else return;
    }
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

        if (!coro_is_running(&c3_0806CDF8))
            coro_add_and_set_field_7(&c3_0806CDF8, 0x8);

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

// 03005074
u32 scripting_npc; // the npc currently executing a script

// 080CBDE8
void context_npc_set_0() {
    scripting_npc = 0;
    var_8012 = 0xFF; // current text color
}

// 08069A54
void sub_8069A54() {
    context_npc_set_0();
    set_03000FA1_to_0();
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

// 0203AE8C
void (*funcptr_203AE8C)();

// 0811092C
void call_203AE8C() {
    if (funcptr_203AE8C)
        funcptr_203AE8C();
}

// 080564C8
void c1_overworld_prev_quest() {
    sub_8112B3C();
    sub_805BEB8();

    u8 *d = (u8*)dword_3005E90;
    sub_806C888(d);
    sub_806CD30(d);
    if (!script_env_2_is_enabled()) {
        if (sub_806CAC8(d)) {
            script_env_2_enable();
            coro_show_mapname_clamp_arg0_low_6();
        } else
            call_203AE8C();
    } else if (sub_8111CD0())
        call_203AE8C();
}


// 0805644C
void c1_overworld_normal(u16 keypad_new, u16 keypad_held) {
    sub_8112B3C();
    sub_805BEB8();

    u8 d[4];
    sub_806C888(d);
    sub_806C8BC(d, keypad_new, keypad_held);
    sub_806CD30(d);
    if (!script_env_2_is_enabled()) {
        if (sub_806CAC8(d)) {
            script_env_2_enable();
            coro_show_mapname_clamp_arg0_low_6();
        } else
            sub_805B3E0(walkrun.running2, keypad_new, keypad_held);
    }
    call_203AE8C();
}

// 08056534
void c1_overworld() {
    if (super.callback2 != &c2_overworld) return;
    if (sub_8112CAC() == 1 || byte_203ADFA == 2)
        c1_overworld_prev_quest();
    else
        c1_overworld_normal(super.buttons3_new_remapped,
                            super.buttons2_held_remapped);
}
