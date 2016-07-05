#pragma once
#include "types.h"

#define BLOCK_FIELD_ROLE 0
#define BLOCK_FIELD_1 1
#define BLOCK_FIELD_2 2
#define BLOCK_FIELD_3 3
#define BLOCK_FIELD_GRASS_WATER 4
#define BLOCK_FIELD_5 5
#define BLOCK_FIELD_BG_ASSIGNMENT 6
#define BLOCK_FIELD_7 7

struct mapdata {
    u32 width;
    u32 height;
    u16 *borderblock;
    u16 *data;
    struct blockset *blockset1;
    struct blockset *blockset2;
    u8 bb_width;
    u8 bb_height;
    u16 field_1A;
};

struct mapscript {
    u8 tag;
    u8 *data; // unaligned
};

struct map {
    struct mapdata   *mapdata_header;
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
    u8 escape_rope;
    u8 showname;
    u8 battletype;
}; // 0x1C

typedef u32 block;

struct blockset {
    u8     isCompressed;
    u8     primaryOrSecondary;
    u16    padding;
    void  *tiles;
    void  *palette;
    u16   *bdef;
    void (*fptr)(void);
    block *blocks;
}; // 0xC

struct conditional_script {
    u16 var1; // not guaranteed to be aligned.
    u16 var2; // read bytewise.
    u8 *ptr;
};

struct cameradata {
    void (*hook)(struct cameradata*);
    void *field04;
    int a_x;
    int a_y;
    int b_x;
    int b_y;
};

//* 02031DB4
extern struct warpdata warp0;
//* 02031DBC
extern struct warpdata warp1;
//* 02031DC4
extern struct warpdata warp2;
//* 02031DCC
extern struct warpdata warp3;
//* 02031DD4
extern u8 player_avatar_init_params[3];
//* 02031DD8
extern u8 warp_sound_disable;
//* 02031DDA
extern u16 wild_pokemon_index;
//* 02031DDC
extern u8 wild_pokemon_from_water_category;
//* 02036DFC
extern struct map current_mapheader;

//* 03000FAE
extern u32 bs1_time;
//* 03000FB0
extern u32 bs1_time_wraparound;
//* 03000FB2
extern u32 bs2_time;
//* 03000FB4
extern u32 bs2_time_wraparound;

//* 03000FB8
extern void (*bs1_func)(void);
//* 03000FBC
extern void (*bs2_func)(void);

//* 03005014
extern u16 *overworld_bg2_tilemap;
//* 03005018
extern u16 *overworld_bg1_tilemap;
//* 0300501C
extern u16 *overworld_bg3_tilemap;
//* 03005020
extern void (*map_post_load_hook)(void);

//* 03005024
extern void (*hm_phase_1)(void); // type ok?
//* 03005028
extern u16 c1_link_related_func_retvl;
//* 0300502C
extern u8 linknpc_id_self_maybe;
//* 03005030
extern u32 linknpc_count;
//* 03005050
extern struct cameradata cameradata;
//* 03005068
extern u16 nu_y;
//* 0300506C
extern u16 nu_x;
//* 03005070
extern u32 script_env_locking_player;
//* 03005074
extern u32 context_npc; // the npc currently executing a script (context_npc instead?)

//* 0826D320
extern u32 overworld_bg_setup_data[];

void mapdata_load_assets_to_gpu_and_full_redraw();
struct map *mapheader_by_mapnumbers(u8 mapbank, u8 mapnr);
void mliX_load_map(u8 mapbank, u8 mapnr);
void mli0_load_map();
void sub_80559A8();
void walkrun_find_lowest_active_bit_in_bitfield();
void is_tile_grass_on_seafoam_island_maybe(u8);
int  cur_mapdata_block_role_at_screen_center_acc_to_sav1();
void wild_pokemon_reroll();
bool sub_8056124(u16 arg0);
u8   mapnumbers_get_light_level(i8 bank, i8 map);
u8   warp_get_light_level(struct warpdata *w);
u8   sav1_map_get_light_level();
u8   warp0_get_light_level();
u8   warp0_get_name();
bool is_light_level_1_2_3_5_or_6(u8 light);
bool is_light_level_1_2_3_or_6(u8 light);
bool is_light_level_8_or_9(u8 light);
u8   sav1_x14_get_name();
u8   sav1_map_get_name();
u8   sav1_map_get_battletype();
void overworld_bg_setup_1();
void overworld_bg_setup_2();
void overworld_free_resources();
void flag_clear_safari_zone();
bool is_c1_x8057884();
void c1_overworld_normal(u16 keypad_new, u16 keypad_held);
void c1_overworld_prev_quest();
void c1_overworld();
void c2_ov_basic();
void c2_ov_to_battle_anim();
void c2_overworld();
void set_callback1(void (*c1)());
void c2_new_game();
void c2_whiteout_maybe();
void sub_8056F08();
bool map_loading_iteration_5();
void c1_link_related();
u8   sub_8058D44(s16 x, s16 y);
u8   sub_8058DC4(s16 x, s16 y);
u16  cur_mapdata_get_blockid_at(s16 x, s16 y);
u32  block_get_field(block b, u8 f);
u32  cur_mapdata_block_get_field_at(u16 x, u16 y, u8 fieldid);
u8   cur_mapdata_block_role_at(u16 x, u16 y);
u32  mapdata_block_get_field(struct mapdata *data, u16 blockid, u8 fieldid);
void mapdata_load_blockset_1(struct mapdata *data);
void mapdata_load_blockset_2(struct mapdata *data);
void mapdata_load_palette1_to_gpu(struct mapdata *data);
void mapdata_load_palette2_to_gpu(struct mapdata *data);
void mapdata_load_blocksets(struct mapdata *data);
void mapdata_load_palettes_to_gpu(struct mapdata *data);
bool sub_8059AF0(u8 role);
bool is_role_x38(u8 role);
bool is_role_x39(u8 role);
bool is_role_x3A(u8 role);
bool is_role_x3B(u8 role);
bool is_role_x02_or_xD1(u8 role);
bool is_role_x21_or_x2B(u8 role);
bool is_role_x21_or_x17(u8 role);
bool sub_8059B8C(u8 role);
bool is_block_that_overrides_player_control(u8 role);
void overworld_draw_block_type1_on_map_coord(u32 x, u32 y, u16 *blockdef);
void cur_mapdata_draw_block_internal(struct mapdata *data, u16 screenpos, s16 x, s16 y);
void overworld_draw_block(u32 ttype, u16 *blockdef, u16 pos);
void cur_mapdata_draw_all_blocks(void); // TODO
void camera_update();
void script_env_12_start_and_stuff(u8 *scr);
u8  *mapheader_get_tagged_pointer(u8 tag);
u8  *mapheader_get_first_match_from_tagged_ptr_list(u8 tag);
bool mapheader_run_first_tag2_script_list_match_conditionally();
void cur_mapheader_run_tileset_funcs_after_806FED8();
void cur_mapheader_run_blockset1_func();
void cur_mapheader_run_blockset2_func();
