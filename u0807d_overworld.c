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

// 0807DF64
void map_post_load_hook_default() {
    map_music_something();
    render_prev_quest_text_if_appropriate();
    pal_fade_depending_on_room_and_tile(0);
    script_env_2_enable();
}
