#pragma once
#include "types.h"

struct script_env;

typedef bool (*vbptr)();
typedef bool (*sbptr)(struct script_env *s);

struct script_env {
    u8 depth;             // 00
    u8 mode;              // 01 script or asm
    u16 cmp_result;       // 02
    vbptr ptr_asm;        // 04
    u8 *ptr_script;       // 08
    u8 *return_addr[20];  // 0C
    sbptr *cmd_table;     // 5C
    sbptr *cmd_table_max; // 60
    u32 vars[4];          // 64
};

extern u8 script_env_2_context;        // 03000EA8
extern struct script_env script_env_1; // 03000EB0
extern struct script_env script_env_2; // 03000F28
extern u8 script_running;              // 03000F9C

void script_env_init(struct script_env *s, sbptr *cmd_table, sbptr *cmd_table_max);
bool script_mode_normal(struct script_env *s, u8 *target);
void script_mode_asm(struct script_env *s, vbptr target);
void script_mode_stop(struct script_env *s);
bool script_stack_push(struct script_env *s, u8 *value);
u8*  script_stack_pop(struct script_env *s);
void script_jump(struct script_env *s, u8 *target);
void script_call(struct script_env *s, u8 *target);
void script_return(struct script_env *s);
u8   script_read_byte(struct script_env *s);
u16  script_read_half(struct script_env *s);
u32  script_read_word(struct script_env *s);
void script_unpause();
void script_pause();
u8   script_is_unpaused();
