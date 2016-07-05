#define ME_SEND_BUF_SIZE 0x400
#include "memory.h"

struct mevent_srv_sub {
	u32 field_0;
	u8  field_4;
	u8  field_5;
	u16 _padding_1;
	u16 field_8;
	u16 field_A;
	u16 field_C;
	u16 _padding_2;
	u16 field_10;
	u16 field_12;
	u16 field_14;
	u16 _padding_3;
	u32 field_18;
	u32 field_1C;
	void (*funcptr_20)(void);
	void (*funcptr_24)(void);
	// size: 0x28
};

struct mevent_cmd {
	u32 opcode;
	u32 flag_or_size;
	u32 parameter;
};

struct mevent_srv {
	u32 field_0;
	u32 unknown1;
	u32 mainseqno;
	u32 cmdindex;
	struct mevent_cmd *cmdbuffer;
	void *field_14;
	void *dstbuffer_1;
	void *dstbuffer_2;
	void *field_20;
	u32 arg2_value_1;
	u32 size_value_1;
	u32 arg2_value_2;
	u32 size_value_2;
	u32 field_34;
	struct mevent_srv_sub sub;
};

// 0203F3C4
struct mevent_srv *mevent_srv_ptr;

// 0814485C
void mevent_srv_sub_init(struct mevent_srv_sub *sub, u32 arg1, u32 arg2) {
	sub->field_4 = arg1;
	sub->field_5 = arg2;
	sub->field_0 = 0;
	sub->field_12 = 0;
	sub->field_14 = 0;
	sub->field_10 = 0;
	sub->field_A = 0;
	sub->field_C = 0;
	sub->field_8 = 0;
	sub->field_1C = 0;
	sub->field_18 = 0;
	sub->funcptr_24 = &sub_081449E0;
	sub->funcptr_20 = &sub_0814490C;
}

// 08144888
void sub_08144888(struct mevent_srv_sub *sub, u16 arg1, u32 arg2, u32 size) {
	sub->field_0 = (u32)0;
	sub->field_E = arg1;
	sub->field_10 = (u16)0;
	sub->field_12 = (u16)0;
	sub->field_14 = size ? size : ME_SEND_BUF_SIZE;
	sub->field_1C = arg2;
}

// 08144F1C
void mevent_srv_init_A() { // wonder news probably
	mevent_srv_ptr = mem_alloc(sizeof(struct mevent_srv));
	mevent_srv_init_common(mevent_srv_ptr, &mevent_srv_init_data_A, 0, 1);
}

// 08144F40
void mevent_srv_init_B() { // wonder cards probably
	mevent_srv_ptr = mem_alloc(sizeof(struct mevent_srv));
	mevent_srv_init_common(mevent_srv_ptr, &mevent_srv_init_data_B, 0, 1);
}

// 08144FA0
void mevent_srv_init_common(struct mevent_srv *srv, void *init_data, u32 arg2, u32 arg3) {
	srv->field_0 = 0;
	srv->field_8 = 0;
	srv->field_18 = mem_alloc_cleared(0x14C);
	srv->field_1C = mem_alloc_cleared(0x1BC);
	srv->field_14 = mem_alloc_cleared(0x400);
	srv->field_20 = mem_alloc_cleared(0x64);
	srv->field_10 = init_data;
	srv->field_C = 0;
	mevent_srv_sub_init(&srv->sub, arg2, arg3);
}

// 0814501C
void sub_0814501C(struct mevent_srv *srv, u16 arg1, u32 arg2, u32 size) {
	assert (size < ME_SEND_BUF_SIZE);
	sub_08144888(&srv->sub, arg1, arg2, size);
}
