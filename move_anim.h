#pragma once
#include "types.h"

extern u8 *ma_cursor;
extern u8 *ma_return_addr;
extern u8 ma_active_tasks;

extern u8 b_anim_attacker;
extern u8 b_anim_defender;

void ma00_load_graphics(void);
void ma01_080728D0(void);
void ma02_instanciate_template(void);
void ma03_launch_coro(void);
void ma04_wait_countdown(void);
void ma05_wait_animation(void);
void ma06_nullsub(void);
void ma07_nullsub(void);
void ma08_finish(void);
void ma09_play_sound(void);
void ma0A_08072C30(void);
void ma0B_0807324C(void);
void ma0C_set_BLDCNT_to_x3F40_and_BLDALPHA_to_argument(void);
void ma0D_reset_BLDCNT_and_BLDALPHA(void);
void ma0E_call(void);
void maOF_return(void);
void ma10_080736AC(void);
void ma11_if_else(void);
void ma12_cond_if(void);
void ma13_goto(void);
void ma14_load_background(void);
void ma15_load_battle_screen_elements(void);
void ma16_wait_for_battle_screen_elements_s2(void);
void ma17_wait_for_battle_screen_elements_s0(void);
void ma18_load_background_probably(void);
void ma19_08073BC8(void);
void ma1A_08073C00(void);
void ma1B_08073C2C(void);
void ma1C_08073ED0(void);
void ma1D_08073FB4(void);
void ma1E_set_BLDCNT(void);
void ma1F_0807406C(void);
void ma20_wait_for_something(void);
void ma21_08074164(void);
void ma22_0807339C(void);
void ma23_08073484(void);
void ma24_skip5(void);
void ma25_080737E4(void);
void ma26_play_song_maybe(void);
void ma27_08073E10(void);
void ma28_080741C8(void);
void ma29_08074214(void);
void ma2A_0807423C(void);
void ma2B_make_side_invisible(void);
void ma2C_make_side_visible(void);
void ma2D_08074320(void);
void ma2E_080743C8(void);
void ma2F_stop_music(void);
