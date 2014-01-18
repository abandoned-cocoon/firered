// 080565EC
bool map_post_load_hook_exec() {
    if (hm_phase_1) {
        if (!hm_phase_1()) 
            return false;
    } else {
        if (map_post_load_hook)
            map_post_load_hook());
        else
            map_post_load_hook_default();
    }
    hm_phase_1 = 0;
    map_post_load_hook = 0;
    return true;
}

// 08056B78
bool map_loading_iteration_1(u8 *state, u32 arg1) {
    switch (*state) {
    case 0:
        overworld_bg_setup();
        sub_80569BC();
        ml_mapdata(arg1);
        break;

    case 1:
        allocate_palette_backup();
        break;

    case 2:
        sub_8057024(arg1);
        break;

    case 3:
        if (sub_8113748())
            return true;
        break;

    case 4:
        ml_mapscripts();
            // this is where npc_state[0] is filled for the player
        sub_8057114();
        if (byte_203ADFA != 2) {
            sub_80CC534();
            sub_80CC59C();
        }
        help_context_update();
        break;

    case 5:
        sub_8056A34();
        sub_8056F1C();
        break;

    case 6:
        sub_805A5E4();
        break;

    case 7:
        mapdata_load_blockset_1(current_mapheader.data);
        break;

    case 8:
        mapdata_load_blockset_2(current_mapheader.data);
        break;

    case 9:
        if (sub_80F682C())
            return false;
        mapdata_load_palettes_to_gpu(current_mapheader.data);
        break;

    case 10:
        sub_805A684();
        break;

    case 11:
        cur_mapheader_run_tileset_funcs_after_some_cpuset();
        break;

    case 12:
        if (current_mapheader.name == warp0_get_name() || sub_80F8154(current_mapheader.name, 1) == 0) {
            if ((current_mapheader.escape_rope >> 4) != 1)
                show_new_mapname(0);
        } else {
            sub_80F819C(current_mapheader.name);
            sub_80F8268(current_mapheader.name);
        }
        break;

    case 13:
        if (!map_post_load_hook_exec())
            return false;
        break;

    case 14:
        // state 14 results in while (1);
        return true;

    default:
        return false;
    }

    *state++;
    return false;
}

// 08056E5C
void map_loading_loop_1(u8 *state) {
    while (!map_loading_iteration(state, 0));
}

// 08057300
void map_loading_loop_4(u8 *state) {
    while (!map_loading_iteration(state));
}

// 0807DF64
void map_post_load_hook_default() {
    map_music_something();
    render_prev_quest_text_if_appropriate();
    pal_fade_depending_on_room_and_tile(0);
    script_env_2_enable();
}
