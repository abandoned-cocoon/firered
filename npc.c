#define MAX_NPCS 0x10

struct npc_position {
    u16 x, y;
    u8 height;
};

// 0805C6C4
u8 player_get_direction() {
    return npc_states[walkrun.npcid].direction & 0xF;
}

// 0805C700
u8 player_get_height() {
    return npc_states[walkrun.npcid].height >> 4;
}

// 0805C538
void player_get_pos_to(u16 x, u16 y) {
    *x = npc_states[walkrun.npcid].y;
    *y = npc_states[walkrun.npcid].y;
}   

// 0806CE20
void player_get_pos_to_and_height(struct npc_position *n) {
    player_get_pos_to(&n->x, &n->y);
    n->height = player_get_height();
}

// 0805DF60
u8 npc_id_by_local_id(u8 local_id, u8 mapnr, u8 mapgroup) {
    if (local_id == 0xFF)
        return npc_id_by_local_id_ignore_map(local_id);
    else
        return npc_id_by_local_id_and_map(local_id, mapnr, mapgroup);
}

// 0805DF84
bool npc_id_by_local_id_and_map_ret_success(u8 local_id, u8 mapnr, u8 mapgroup, u8 *result) {
    return (*result=npc_id_by_local_id(local_id, mapnr, mapgroup)) == MAX_NPCS;
}

// 0805DFF4
u8 npc_id_by_local_id_and_map(u8 local_id, u8 mapnr, u8 mapgroup) {
    u8 i=0;
    while (i<MAX_NPCS) {
        if (npc_states[i].bits & 1 && \
            npc_states[i].local_id       == local_id && \
            npc_states[i].local_mapnr    == local_mapnr && \
            npc_states[i].local_mapgroup == local_mapgroup) break;
        i++;
    }
    return i;
}

// 0805E044
u8 npc_id_by_local_id_ignore_map(u8 local_id) {
    u8 i=0;
    while (i<MAX_NPCS) {
        if (npc_states[i].bits & 1 && \
            npc_states[i].local_id == local_id) break;
        i++;
    }
    return i;
}

// 0805E4B4
void npc_hide(struct npc_state *npc) {
    npc->bits &= ~2; // clear bit 2
    sub_0805E510(npc); // TODO
}

// 0805E4C8
void hide_sprite(u8 local_id, u8 mapnr, u8 mapgroup) {
    // TODO: Find better name. Check correctness of this transcription.
    u8 npc_id;
    if (!npc_id_by_local_id_and_map_ret_success(local_id, mapnr, mapgroup, &npc_id)) {
        flag_set(trainerid_by_npc_id(npc_id));
        npc_hide(&npc_states[npc_id]);
    }
}

// 0805FC5C
u16 trainerid_by_local_id_and_map(u8 local_id, u8 mapnr, u8 mapgroup) {
    struct rom_npc *rnpc = rom_npc_by_nr_and_map(local_id, mapnr, mapgroup);
    return rnpc->trainerid;
}

// 0805FC74
u16 trainerid_by_npc_id(u8 npc_id) {
    struct npc_state *npc = &npc_states[npc_id];
    return trainerid_by_local_id_and_map(npc->local_id, npc->local_mapnr, npc->local_mapgroup);
}

// 0805FD5C
struct rom_npc *rom_npc_by_nr_and_map(u8 local_id, u8 mapnr, u8 mapgroup) {
    // To be written.
}

// 0806CEA0
bool onpress_a(struct npc_position *n, u8 tt, u8 d) {
    // tt = tile type
    // d = direction in which the player is looking

    u8 *scr = (u8*)onpress_a_get_script(n, tt, d);
    if (!scr) return false;
    if (scr != scr_pc1 && scr != scr_pc2) sound_play(5);
    script_start_1(scr);
    return true;
}

// 0806CEE0
u8 *onpress_a_get_script(struct npc_position *n, u8 tt, u8 d) {
    u32 scr;
    if (scr=onpress_a_get_script_npc     (n, tt, d)) return scr;
    if (scr=onpress_a_get_script_signpost(n, tt, d)) return scr;
    if (scr=onpress_a_get_script_tile    (n, tt, d)) return scr;
    if (scr=surf(n, tt, d)) return scr;
    return 0;
}

// 0806CFF4
u32 onpress_a_get_script_npc(struct npc_position *n, u8 tt, u8 d) {
    // tt = tile type
    // d = direction in which the player is looking

    u8 npc_id = npc_at_pos_and_height(
        n->x,
        n->y,
        n->height
    );
    if (npc_id == MAX_NPCS || npc_states[npc_id].local_id == 0xFF) {
        if (!is_tile_x80(tt)) return 0;

        npc_id = npc_at_pos_and_height(
            n->x + directions_i32[d*2+0],
            n->y + directions_i32[d*2+1],
            n->height
        );
        if (npc_id == MAX_NPCS || npc_states[npc_id].local_id == 0xFF) return 0;
    }
    if (in_trade_center() && !sub_08063D68(&npc_states[npc_id])) return 0;

    scripting_npc = npc_id;
    var_800F = npc_states[npc_id].local_id;
    var_800C = d;
    return sub_08069D8C(var_800F, npc_get_script_by_npc_id(npc_id));
}

// 0806D1F0
u32 onpress_a_get_script_tile(struct npc_position *n, u8 tt, u8 d) {
    // tt = tile type
    // d = direction in which the player is looking

    var_800C = d;
    // tile types -> scripts

    if (is_tile_x83(tt)) return 0x081A6955; // (complex stuff. probably for the PC)
    if (is_tile_x85(tt)) return 0x081A6C32; // (complex stuff)
    if (is_tile_x81(tt)) return 0x081A7606; // It`s crammed full of POKeMON books.
    if (is_tile_x82(tt)) return 0x081A760F; // Wow! Tons of POKeMON stuff!
    if (is_tile_x90(tt)) return 0x081A7618; // It should be packed with all kinds of delicious things to eat!
    if (is_tile_xA0(tt)) return 0x081A7633; // It`s a machine of some sort. It sure is impressive.
    if (is_tile_x93(tt)) return 0x081A763C; // It`s a blueprint of some sort. It`s filled with diagrams and text.
    if (is_tile_xA1(tt)) return 0x081A7621; // It`s the latest video game! It sure looks fun!
    if (is_tile_xA2(tt)) return 0x081A7645; // There are obvious signs of burglary here|
    if (is_tile_x97(tt)) return 0x081A762A; // It`s all complicated words and numbers that make no sense|
    if (is_tile_xA3(tt)) return 0x081C549C; // (complex stuff)
    if (is_tile_x86(tt)) return 0x081A764E; // There`s a POKeMON on TV! It looks like it`s having fun.
    if (is_tile_x89(tt)) return 0x081A7657; // Dishes and plates are neatly lined up.
    if (is_tile_x8A(tt)) return 0x081A7660; // It smells delicious! Somebody`s been cooking here.
    if (is_tile_x8B(tt)) return 0x081A7669; // It`s a nicely made dresser. It will hold a lot of stuff.
    if (is_tile_x8C(tt)) return 0x081A7672; // There`s a pile of snacks here.
    if (is_tile_x94(tt)) return 0x081A767B; // It`s a pretty picture of a POKeMON. It looks like it`s feeling good.
    if (is_tile_x95(tt)) return 0x081A7684; // What could this machine be? Better not mess around with it!
    if (is_tile_x96(tt)) return 0x081A768D; // It`s a telephone. Better not use it.
    if (is_tile_x98(tt)) return 0x081A7696; // It`s an advertising poster about all kinds of products.
    if (is_tile_x99(tt)) return 0x081A769F; // Oh, that smells tasty! It might get the stomach growling.
    if (is_tile_x9A(tt)) return 0x081A76A8; // Inside this¦ ¦¦_There`s nothing here!
    if (is_tile_x9B(tt)) return 0x081A76B1; // It`s a cup with a POKeMON mark on it.
    if (is_tile_NO1(tt)) return 0x081A76BA; // The window is very well polished.
    if (is_tile_NO2(tt)) return 0x081A76C3; // Outside the window¦_The sky looks fantastically beautiful.
    if (is_tile_x9E(tt)) return 0x081A76CC; // Lights in different colors are flashing on and off.
    if (is_tile_x9F(tt)) return 0x081A76D5; // All sorts of tools are lined up neatly.
    if (is_tile_x8D_and_d_is_x2(tt, d)) return 0x081BBFD8; // complex stuff.
    if (is_tile_x8F(tt)) return 0x081A7702; // complex stuff.
    if (is_tile_x8E_and_d_is_x2(tt, d)) return 0x081BB8A7; // complex stuff.
    if (is_tile_x91(tt)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76F0; // INDIGO PLATEAU_The ultimate goal of TRAINERS! POKeMON LEAGUE HQ
    }
    if (is_tile_x92(tt)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76F9; // INDIGO PLATEAU_The highest POKeMON authority! POKeMON LEAGUE HQ
    }
    if (is_tile_x88_and_d_is_x2(tt, d)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76DE; // All your item needs fulfilled! POKeMON MART
    }
    if (is_tile_x87_and_d_is_x2(tt, d)) {
        (*(u8*)03000FA1) = 1;
        return 0x081A76E7; // Heal Your POKeMON! POKeMON CENTER
    }
    return 0;
}

// 0826D2D8
i32 directions_i32[9*2] = {
     0,  0,
     0,  1,
     0, -1,
    -1,  0,
     1,  0,
    -1,  1,
     1,  1,
    -1, -1,
     1, -1
};
