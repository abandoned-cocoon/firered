
#ifndef NO_RAM
//* 020371F8
palette palette_bg_unfaded[16];
//* 020373F8
palette palette_obj_unfaded[16];
//* 020375F8
palette palette_bg_faded[16];
//* 020377F8
palette palette_obj_faded[16];
//* 020379F8
struct { u8 unknown[12]; } stru_20379F8[16]; // unused?
//* 02037AB8
struct fade_ctrl pal_fade_control;
//* 02037AC8
u32 copy_of_fade_ctrl_pal_bitfield;
//* 02037ACC
palette palette_decompress_buffer[32];
#endif
