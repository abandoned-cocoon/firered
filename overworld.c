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
};

//* 03005014
u16 *overworld_bg2_tilemap;
//* 03005018
u16 *overworld_bg1_tilemap;
//* 0300501C
u16 *overworld_bg3_tilemap;

// 08055148
void mapdata_load_assets_to_gpu_and_full_redraw() {
    sub_805A5E4();
    mapdata_load_blocksets(current_mapheader.data);
    mapdata_load_palettes_to_gpu(current_mapheader.data);
    cur_mapdata_draw_all_blocks();
    cur_mapheader_run_tileset_funcs_after_some_cpuset();
}

// 0805610C
void wild_pokemon_reroll() {
    wild_pokemon_index = wild_pokemon_rand_for_map(&wild_pokemon_from_water_category);
}

// 08056124
bool sub_8056124(u16 arg0) {
    if (arg0 != 0x11A) return true;
    if (arg0 != 0x017) return true;
    u8 name = current_mapheader.name;
    if (name == 0x84) return false;
    if (name == 0x7B) return false;
    if (name == 0x61) return false;
    return true;
}

// 08056158
u8 mapnumbers_get_light_level(i8 bank, i8 map) {
    struct map *m = mapheader_by_mapnumbers(bank, map);
    return m->light;
}

// 08056170
u8 warp_get_light_level(struct warpdata *w) {
    return mapnumbers_get_light_level(w->bank, w->map);
}

// 08056188
u8 sav1_map_get_light_level() {
    return warp_get_light_level(&sav1->location);
}

// 080561A0
u8 warp0_get_light_level() {
    return warp_get_light_level(&warp0);
}

// 080561B4
u8 warp0_get_name() {
    struct map *m = mapheader_by_mapnumbers(warp0.bank, warp0.map);
    return m->name;
}

// 080561D8
bool is_light_level_1_2_3_5_or_6(u8 light) {
    return (light == 1) || (light == 2) || (light == 3) || (light == 5) || (light == 6);
}

// 080561FC
bool is_light_level_1_2_3_or_6(u8 light) {
    return (light == 1) || (light == 2) || (light == 3) || (light == 6);
}

// 0805621C
bool is_light_level_8_or_9(u8 light) {
    return (light == 8) || (light == 9);
}

// 08056238
u8 sav1_x14_get_name() {
    // unused
    struct warpdata *l = &sav1->field_14;
    struct map *m = mapheader_by_mapnumbers(l->bank, l->map);
    return m->name;
}

// 08056260
u8 sav1_map_get_name() {
    struct warpdata *l = &sav1->location;
    struct map *m = mapheader_by_mapnumbers(l->bank, l->map);
    return m->name;
}

// 08056288
u8 sav1_map_get_battletype() {
    struct warpdata *l = &sav1->location;
    struct map *m = mapheader_by_mapnumbers(l->bank, l->map);
    return m->battletype;
}

// 080562B0
void overworld_bg_setup_1() {
    saveblocks_randomize_and_relocate();
    sub_8056E80();
    overworld_bg_setup_2();
}

// 08056354
void overworld_bg_setup_2() {
    sub_8001618(0);
    overworld_bg_vram_setup(0, unk_826D320, 4);
    gpu_bg_config_set_field(1, 5, 1); // mosaic activate (??)
    gpu_bg_config_set_field(2, 5, 1); // mosaic activate (??)
    gpu_bg_config_set_field(3, 5, 1); // mosaic activate (??)
    overworld_bg1_tilemap = malloc_and_clear(0x800);
    overworld_bg2_tilemap = malloc_and_clear(0x800);
    overworld_bg3_tilemap = malloc_and_clear(0x800);
    bgid_set_tilemap(1, overworld_bg1_tilemap);
    bgid_set_tilemap(2, overworld_bg2_tilemap);
    bgid_set_tilemap(3, overworld_bg3_tilemap);
    sub_80F6C6C();
    sub_80F6C98();
    sub_8069348();
}

// 080563F0
void overworld_free_resources() {
    sub_80F6C8C(); // free rboxes
    free(overworld_bg3_tilemap);
    free(overworld_bg2_tilemap);
    free(overworld_bg1_tilemap);
}

// 08056420
void flag_clear_safari_zone() {
    flag_clear(FLAG_PLAYER_IN_SAFARI_ZONE);
}

// 0805642C
bool is_c1_x8057884() {
    return super.c1 == &c1_x8057884;
}

//* 0203AE8C
void (*funcptr_203AE8C)();

//* 0811092C
void call_203AE8C() {
    if (funcptr_203AE8C)
        funcptr_203AE8C();
}

// 0805644C
void c1_overworld_normal(u16 keypad_new, u16 keypad_held) {
    sub_8112B3C();
    sub_805BEB8();

    u8 d[4];
    sub_806C888(d);
    input_process(d, keypad_new, keypad_held);
    sub_806CD30(d);
    if (!script_env_2_is_enabled()) {
        if (sub_806CAC8(d)) {
            script_env_2_enable();
            task_show_mapname_clamp_arg0_low_6();
        } else
            player_step(walkrun.running2, keypad_new, keypad_held);
    }
    call_203AE8C();
}

// 080564C8
void c1_overworld_prev_quest() {
    sub_8112B3C();
    sub_805BEB8();

    u8 d[4];
    sub_806C888(d);
    *(u32*)d = dword_3005E90;
    sub_806CD30(d);
    if (!script_env_2_is_enabled()) {
        if (sub_806CAC8(d)) {
            script_env_2_enable();
            task_show_mapname_clamp_arg0_low_6();
        } else
            call_203AE8C();
    } else if (sub_8111CD0())
        call_203AE8C();
    sub_806C888((u8*)dword_3005E90);
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

// 08056578
void c2_ov_basic() {
    script_something();
    task_exec();
    obj_exec();
    camera_update();
    sub_8115798();
    sub_805AE28();
    obj_sync_something();
    fade_and_return_progress_probably();
    sub_806FFBC();
    tilemaps_sync();
}

// 080565A8
void c2_ov_to_battle_anim() {
    c2_ov_basic();
}

// 080565B4
void c2_overworld() {
    u8 *trs = (u8*)0x02037AB8;
    bool t = trs[7] >> 7;
    if (!t) super.field_C = NULL;
    c2_ov_basic();
    if (!t) sub_8056A04();
}

// 080565E0
void set_callback1(void (*c1)()) {
    super.callback1 = c1;
}

// 08056644
void c2_new_game() {
    sub_80569BC();
    sub_8071A94();
    flag_clear_safari_zone();
    new_game();
    sub_80559E4();
    gameclock_sanity_check();
    script_env_1_start();
    script_env_2_disable();
    map_post_load_hook = &mapldr_for_new_game;
    hm_phase_1 = 0;
    map_loading_loop_1(&super.map_loading_state);
    sub_8056A04();
    set_callback1(&c1_overworld);
    set_callback2(&c2_overworld);
}

// 080566A4
void c2_whiteout_maybe() {
    if (superstate.map_loading_state++ < 120) return;
    sub_80569BC();
    sub_8071A94();
    flag_clear_safari_zone();
    sub_8054BC8();
    sub_80559F8(2);
    script_env_1_start();
    script_env_2_disable();
    map_post_load_hook = &mapldr_c3_whiteout;
    u8 state = 0;
    map_loading_loop_1(&state);
    sub_8112364();
    vblank_handler_set_08056A14();
    set_callback1(&c1_overworld);
    set_callback2(&c2_overworld);
}

////

// 08056F08
void sub_8056F08() {
    sub_8056A34();
    lcd_reset();
    mapdata_load_assets_to_gpu_and_full_redraw();
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
        0x18000000, // 0=bg2/bg1 1=bg3/bg2 2=bg3/bg2
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

// 08059080
u32 mapdata_block_get_field(struct mapdata *data, u16 blockid, u8 fieldid) {
    if (blockid >= 0x400) return 0xFF;
    return block_get_field((blockid < 0x280)
        ? data->tileset1->blocks[blockid]
        : data->tileset2->blocks[blockid-0x100],
        fieldid);
}

// 08059A54
void mapdata_load_blockset_1(struct mapdata *data) {
    blockset_load(data->blockset1, 0x280, 0x000);
}

// 08059A68
// 08059A7C
void mapdata_load_blockset_2(struct mapdata *data) {
    blockset_load(data->blockset2, 0x180, 0x280);
}

// 08059A90
void mapdata_load_palette1_to_gpu(struct mapdata *data) {
    blockset_load_palette_to_gpu(data->blockset1, 0x00, 0xE0);
}

// 08059AA0
void mapdata_load_palette2_to_gpu(struct mapdata *data) {
    blockset_load_palette_to_gpu(data->blockset2, 0x70, 0xC0);
}

// 08059AB0
void mapdata_load_blocksets(struct mapdata *data) {
    if (!data) return;
    blockset_load(data->blockset1, 0x280, 0x000);
    blockset_load(data->blockset2, 0x180, 0x280);
}

// 08059AD8
void mapdata_load_palettes_to_gpu(struct mapdata *data) {
    if (!data) return;
    mapdata_load_palette1_to_gpu(data);
    mapdata_load_palette2_to_gpu(data);
}

// 08059AF0
bool sub_8059AF0(u8 role) {
    return true;
}

// 08059AF4
bool is_role_x38(u8 role) {
    return role == 0x38;
}

// 08059B08
bool is_role_x39(u8 role) {
    return role == 0x39;
}

// 08059B1C
bool is_role_x3A(u8 role) {
    return role == 0x3A;
}

// 08059B30
bool is_role_x3B(u8 role) {
    return role == 0x3B;
}

// 08059B44
bool is_role_x02_or_xD1(u8 role) {
    return (role == 0x02) || (role == 0xD1);
}

// 08059B5C
bool is_role_x21_or_x2B(u8 role) {
    return (role == 0x21) || (role == 0x2B);
}

// 08059B74
bool is_role_x21_or_x17(u8 role) {
    return (role == 0x21) || (role == 0x17);
}

// 08059B8C
bool sub_8059B8C(u8 role) {
    return false;
}

// 08059D70
bool is_block_that_overrides_player_control(u8 role) {
    // 0x40:0x44 walk
    // 0x44:0x48 slide
    // 0x49      random slide?
    if (role >= 0x40 && role < 0x49)
        return 1;
    // 0x50:0x54 run
    if (role >= 0x50 && role < 0x54)
        return 1;
    // 0x13 waterfall down
    if (role == 0x13)
        return 1;
    // 0x23 ???
    if (role == 0x23)
        return 1;
    // 0x54:0x58 ???
    if (role >= 0x54 && role < 0x58)
        return 1;
    return 0;
}

// 0805A948
void cur_mapdata_draw_block_internal(struct mapdata *data, u16 screenpos, s16 x, s16 y) {
    u16 *blockdef;
    u16 blockid = cur_mapdata_get_blockid_at(x, y);
    if (blockid >= 0x400) blockid = 0;
    if (blockid < 0x280)
        blockdef = &data->tileset1.bdef[blockid];
    else
        blockdef = &data->tileset2.bdef[blockid-0x280];
    u32 ttype = cur_mapdata_block_get_bgs_at(x, y);
    overworld_draw_block(ttype, blockdef, screenpos);
}

// 0805A9B4
void overworld_draw_block(u32 ttype, u16 *blockdef, u16 pos) {
    u16 *side_a, *side_b, *side_c;
    u16 empty;

    if (ttype == 0) {
        side_c = overworld_bg3_tilemap;
        side_a = overworld_bg2_tilemap;
        side_b = overworld_bg1_tilemap;
        empty = 0x3014;
    } else if (ttype == 1) {
        side_a = overworld_bg3_tilemap;
        side_b = overworld_bg2_tilemap;
        side_c = overworld_bg1_tilemap;
        empty = 0x0000;
    } else if (ttype == 2) {
        side_a = overworld_bg3_tilemap;
        side_c = overworld_bg2_tilemap;
        side_b = overworld_bg1_tilemap;
        empty = 0x0000;
    } else
        return;

    side_a[pos+ 0] = blockdef[0];
    side_a[pos+ 1] = blockdef[1];
    side_a[pos+32] = blockdef[2];
    side_a[pos+33] = blockdef[3];

    side_b[pos+ 0] = blockdef[4];
    side_b[pos+ 1] = blockdef[5];
    side_b[pos+32] = blockdef[6];
    side_b[pos+33] = blockdef[7];

    side_c[pos+ 0] = empty;
    side_c[pos+ 1] = empty;
    side_c[pos+32] = empty;
    side_c[pos+33] = empty;
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

    return grid[x + y*hsize]; // & 0xC00;
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

//* 0806C888
void sub_806C888(u8 *d) {
    d[0] = 0;
    d[1] &= ~0x1F;
    d[2] = 0;
}

//* 03005074
u32 scripting_npc; // the npc currently executing a script

//* 080CBDE8
void context_npc_set_0() {
    scripting_npc = 0;
    var_8012 = 0xFF; // current text color
}

// 0806CE74
u8 cur_mapdata_block_role_at_player_pos() {
    u16 x, y;
    player_get_pos_to(&x, &y);
    return cur_mapdata_block_role_at(x, y);
}

// 0806FF9C
void cur_mapheader_run_tileset_funcs_after_806FED8() {
    sub_806FED8();
    cur_mapheader_run_blockset1_func();
    cur_mapheader_run_blockset2_func();
}

// 0807002C
void cur_mapheader_run_blockset1_func() {
    struct blockset *bs = current_mapheader.data.blockset1;
    word_3000FAE = 0;
    word_3000FB0 = 0;
    dword_3000FB8 = 0;
    if (bs && bs->funcptr) bs->funcptr();
}

// 08070068
void cur_mapheader_run_blockset2_func() {
    struct blockset *bs = current_mapheader.data.blockset2;
    word_3000FB2 = 0;
    word_3000FB4 = 0;
    dword_3000FBC = 0;
    if (bs && bs->funcptr) bs->funcptr();
}

// 0807DB58
void pal_fill_for_maplights() {
    u8 warp0_light    = warp_get_light_level();
    u8 sav1_map_light = sav1_map_get_light_level();
    u8 ft = fade_type_for_given_maplight_pair(warp0_light, sav1_map_light);
    if (ft == 0) {
        palette_bg_faded_fill_black();
        sub_807A818(0, 0);
        palette_bg_faded_fill_black();
    } else if (ft == 1) {
        palette_bg_faded_fill_white();
        sub_807A818(2, 0);
        palette_bg_faded_fill_white();
    }
}
