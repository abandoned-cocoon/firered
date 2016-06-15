#pragma once
#include "types.h"
#include "main.h" // for sprite
#include "uncategorized.h"

// provisional
typedef struct sprite gpu_sprite;
typedef struct coords8 coords8;
typedef struct coords16 coords16;

#define NUM_OBJS 0x40
#define NUM_ROTSCALE_OBJS 0x20
#define NUM_COPY_QUEUE_ENTRIES 0x40

#define OBJ_BIT2_IN_USE    1
#define OBJ_BIT2_INVISIBLE 4

#define OBJ_BIT1_HFLIP               0x01
#define OBJ_BIT1_VFLIP               0x02
#define OBJ_BIT1_ANIM_IMAGE_BEGIN    0x04 // TODO
#define OBJ_BIT1_SHARES_RESOURCES    0x40
#define OBJ_BIT1_ANIM_ROTSCALE_BEGIN 0x80 // TODO

#define OBJ_ANIM_IMAGE_PAUSED     0x40
#define OBJ_ANIM_ROTSCALE_PAUSED  0x80
#define OBJ_ANIM_ANY_PAUSED       0xC0

typedef struct {
	void *data;
	u16 size;
	u16 tag;
} gfxentry;

struct image_frame {
	u16 data;
	union {
		u16 _rd;
		struct {
			u8 duration : 6; // or goto-target
			u8 hflip    : 1;
			u8 vflip    : 1;
			u8 padding  : 8;
		};
	};
};

struct rotscale_frame {
	u16 scale_x_delta;
	u16 scale_y_delta;
	u8  rotation_delta;
	u8  duration;
	u16 _padding;
};

struct rotscale_state {
	u8 index;
	u8 subindex;
	u8 delay_countdown;
	u8 field_3;
	u16 scale_x;
	u16 scale_y;
	u16 rotation;
	u16 _padding2;
};

typedef struct image_frame    *image_seq;
typedef struct rotscale_frame *rotscale_seq;

struct proto_t {
	u16 tile_tag;
	u16 pal_tag;
	gpu_sprite   *sprite; // a.k.a. OAM
	image_seq    *image_anims;
	gfxentry     *gfx_table;
	rotscale_seq *rotscale_anims;
	void *callback;
};

struct obj_oversize_piece {
	uint x          :  8;
	uint y          :  8;
	uint shape      :  2;
	uint size       :  2;
	uint tileoffset : 10;
	uint priority   :  2;
};

struct obj_oversize_formation {
	u32 count;
	struct obj_oversize_piece *data;
};

typedef struct obj obj_t;
typedef struct obj_oversize_formation obj_oversize_formation; // ?

struct obj {
	gpu_sprite              sprite; // a.k.a. OAM
	image_seq              *image_anims;
	gfxentry               *gfx_table;
	rotscale_seq           *rotscale_anims;
	struct proto_t         *template;
	obj_oversize_formation *formations;
	void                  (*callback)(obj_t *);

	coords16 pos_1;
	coords16 pos_2;
	coords8  pos_neg_center;

	u8 anim_number;
	u8 anim_frame;
	u8 anim_delay_countdown; // top two bits used as flags (OBJ_ANIM_ANY_PAUSED)
	u8 anim_unknown_counter;
	u16 priv0; // in npcs: npc_id
	u16 priv1; // in npcs: local id(?)
	u16 priv2; // in npcs: an_sub_index
	u16 priv3; // in npcs: direction supplied to the stepper
	u16 priv4; // in npcs: stepperspeed
	u16 priv5; // in npcs: stepperphase
	u16 priv6;
	u16 priv7;
	u8 bitfield2;
		// 0x01 - in use
		// 0x02 - affected by global_sprite_displace
		// 0x04 - invisible
		// 0x08
		// 0x10
		// 0x20
		// 0x40
		// 0x80
	u8 bitfield;
		// 0x01 - horizontal flip
		// 0x02 - vertical flip
		// 0x04 - set when the image animation is at its beginning
		// 0x08
		// 0x10 - cleared by 0800786C anim_player_2
		// 0x20
		// 0x40 - disable memory allocation (for tiles and palettes)
		// 0x80 - set when the rotscale animation is at its beginning
	u16 anim_data_offset; // ?
	u8 formation_index               : 6;
	u8 formation_determines_priority : 1;
	u8 formation_enabled             : 1;
	u8 y_height_related;
};

// 0202063C
extern struct obj objects[NUM_OBJS];
