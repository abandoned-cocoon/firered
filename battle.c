struct battle_data {
	u16  species;
	u16  stats[5];
	u16  move_ids[4];
	u32  ivs;
	u8   stat_buffs[8]; // hp/atk/def/spd/spatk/spdef/accuracy/evasion
		// min 0 -- 6 -- 12 max
	u8   ability_id;
	u8   type1;
	u8   type2;
	u8   padding;
	u8   current_pp[4];
	u16  current_hp;
	u8   level;
	u8   happiness;
	u16  max_hp;
	u16  held_item;
	char name[11];
	u8   field_3B;
	char trainer_name[8];
	u32  padding3;
	u32  pid;
	u32  status1;
		// 0x00000007  -> Sleep (up to 7 turns)
		// 0x00000008  -> poison
		// 0x00000010  -> burn
		// 0x00000020  -> freeze
		// 0x00000040  -> paralyze
		// 0x00000080  -> badly poisoned
	u32  status2;
		// see http://www.pokecommunity.com/archive/index.php/t-215644.html

		// 0x00000007 -> confusion: 7 turns max, decreases each turn before use. snaps out when 1->0)
		// 0x00000008 -> flinching
		// 0x00000070 -> uproar
		// 0x00000f00 -> charging energy (bide) when 1, spit all energy. 3 total, as each flag is a different state
		 
		// 0x00001000 -> multiple attack over several turns.
		// This one is very important, as it is used by any attack that has two turns.
		// From Rollout to dig, solarbeam to petal dance, any attack that forces the user to use it a second turn (skip attack selection) sets this flag.
		// The attack must then know how to deal with it, and unset it when time is up.
		 
		// 0x0000e000 -> multiple attack (warp) target by up to 7 turns.
		// This is the target's side, so its the timer for the one suffering from the attack. 
		// 0x000f0000 -> attraction, infatuation.
		// The bit indicates the slot of who is the target. 
		// 1 is your pokemon,2 is opponent, 3rd bit is your second out pokemon, 4th is your opponent 2nd pokemon out.
		// ex: 0x000a0000 means the pokemon is in love with both of your opponent pokemon. The one sho
	u32  otid;
};

u8  b_buffer_A[4][0x200]; // 02022BC4
u8  b_buffer_B[4][0x200]; // 020233C4

u8  b_active_side; // 02023BC4
u32 b_buffers_awaiting_execution_bitfield; // 02023BC8
u8  b_num_active_sides;

u16 b_pokemon_team_id_by_side[4]; // 02023BCE
u8  b_side_status[4]; // 02023BD6
	// bit 0x01: is enemy pokemon
	// val 0xff: is dead

struct battle_data b_data[4]; // 02023BE4
u8 b_oamid[NUM_BATTLE_SIDES]; // 02023D44

// 0800D30C
void b_setup_bx() {
	if (battle_type_flags & BATTLE_LINK)
		b_setup_bx_link();
	else
		b_setup_bx_local();

	sub_800D768();

	if (battle_type_flags & BATTLE_DOUBLE_2)
		for (u32 i=0; i<b_num_active_sides; i++)
			sub_8127DA8(i, 0);

}

// 0800D364
void b_setup_bx_local() {
	if (battle_type_flags & BATTLE_DOUBLE) {
		bc = &bc_080123C0;

		b_side_status[0] = 0;
		b_side_status[1] = 1;
		b_side_status[2] = 2;
		b_side_status[3] = 3;
		b_num_active_sides = 4;

		if (battle_type_flags & BATTLE_POKE_DUDE) {
			bx[0] = &bx_goto_buffer_A_ch0_tbl5;
			bx[1] = &bx_goto_buffer_A_ch0_tbl5;
			bx[2] = &bx_goto_buffer_A_ch0_tbl5;
			bx[3] = &bx_goto_buffer_A_ch0_tbl5;
		} else {
			bx[0] = &bx_goto_buffer_A_ch0_tbl1;
			bx[1] = &bx_goto_buffer_A_ch0_tbl2;
			bx[1] = &bx_goto_buffer_A_ch0_tbl1;
			bx[1] = &bx_goto_buffer_A_ch0_tbl2;
		}

	} else {
		bc = &bc_080123C0;

		b_side_status[0] = 0;
		b_side_status[1] = 1;
		b_num_active_sides = 2;

		if (battle_type_flags & BATTLE_POKE_DUDE) {
			bx[0] = &bx_goto_buffer_A_ch0_tbl5;
			bx[1] = &bx_goto_buffer_A_ch0_tbl5;
		} else if (battle_type_flags & BATTLE_SAFARI) {
			bx[0] = &bx_goto_buffer_A_ch0_tbl6;
			bx[1] = &bx_goto_buffer_A_ch0_tbl2;
		} else if (battle_type_flags & (BATTLE_OAK_COMMENTS|0x200)) {
			bx[0] = &bx_goto_buffer_A_ch0_tbl3;
			bx[1] = &bx_goto_buffer_A_ch0_tbl2;
		} else {
			bx[0] = &bx_goto_buffer_A_ch0_tbl1;
			bx[1] = &bx_goto_buffer_A_ch0_tbl2;
		}
	}
}

// 080123E4
void c1_battle_exec_bc_and_bx() {
	(*bc)();
	for (b_active_side = 0; b_active_side < b_num_active_sides; b_active_side++) {
		(*bx[b_active_side])();
	}
}
