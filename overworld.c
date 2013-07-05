struct mapdata {
    u32 width;
    u32 height;
    u16 *borderblock;
    u16 *data;
    struct tileset *tileset1;
    struct tileset *tileset2;
    u8 bb_width;
    u8 bb_height;
    u16 field_1A;
};

struct map {
    struct mapdata   *data;
    struct mapevents *events;
    struct mapscript *scripts;
    struct mapconn   *connections;
    u16 music;
    u16 mapindex;
    u8 name;
    u8 cave;
    u8 weather;
    u8 light;
    u8 field_18;
    u8 escape_rope
    u8 showname;
    u8 battletype;
}; // 0x1C

typedef u32 block;

struct tileset {
    void *ptr1;
    void *ptr2;
    void *ptr3;
    void *ptr4;
    void (*fptr)(void);
    block *blocks;
}; // 0xC


static inline u16 helper1(s16 x, s16 y) {
    u8  hsize = current_mapheader.data->bb_width;
    u8  vsize = current_mapheader.data->bb_height;
    u16 *grid = current_mapheader.data->borderblock;

    u32 x2 = (hsize<<3)+x-7 % hsize;
    u32 y2 = (vsize<<3)+y-7 % vsize;

    return grid[x + y*hsize] & 0xC00;
}

static inline u16 helper2(s16 x, s16 y) {
    u32 hsize = *(u32* )0x03005040;
    u32 vsize = *(u32* )0x03005044;
    u16 *grid = *(u16**)0x03005048;

    if ((x>=0) && (x<hsize)
     && (y>=0) && (y<vsize))
        return grid[x + y*hsize];
    return helper1(x, y);
}

// 08058D44
u8 sub_8058D44(s16 x, s16 y) {
    u16 blockid = helper2(x, y);
    if (blockid == 0x3FF) return 1;
    return blockid >> 12;
}

// 08058DC4
u8 sub_8058DC4(s16 x, s16 y) {
    u16 blockid = helper2(x, y);
    if (blockid == 0x3FF) return 1;
    return (blockid >> 10) & 0x3;
}

// 08058E48
u16 cur_mapdata_get_blockid_at(s16 x, s16 y) {
    u16 blockid = helper2(x, y);
    if (blockid == 0x3FF)
        blockid = helper1(x, y);
    return blockid & 0x3FF;
}

// 08058F1C
u32 block_get_field(block b, u8 f) {
    // 08352EF0
    const u32 mask[8] = {
        0x000001FF, // role (a.k.a. behaviour byte)
        0x00003E00,
        0x0003C000,
        0x00FC0000,
        0x07000000, // is grass/water tile?
        0x18000000,
        0x60000000,
        0x80000000
    }

    // 08352F10
    const u8 shift[8] = {
        0x0,
        0x9,
        0xE,
        0x12,
        0x18,
        0x1B,
        0x1D,
        0x1F
    };

    if (b>=8) return b;
    return (b & mask[f]) >> shift[f];
}

// 08059080
u32 mapdata_block_get_field(struct mapdata *data, u16 blockid, u8 fieldid) {
    if (blockid >= 0x400) return 0xFF;
    return block_get_field((blockid < 0x280)
        ? data->tileset1->blocks[blockid]
        : data->tileset2->blocks[blockid-0x100],
        fieldid);
}

// 08058F48
u32 cur_mapdata_block_get_field_at(u16 x, u16 y, u8 fieldid) {
    return mapdata_block_get_field(
        current_mapheader.data,
        cur_mapdata_blockid_at(x, y),
        fieldid);
}

// 08058F78
u8 cur_mapdata_block_role_at(u16 x, u16 y) {
    return cur_mapdata_block_get_field_at(x, y, 0);
}

// 0806CE74
u8 cur_mapdata_block_role_at_player_pos() {
    u16 x, y;
    player_get_pos_to(&x, &y);
    return cur_mapdata_block_role_at(x, y);
}

// 0806C888
void sub_806C888(u8 *d) {
    d[0] = 0;
    d[1] &= ~0x1F;
    d[2] = 0;
}

// 0806C8BC
void sub_806C8BC(u8 *d, u16 keypad_new, u16 keypad_held) {
    u8 role = cur_mapdata_block_role_at_player_pos();
    bool override = is_tile_that_overrides_player_control(role);

    if (!script_env_2_context_is_normal() && is_x03000FA0_eq_1())
        sub_806CA4C(d, keypad_new, keypad_held);

    if ((running1 == 2 && !override) || running1 == 0) {
        // TODO
        if ((byte_203ADFA-2) >= 2 && keypad_held & 0xF0) // any direction
            d[0] |= 0x30;
    }
    if (!override) {
        if (running1 == 2 && running2 == 2)
            d[0] |= 0x40;
        if (running1 == 2)
            d[0] |= 0x02;
    }
    // TODO
    if ((byte_203ADFA-2) >= 2) {
             if (keypad_held & 0x40) d[2] = 2; // up
        else if (keypad_held & 0x80) d[2] = 1; // down
        else if (keypad_held & 0x20) d[2] = 3; // left
        else if (keypad_held & 0x10) d[2] = 4; // right
    }
}

// 0203AE8C
void (*funcptr_203AE8C)();

// 0811092C
void call_203AE8C() {
    if (funcptr_203AE8C)
        funcptr_203AE8C();
}

// 0805644C
void sub_805644C(u16 keypad_new, u16 keypad_held) {
    sub_8112B3C();
    sub_805BEB8();

    u8 d[4];
    sub_806C888(d);
    sub_806C8BC(d, keypad_new, keypad_held);
    sub_806CD30(d);
    if (!script_env_2_is_enabled()) {
        if (sub_806CAC8(d)) {
            sub_805B3E0(walkrun.running2, keypad_new, keypad_held);
        } else {
            script_env_2_enable();
            sub_80982EC();
        }
    }
    call_203AE8C();
}
