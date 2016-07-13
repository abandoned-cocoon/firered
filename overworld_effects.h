#pragma once
#include "types.h"

struct obj;

void oe_active_list_clear();

bool oei_cut(void);
void hm_cut_run_scr(void);
bool oei_cut2(void);
bool oei_3A_08097A48(void);
void objc_08097BA8(struct obj *obj);
void oei_cut2_finish(void);

// interp.c
u32 oe_exec(u8 num);
bool oec00_load_gfx(u8 **cursor, u32 *retval);
bool oec01_load_pal(u8 **cursor, u32 *retval);
bool oec02_load_pal(u8 **cursor, u32 *retval);
bool oec03_call_asm(u8 **cursor, u32 *retval);
bool oec04_end(u8 **cursor, u32 *retval);
bool oec05_combined_00_01_03(u8 **cursor, u32 *retval);
bool oec06_combined_00_03(u8 **cursor, u32 *retval);
bool oec07_combined_01_03(u8 **cursor, u32 *retval);
u32 oe_read_word(u8 **cursor);
void oec00_load_gfx_impl(u8 **cursor);
void tint_palette();
void oec01_load_pal_impl(u8 **cursor);
void oec02_load_pal_impl(u8 **cursor);
void oec03_call_asm_impl(u8 **cursor, u32 *retval);
void oe_stop2(struct obj *o);
void oe_stop(struct obj *o, u8 num);
void gpu_tile_obj_free_by_ado_when_unused_maybe(u16 tag);
void gpu_pal_free_by_index_when_unused(u8 pal);
void oe_active_list_clear();
void oe_active_list_add(u8 num);
void oe_active_list_remove(u8 num);
bool oe_active_list_contains(u8 num);
extern bool (*oe_cmds[])(u8 **, u32 *);


// 02039870
extern void *dword_2039870;

// effects_desc.s
extern u8 **oe_table;
