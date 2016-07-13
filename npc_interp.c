#include "npc_interp.h"

#ifndef NO_RAM
u8 script_env_2_context;        // 03000EA8
struct script_env script_env_1; // 03000EB0
struct script_env script_env_2; // 03000F28
u8 script_running;              // 03000F9C
#endif

// 080697AC
void script_env_init(struct script_env *s, sbptr *cmd_table, sbptr *cmd_table_max) {
    s->depth = s->mode = 0;
    s->ptr_script = 0;
    s->ptr_asm = 0;
    s->cmd_table = cmd_table;
    s->cmd_table_max = cmd_table_max;
    for(u32 i=0; i< 5; i++) s->vars[i]=0;
    for(u32 i=0; i<20; i++) s->return_addr[i]=0;

}

// 080697E8
bool script_mode_normal(struct script_env *s, u8 *target) {
    s->ptr_script = target;
    s->mode = 1;
    return true;
}

// 080697F4
void script_mode_asm(struct script_env *s, vbptr target) {
    s->ptr_asm = target;
    s->mode = 2;
}

// 080697FC
void script_mode_stop(struct script_env *s) {
    s->ptr_script = 0;
    s->mode = 0;
}

// 08069804
// TODO: script_main_handler

// 08069884
bool script_stack_push(struct script_env *s, u8 *value) {
    if (s->depth > 18) return true;
    s->return_addr[s->depth++] = value;
}

// 080698AC
u8 *script_stack_pop(struct script_env *s) {
    if (s->depth == 0) return 0;
    return s->return_addr[--s->depth];
}

// 080698D0
void script_jump(struct script_env *s, u8 *target) {
    s->ptr_script = target;
}

// 080698D4
void script_call(struct script_env *s, u8 *target) {
    script_stack_push(s, s->ptr_script);
    s->ptr_script = target;
}

// 080698E8
void script_return(struct script_env *s) {
    s->ptr_script = script_stack_pop(s);
}

// nowhere
u8 script_read_byte(struct script_env *s) {
    return *s->ptr_script++;
}

// 080698F8
u16 script_read_half(struct script_env *s) {
    // unaligned read
    return (*s->ptr_script++) + ((*s->ptr_script++)<<8);
}

// 08069910
u32 script_read_word(struct script_env *s) {
    // unaligned read
    return (*s->ptr_script++) + ((*s->ptr_script++)<<8) + ((*s->ptr_script++)<<16) + ((*s->ptr_script++)<<24);
}

// 08069940
void script_unpause() {
    script_running = 1;
}

// 0806994C
void script_pause() {
    script_running = 0;
}

// 08069958
u8 script_is_unpaused() {
    return script_running;
}
