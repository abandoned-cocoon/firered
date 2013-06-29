// 08069ED0
bool s00_nop(struct script_env *s) {
    return false;
}

// 08069ED4
bool s01_nop(struct script_env *s) {
    return false;
}

// 08069ED8
bool s02_end(struct script_env *s) {
    script_status_stop(s);
    return false;
}

// 08069FC8
bool s03_return(struct script_env *s) {
    script_return(s);
    return false;
}

// 08069FD4
bool s04_call(struct script_env *s) {
    script_call(s, script_read_u32(s));
    return false;
}

// 08069FB0
bool s05_goto(struct script_env *s) {
    script_jump(s, script_read_u32(s));
    return false;
}

// 083A7248
u8 compare_modes[] = {1, 0, 0,  // less
                      0, 1, 0,  // equal
                      0, 0, 1,  // more
                      1, 1, 0,  // less or same
                      0, 1, 1,  // more or same
                      1, 0, 1}; // not equal

// 08069FEC
bool s06_if_jump(struct script_env *s) {
    u8 condition = script_read_u8(s);
    u32 target = script_read_u32(s);
    if (compare_mode[condition*3 + s->cmp_result])
        script_jump(s, target);
    return false;
}

// 0806A028
bool s07_if_call(struct script_env *s) {
    u8 condition = script_read_u8(s);
    u32 target = script_read_u32(s);
    if (compare_mode[condition*3 + s->cmp_result])
        script_call(s, target);
    return false;
}

// 0806A150
bool s08_jump_std(struct script_env *s) {
    u32 nr = script_read_u8(s);
    if (nr < sizeof(std_scrips)/4)
        script_jump(s, std_scrips[nr]);
    return false;
}

// 0806A180
bool s09_call_std(struct script_env *s) {
    u32 nr = script_read_u8(s);
    if (nr < sizeof(std_scrips)/4)
        script_call(s, std_scrips[nr]);
    return false;
}

// 0806A1B0
bool s0A_jump_std_if(struct script_env *s) {
    u8 condition = script_read_u8(s);
    u32 nr = script_read_u8(s);
    if (compare_mode[condition*3 + s->cmp_result])
        if (nr < sizeof(std_scrips)/4)
            script_jump(s, std_scrips[nr]);
    return false;
}

// 0806A1FC
bool s0B_call_std_if(struct script_env *s) {
    u8 condition = script_read_u8(s);
    u32 nr = script_read_u8(s);
    if (compare_mode[condition*3 + s->cmp_result])
        if (nr < sizeof(std_scrips)/sizeof(u8*))
            script_jump(s, std_scrips[nr]);
    return false;
}

// 020370A4
void *script_pointer_backup_for_sCF;

// 0806A248
bool s0C_restore_execution_after_sCF(struct script_env *s) {
    script_jump(s, script_pointer_backup_for_sCF);
    return false;
}

// 0806A25C
bool s0D_endscript_killram(struct script_env *s) {
    memset(((u8*)saveblock1_mapdata)+0x361C, 0, 0xFB*4);
    script_status_stop(s);
    return true;
}

// TODO: s0E

// 0806A2B4
bool s0F_u32_var_to_const(struct script_env *s) { // used to load messages
    u8 nr = script_read_byte(s);
    s->vars[nr] = script_read_word(s);
    return false;
}

// 0806A314
bool s10_u8_var_to_const(struct script_env *s) {
    u8 nr = script_read_byte(s);
    s->vars[nr] = (u32)script_read_byte(s);
    return false;
}

// 0806A2FC
bool s11_u8_ptr_to_const(struct script_env *s) {
    u8 value = script_read_byte(s);
    *(u8*)script_read_word(s) = value;
    return false;
}

// 0806A2D8
bool s12_u32_var_to_ptr(struct script_env *s) {
    u8 nr = script_read_byte(s);
    s->vars[nr] = *(u8*)script_read_word(s);
    return false;
}

// 0806A330
bool s13_u8_var_to_ptr(struct script_env *s) {
    *(u8*)script_read_word(s) = (u8)s->vars[nr];
    return false;
}

// 0806A354
bool s14_u32_var_to_var(struct script_env *s) {
    u32 value = s->vars[script_read_byte(s)];
    s->vars[script_read_byte(s)] = value;
    return false;
}

// 0806A374
bool s15_u8_ptr_to_ptr(struct script_env *s) {
    u8 *target = (u8*)script_read_word(s);
    *target = *(u8*)script_read_word(s);
    return false;
}

// 0806A390
bool s16_u16_gvar_to_const(struct script_env *s) {
    u16 *target = var_access(script_read_half(s));
    *target = *(u8*)script_read_half(s);
    return false;
}

// 0806A584
bool s17_u16_gvar_add_const(struct script_env *s) {
    u16 *target = var_access(script_read_half(s));
    *target += *(u8*)script_read_half(s);
    return false;
}

// 0806A5AC
bool s18_u16_gvar_sub_const(struct script_env *s) {
    u16 *target = (u16*)var_access(script_read_half(s));
    *target -= *(u8*)script_read_half(s);
    return false;
}

// 0806A3B4
bool s19_u16_gvar_gvar(struct script_env *s) {
    u16 *target = var_access(script_read_half(s));
    u16 *source = var_access(script_read_half(s));
    *target = *source;
    return false;
}

// 0806A3E0
bool s1A_u16_gvar_gvar(struct script_env *s) {
    u16 *target = var_access(script_read_half(s));
    *target = var_load(script_read_half(s));
    return false;
}

// 0806A40C
int compare123(u32 a, u32 b) {
    if (a < b) return 0;
    if (a > b) return 2;
    return 1;
}

// 0806A42C
bool s1B_cmp_u8_var_var(struct script_env *s) {
    u8 a = script_read_byte(s);
    u8 b = script_read_byte(s);
    s->cmp_result = compare012((u8)s->vars[a], (u8)s->vars[b]);
    return false;
}

// 0806A45C
bool s1C_cmp_u8_var_const(struct script_env *s) {
    u8 a = script_read_byte(s);
    u8 b = script_read_byte(s);
    s->cmp_result = compare012((u8)s->vars[a], b);
    return false;
}

// 0806A484
bool s1D_cmp_u8_var_ptr(struct script_env *s) {
    u8 a = (u8)s->vars[script_read_byte(s)];
    u8 b = *(u8*)script_read_word(s);
    s->cmp_result = compare012(a, b);
    return false;
}

// 0806A4B0
bool s1E_cmp_u8_ptr_var(struct script_env *s) {
    u8 a = *(u8*)script_read_word(s);
    u8 b = s->vars[script_read_word(s)];
    s->cmp_result = compare012(a, b);
    return false;
}

// 0806A4DC
bool s1F_cmp_u8_ptr_const(struct script_env *s) {
    u8 a = *(u8*)script_read_word(s);
    u8 b = script_read_word(s);
    s->cmp_result = compare012(a, b);
    return false;
}

// 0806A4FC
bool s20_cmp_u8_ptr_ptr(struct script_env *s) {
    u8 a = *(u8*)script_read_word(s);
    u8 b = *(u8*)script_read_word(s);
    s->cmp_result = compare012(a, b);
    return false;
}

// 0806A520
bool s21_cmp_u16_gvar_const(struct script_env *s) {
    u16 a = *var_access(script_read_half(s));
    u16 b = script_read_half(s);
    s->cmp_result = compare012(a, b);
    return false;
}

// 0806A550
bool s22_cmp_u16_gvar_gvar(struct script_env *s) {
    u16 a = *var_access(script_read_half(s));
    u16 b = *var_access(script_read_half(s));
    s->cmp_result = compare012(a, b);
    return false;
}

// 08069F94
bool s23_asm_blocking(struct script_env *s) {
    typedef void (*vvptr)();
    ((vvptr)script_read_half(s))();
    return false;
}

// 08069EE4
bool s24_asm_nonblocking(struct script_env *s) {
    script_enter_asm_mode(s, script_read_word(s));
    return true;
}

// 08069EFC
bool s25_extended_command_discard_return_value(struct script_env *s) {
    u16 nr = script_read_u16(s);
    assert(nr < sizeof(specials)/4); // C:/WORK/POKeFRLG/src/pm_lgfr_ose/source/scrcmd.c
    specials[nr]();
    return false;
}

// 08069F3C
bool s26_extended_command_save_return_value(struct script_env *s) {
    u16 *ret = var_access(script_read_half(s));
    u16 nr = script_read_u16(s);
    assert(nr < sizeof(specials)/4); // C:/WORK/POKeFRLG/src/pm_lgfr_ose/source/scrcmd.c
    *ret = specials[nr]();
    return false;
}

// 08069FA4
bool s27_set_to_waitstate(struct script_env *s) {
    // TODO
    return false;
}

// 020370AE
static u16 sa_pause_remaining_frames;

// 0806A990
static void sa_pause() {
    return --sa_pause_remaining_frames == 0;
}

// 0806A9B0
bool s28_pause(struct script_env *s) {
    sa_pause_remaining_frames = script_read_half(s);
    script_enter_asm_mode(s, &sa_pause);
    return true;
}

// 0806A82C
bool s29_flag_set(struct script_env *s) {
    flag_set(script_read_half(s));
    return false;
}

// 0806A840
bool s2A_flag_clear(struct script_env *s) {
    flag_clear(script_read_half(s));
    return false;
}

// 0806A854
bool s2B_flag_check() {
    s->cmp_result = flag_check(script_read_half(s));
    return false;
}

// 0806A9D4
bool s2C_nop(struct script_env *s) {
    return false;
}

// 0806A9D8
bool s2D_nop(struct script_env *s) {
    return false;
}

// 0806A9DC
bool s2E_clear_8000_to_8002(struct script_env *s) {
    var_8000 = var_8001 = var_8002 = 0;
    return false;
}

// 0806B068
bool s2F_music_play(struct script_env *s) {
    u16 songnr = script_read_halfword(s);
    sound_play(songnr);
    return false;
}

// 0806B07C
bool sa_music_check() {
    return sub_80723E0() == 0;
}

// 0806B094
bool s30_music_check(struct script_env *s) {
    script_enter_asm_mode(s, &sa_music_check);
    return true;
}

// 0806B0A8
bool s31_fanfare_play(struct script_env *s) {
    u16 soundnr = script_read_halfword(s);
    fanfare_play(soundnr);
    return false;
}

// 0806B0BC
bool sa_fanfare_wait() {
    return sub_8071C9C();
}

// 0806B0CC
bool s32_fanfare_wait(struct script_env *s) {
    script_enter_asm_mode(s, &sa_fanfare_wait);
    return true;
}

// Some commands missing

// 0806A610 (incorrect)
bool s44_add_item_silently(struct script_env *s) {
    u16 item = var_load(script_read_half(s));
    u16 amount = var_load(script_read_half(s));
    var_800D = bag_check_space(item, amount); //LASTRESULT
    bag_add_item(item, amount); // FIXME!!!
    return false;
}

// 0806A65C
bool s45_remove_item(struct script_env *s) {
    u16 item = var_load(script_read_half(s));
    u16 amount = var_load(script_read_half(s));
    var_800D = bag_remove_item(item, amount); //LASTRESULT
    return false;
}

// 0806A6A0
bool s46_check_item_amount(struct script_env *s) {
    u16 item = var_load(script_read_half(s));
    u16 amount = var_load(script_read_half(s));
    var_800D = bag_check_item_amount(item, amount); //LASTRESULT
    return false;
}

// 0806A6E4
bool s47_check_item(struct script_env *s) {
    u16 item = var_load(script_read_half(s));
    u16 amount = var_load(script_read_half(s));
    var_800D = bag_check_item(item, amount); //LASTRESULT
    return false;
}

// 0806A728
bool s48_get_item_type(struct script_env *s) {
    u16 item = var_load(script_read_half(s));
    var_800D = get_item_type(item); //LASTRESULT
    return false;
}

/*
// 0806A754
bool s49_add_item_pc(struct script_env *s) {
    u16 item = var_load(script_read_half(s));
    u16 qty  = var_load(script_read_half(s));
    var_800D = add_item_pc(item); //LASTRESULT
    return false;
}*/

// Many commands missing

// 083A7294
char **fcode_buffers[] = {
    fcode_buffer2,
    fcode_buffer3,
    fcode_buffer4
};

// 0806BC88
bool s7D_load_pokemon_name(struct script_env *s) {
    u8 fcode_target = script_read_u8(s);
    u16 species = var_read(script_read_u16(s));
    strcpy_xFF_terminated(fcode_buffers[fcode_target], pokemon_names[species]);
    return false;
}
