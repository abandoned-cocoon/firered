#include "continuegame.h"
#include "fade.h"
#include "flags.h"
#include "main.h"
#include "memory.h"
#include "npc_cmds.h"
#include "npc_interp.h"
#include "overworld.h"
#include "overworld_effects.h"
#include "overworld_navigation.h"
#include "save.h"
#include "task.h"
#include "u0807d_overworld.h"
#include "vars.h"
#include "whiteout.h"
#include "uncategorized.h"

// to overworld_loading.h
void map_loading_loop_1(u8*);

#ifndef NO_RAM
//* 02031DB4
struct warpdata warp0;
//* 02031DBC
struct warpdata warp1;
//* 02031DC4
struct warpdata warp2;
//* 02031DCC
struct warpdata warp3;
//* 02031DD4
u8 player_avatar_init_params[3];
//* 02031DD8
u8 warp_sound_disable;
//* 02031DDA
u16 wild_pokemon_index;
//* 02031DDC
u8 wild_pokemon_from_water_category;
//* 02036DFC
struct map current_mapheader;
//* 02036E18
struct npc_translate_info translate_info;
//* 02036E24
u32 adjacent_maps_presence_bitfield;
//* 02036E27
u8 color_filter;
//* 03000FAE
u32 bs1_time;
//* 03000FB0
u32 bs1_time_wraparound;
//* 03000FB2
u32 bs2_time;
//* 03000FB4
u32 bs2_time_wraparound;
//* 03000FB8
void (*bs1_func)(void);
//* 03000FBC
void (*bs2_func)(void);
//* 03005014
u16 *overworld_bg2_tilemap;
//* 03005018
u16 *overworld_bg1_tilemap;
//* 0300501C
u16 *overworld_bg3_tilemap;
//* 03005020
void (*map_post_load_hook)(void);
//* 03005024
bool (*hm_phase_1)(void);
//* 03005028
u16 c1_link_related_func_retvl;
//* 0300502C
u8 linknpc_id_self_maybe;
//* 03005030
u32 linknpc_count;
//* 03005050
struct cameradata cameradata;
//* 03005068
u16 nu_y;
//* 0300506C
u16 nu_x;
//* 03005070
u32 script_env_locking_player;
//* 03005074
u32 context_npc; // the npc currently executing a script
#endif

// TODO: Place somewhere
struct cameradata *cd;

// 08055148
void cur_mapdata_load_assets_to_gpu_and_full_redraw() {
    sub_805A5E4();
    mapdata_load_blocksets(current_mapheader.data);
    mapdata_load_palettes_to_gpu(current_mapheader.data);
    cur_mapdata_draw_all_blocks();
    cur_mapheader_run_tileset_funcs_after_some_cpuset();
}

// struct map *mapheader_by_mapnumbers(u8 mapgroup, u8 mapnr) {
//     // TODO
// }

// 08055864
void mliX_load_map(u8 mapgroup, u8 mapnr) {

    warp0_set(mapgroup, mapnr, -1);
    sub_8055E94();
    warp_shift();
    // mli0
    cur_mapheader_load_sav1_and_update_sav1_mapindex();
    copy_rom_npcs_to_ram();
    sav2_set_x9_depending_on_sav1_map();
    sub_806E110();
    nullsub_50();
    sub_806D7E8();
    sub_810C578(mapgroup, mapnr);
    weather_807B140();
    wild_pokemon_reroll();
    sav1_write_flash_status();
    sav1_clear_music_override();
    mapheader_run_script_with_tag_x3();
    sub_815D8F8();
    mapheader_copy_mapdata_and_run_tag_x1();
    // mli1
    mapdata_load_blockset2_(current_mapheader.mapdata_header);
    mapdata_load_palette2_to_gpu(current_mapheader.mapdata_header);

    for (u32 i = 7; i < 12; i++)
        sub_807AB74(i);

    cur_mapheader_run_blockset2_func();
    mapnumbers_history_shift_sav1_0_2_4_out();
    roaming_pokemon_roam();
    prev_quest_postbuffer_cursor_backup_reset();
    weather_set2();
    wild_encounter_reset_coro_args();
    mapheader_run_script_with_tag_x5();
    if ( warp0_get_name() != current_mapheader.name )
        show_new_mapname(1);
}

// 08055920
void mli0_load_map() {
    int light;

    cur_mapheader_load_sav1_and_update_sav1_mapindex();
    copy_rom_npcs_to_ram();
    light = is_light_level_1_2_3_5_or_6(current_mapheader.light);
    sav2_set_x9_depending_on_sav1_map();
    sub_806E110();
    nullsub_50();
    sub_806D7E8();
    sub_810C578(saveblock1_mapdata->location.group & 0xFFFF, saveblock1_mapdata->location.map & 0xFFFF);
    weather_807B140();
    wild_pokemon_reroll();
    if (light)
        flag_clear(2054);
    sav1_write_flash_status();
    sav1_clear_music_override();
    mapheader_run_script_with_tag_x3();
    sub_815D8F8();
    mapnumbers_history_shift_sav1_0_2_4_out();
    roaming_pokemon_roam_freely();
    prev_quest_postbuffer_cursor_backup_reset();
    mapheader_copy_mapdata_and_run_tag_x1();
}

// 080559A8
void sub_80559A8() {
    cur_mapheader_load_sav1_and_update_sav1_mapindex();
    copy_rom_npcs_to_ram();
    is_light_level_1_2_3_5_or_6(current_mapheader.light);
    sav2_set_x9_depending_on_sav1_map();
    weather_807B140();
    wild_pokemon_reroll();
    sav1_write_flash_status();
    prev_quest_postbuffer_cursor_backup_reset();
    sub_8111708();
    cur_mapheader_from_sav1();
    mapheader_copy_mapdata_and_run_tag_x1();
}

// 080559E4 player_avatar_init_params_reset
// 080559F8

// 08055A08
void walkrun_find_lowest_active_bit_in_bitfield() {
    char bit;

    player_avatar_init_params[1] = player_get_direction__sp1AA();

    if ( walkrun_bitfield_and_r0(2) << 24 ) {
        bit = 2;
    } else if ( walkrun_bitfield_and_r0(4) << 24 ) {
        bit = 4;
    } else if ( walkrun_bitfield_and_r0(8) << 24 ) {
        bit = 8;
    } else if ( walkrun_bitfield_and_r0(16) << 24 ) {
        bit = 16;
    } else {
        bit = 1;
    }

    player_avatar_init_params[0] = bit;
    player_avatar_init_params[2] = 0;
}

// 08055A6C
// 08055ACC

// 08055B38
bool is_tile_grass_on_seafoam_island_maybe(u8 a1) {
    struct warpdata *w = &saveblock1_mapdata->location;
    return (is_tile_grass_maybe(a1) == 1) && (
        // Seafoam Island
        w->group == 1 && w->map == 86 ||
        w->group == 1 && w->map == 87
    );
}

// 08055B74

// 08055C74
int cur_mapdata_block_role_at_screen_center_acc_to_sav1() {
    return cur_mapdata_block_role_at(
        (saveblock1_mapdata->camera_position.x + 7),
        (saveblock1_mapdata->camera_position.y + 7));
}

// 08055C9C
// 08055CB8 sav1_write_flash_status
// 08055D08
// 08055D30 sav1_get_flash_used_on_map
// 08055D5C warp1_assign

// music
// 08055D6C warp_target_get_music
// 08055D8C sav1_get_music
// 08055DA4 warp1_target_get_music
// 08055DB8 map_music_set_to_zero_
// 08055DC4 map_music_something
// 08055E78 sav1_set_music_override
// 08055E84 sav1_clear_music_override
// 08055E94
// 08055F1C
// 08055F48

// 08055F68 is_warp1_light_level_8_or_9
// 08055F88 music_something

// 08055FC4
// 08055FD4
// 08055FE0
// 08056078 wild_encounter_related

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
    return mapnumbers_get_light_level(w->group, w->map);
}

// 08056188
u8 sav1_map_get_light_level() {
    return warp_get_light_level(&sav1i->location);
}

// 080561A0
u8 warp0_get_light_level() {
    return warp_get_light_level(&warp0);
}

// 080561B4
u8 warp0_get_name() {
    struct map *m = mapheader_by_mapnumbers(warp0.group, warp0.map);
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
    struct warpdata *l = &sav1i->field_14;
    struct map *m = mapheader_by_mapnumbers(l->group, l->map);
    return m->name;
}

// 08056260
u8 sav1_map_get_name() {
    struct warpdata *l = &sav1i->location;
    struct map *m = mapheader_by_mapnumbers(l->group, l->map);
    return m->name;
}

// 08056288
u8 sav1_map_get_battletype() {
    struct warpdata *l = &sav1i->location;
    struct map *m = mapheader_by_mapnumbers(l->group, l->map);
    return m->battletype;
}

// 080562B0
void overworld_bg_setup_1() {
    saveblocks_randomize_and_relocate();
    sub_8056E80();
    overworld_bg_setup_2();
}

// 0826D320
u32 overworld_bg_setup_data[] = {
    0x01F8,
    0x11D1,
    0x21C2,
    0x31E3
};

// 08056354
void overworld_bg_setup_2() {
    sub_8001618(0);
    overworld_bg_vram_setup(0, overworld_bg_setup_data, 4);
    gpu_bg_config_set_field(1, 5, 1); // mosaic activate (??)
    gpu_bg_config_set_field(2, 5, 1); // mosaic activate (??)
    gpu_bg_config_set_field(3, 5, 1); // mosaic activate (??)
    overworld_bg1_tilemap = mem_alloc_cleared(0x800);
    overworld_bg2_tilemap = mem_alloc_cleared(0x800);
    overworld_bg3_tilemap = mem_alloc_cleared(0x800);
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
    memory_free(overworld_bg3_tilemap);
    memory_free(overworld_bg2_tilemap);
    memory_free(overworld_bg1_tilemap);
}

// 08056420
void flag_clear_safari_zone() {
    flag_clear(FLAG_PLAYER_IN_SAFARI_ZONE);
}

// 0805642C
bool is_c1_link_related_active() {
    return super.callback1 == &c1_link_related;
}

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
    sub_806C888(&d[0]);
    input_process(d, keypad_new, keypad_held);
    sub_806CD30(d);
    if (!script_env_2_is_enabled()) {
        if (sub_806CAC8(d)) {
            script_env_2_enable();
            task_show_mapname_clamp_arg0_low_6();
        } else
            player_step(walkrun_state.running2, keypad_new, keypad_held);
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
    if (sub_8112CAC() == 1 || prev_quest_mode == 2)
        c1_overworld_prev_quest();
    else
        c1_overworld_normal(super.buttons_new_remapped,
                            super.buttons_held_remapped);
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
    if (!t) super.vblank_callback = NULL;
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
    map_post_load_hook = &mapldr_black;
    hm_phase_1 = 0;
    map_loading_loop_1(&super.multi_purpose_state_tracker);
    sub_8056A04();
    set_callback1(&c1_overworld);
    set_callback2(&c2_overworld);
}

// 080566A4
void c2_whiteout() {
    if (super.multi_purpose_state_tracker++ < 120) return;
    sub_80569BC();
    sub_8071A94();
    flag_clear_safari_zone();
    sub_8054BC8();
    sub_80559F8(2);
    script_env_1_start();
    script_env_2_disable();
    map_post_load_hook = &mapldr_whiteout;
    u8 state = 0;
    map_loading_loop_1(&state);
    sub_8112364();
    vblank_handler_set_08056A14();
    set_callback1(&c1_overworld);
    set_callback2(&c2_overworld);
}

//// 0805671C
//void c2_temp_teleport_after_maybe() {
//}

//// 0805674C
//void c2_0805674C() {
//}

//// 08056788
//void sub_8056788() {
//}

//// 080567AC
//void c2_80567AC() {
//}

//// 080567DC
//void c2_exit_to_overworld_2_switch() {
//}

//// 08056808
//void c2_exit_to_overworld_2_local() {
//}

//// 0805682C
//void c2_exit_to_overworld_2_link() {
//}

//// 08056854
//void c2_8056854() {
//}

//// 080568A8
//void c2_c5_trainer_card() {
//}

//// 080568C4
//void c2_exit_to_overworld_1_continue_scripts_and_music() {
//}

//// 080568E0
//void c2_exit_to_overworld_1_continue_scripts_restart_music() {
//}

// 08056F08
void sub_8056F08() {
    sub_8056A34();
    lcd_reset();
    mapdata_load_assets_to_gpu_and_full_redraw();
}

// 0805726C
// void c2_0805726C() {
// 
// }

// 080572A8
// void c2_080572A8() {
// 
// }

// 08057650 (-> map_loading.c ?)
bool map_loading_iteration_5() {
    switch (super.multi_purpose_state_tracker) {
        case 0:
            overworld_bg_setup_2();
            mli0_load_map();
            break;
        case 1:
            dp12_8087EA4();
            dp13_810BB8C();
            clear_all_callback6();
            sub_805ADF8();
            oe_active_list_clear();
            sub_8079C08();
            weather_set_by_sav1_maybe();
            overworld_ensure_per_step_tasks_running();
            mapheader_run_script_with_tag_x5();
            break;
        case 2:
            sub_8056A34();
            lcd_reset();
            break;
        case 3:
            sub_805A5E4();
            break;
        case 4:
            mapdata_load_blockset_1(current_mapheader.mapdata_header);
            break;
        case 5:
            mapdata_load_blockset_2(current_mapheader.mapdata_header);
            break;
        case 6:
            if ( sub_80F682C() == 1 )
                return 0;
            mapdata_load_palettes_to_gpu(current_mapheader.mapdata_header);
            break;
        case 7:
            cur_mapdata_draw_all_blocks();
            break;
        case 8:
            cur_mapheader_run_tileset_funcs_after_some_cpuset();
            pal_fade_control.field_8 &= 0x7Fu;
            sub_807A944(0, 0, 0x3FFFFFFF);
            break;
        default:
            return 1;
    }
    ++super.multi_purpose_state_tracker;
    return 0;
}

// 08057884
// void c1_link_related() {
//     if (!link_mode_is_wireless || !sub_80F90DC() || !sub_800A00C()) {
//         u32 id = linknpc_id_self_maybe;
//         linknpc_process_commands(linknpc_command_buffer, linknpc_id_self_maybe);
//         u16 r = c1_link_related_func(id);
//         sub_8057C4C(r);
//         linknpc_command_buffer_clear_with_x11_();
//     }
// }

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
        0x00003E00, // used for something
        0x0003C000,
        0x00FC0000,
        0x07000000, // is grass/water tile?
        0x18000000,
        0x60000000, // 0=bg2/bg1 1=bg3/bg2 2=bg3/bg2
        0x80000000
    };

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
u8 cur_mapdata_get_block_role_at(u16 x, u16 y) {
    return (u8)cur_mapdata_block_get_field_at(x, y, 0);
}

// 08059080
u32 mapdata_block_get_field(struct mapdata *data, u16 blockid, u8 fieldid) {
    if (blockid >= 0x400) return 0xFF;
    return block_get_field((blockid < 0x280)
        ? data->blockset1->blocks[blockid]
        : data->blockset2->blocks[blockid-0x100],
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

// 03000E90
struct coords8 tilemap_camera_move_something;

// 0805A91C
void overworld_draw_block_type1_on_map_coord(u32 x, u32 y, u16 *blockdef) {
    i16 pos = map_pos_to_screenspace(&tilemap_camera_move_something, x, y);
    if (pos >= 0)
        overworld_draw_block(/*ttype*/1, blockdef, pos);
}

// 0805A948
void cur_mapdata_draw_block_internal(struct mapdata *data, u16 screenpos, s16 x, s16 y) {
    u16 *blockdef;
    u16 blockid = cur_mapdata_get_blockid_at(x, y);
    if (blockid >= 0x400) blockid = 0;
    if (blockid < 0x280)
        blockdef = &data->blockset1->bdef[blockid];
    else
        blockdef = &data->blockset2->bdef[blockid-0x280];
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

// 0805ABB0
void camera_update() {
    int dx, dy, ex, ey;

    if (cd->hook) cd->hook(cd);

    dx = 0;
    dy = 0;

    if (cd->b_x == 0) {
        if (cd->a_x > 0) dx = 1;
        if (cd->a_x < 0) dx = -1;
    }

    if (cd->b_y == 0) {
        if (cd->a_y > 0) dy = 1;
        if (cd->a_y < 0) dy = -1;
    }

    /* ... */

    if (ex || ey) {
        camera_move_maybe(ex, ey);
        overworld_rebase(ex, ey);
        tilemap_move_something(&tilemap_camera_move_something, ex*2, ey*2);
        cur_mapheader_draw_map_slice(&tilemap_camera_move_something, ex*2, ey*2);
    }

    coords8_add(&tilemap_camera_move_something, dx, dy);
    nu_x -= dx;
    nu_y -= dy;
}

// 08069AE4
void script_env_12_start_and_stuff(u8 *scr) {

    script_env_2_x76_clear();
    script_env_2_x75_clear();
    keypad_override_through_script_env_2_disable();

    // XXX: It's 1, not 2!
    script_env_init(&script_env_1, npc_cmds, npc_cmds+npc_cmd_max);
    script_mode_set_bytecode_and_goto(&script_env_1, scr);

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
    for (struct mapscript *s = current_mapheader.scripts; s->tag; s++)
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
    if (prev_quest_mode==3) return 0;
    if (!(ptr = mapheader_get_first_match_from_tagged_ptr_list(2))) return 0;
    script_env_12_start_and_stuff(ptr);
    return 1;
}

//* 0806C888
void sub_806C888(u8 *d) {
    // rename to ptr_andeq_xFF00E000?
    d[0] = 0;
    d[1] &= ~0x1F;
    d[2] = 0;
}

//* 080CBDE8
void context_npc_set_0() {
    context_npc = 0;
    var_8012 = 0xFF; // current text color
}

// 0806FF9C
void cur_mapheader_run_tileset_funcs_after_806FED8() {
    sub_806FED8();
    cur_mapheader_run_blockset1_func();
    cur_mapheader_run_blockset2_func();
}

// 0807002C
void cur_mapheader_run_blockset1_func() {
    struct blockset *bs = current_mapheader.data->blockset1;
    bs1_time = 0;
    bs1_time_wraparound = 0;
    bs1_func = 0;
    if (bs && bs->fptr) bs->fptr();
}

// 08070068
void cur_mapheader_run_blockset2_func() {
    struct blockset *bs = current_mapheader.data->blockset2;
    bs2_time = 0;
    bs2_time_wraparound = 0;
    bs2_func = 0;
    if (bs && bs->fptr) bs->fptr();
}
