struct help_top_item {
	char *label;
	u32 number;
};

// CONTEXT NUMBERS
//
// see 08078FD6
//  0x1: on titlescreen
// see 0812EC0E
//  0x2: in tutorial
// see 0806F67C
//  0xC: saving the game
// see 0812B35C
//  0xE: at home in pallet town
//  0xF: in oaks lab
//  0x10: in pokecenter
//  0x11: in pokemart
//  0x12: in gym
//  0x13: elsewhere with maplight 0x8 or 0x9
//  0x14: places without maplight 0x8 or 0x9
//  0x15: cave or forest (see 0812B2C4)
//  0x16: walkrun.bitfield & 0x8
// see 0800FD9C
//  0x17: in normal wild battle
//  0x18: in trainer battle
//  0x19: in double trainer battle
//  0x1A: in safari battle


u16 help_system_context; // 0203B0EC
u16 help_system_context_backup; // 03005EA0
u8 help_system_enabled; // 03005ECC

// 0845B080
char *help_top_item_labels[6] = {
	"What should I do in this situation?",
	"How do I do this?",
	"What does this term mean?",
	"About this game",
	"Type Matchup List",
	"EXIT"
};

// CONTEXT 0xE (example)

	// 0845B6BF
	u8 help_0E_0[] = {
		2, // What should I be doing?
		3, // I can't get out of a room.
		4, // I can't find the person I want.
		0xFF
	};

	// 0845B6C3
	u8 help_0E_3[] = {
		1, // The HELP System
		2, // The game
		3, // Wireless Adapter
		7, // What are Pokemon
		0xFF
	};

// 0845C4B0
u8 help_top_item_permutation[6] = {
	3, 0, 1, 2, 4, 5
};

// 0845C4B6
u8 help_top_item_visibility[36][6] = {
	{0, 0, 0, 0, 0, 1}, // 0x00
	{0, 0, 0, 1, 0, 1}, // 0x01
	{0, 0, 0, 1, 0, 1}, // 0x02
	{0, 1, 0, 1, 0, 1}, // 0x03
	{0, 1, 0, 0, 0, 1}, // 0x04
	{0, 1, 1, 0, 0, 1}, // 0x05
	{0, 1, 1, 0, 0, 1}, // 0x06
	{0, 0, 1, 0, 0, 1}, // 0x07
	{0, 0, 1, 0, 0, 1}, // 0x08
	{0, 1, 1, 0, 0, 1}, // 0x09
	{0, 1, 1, 0, 0, 1}, // 0x10
	{0, 1, 0, 0, 0, 1}, // 0x11
	{0, 1, 1, 0, 0, 1}, // 0x12
	{0, 1, 1, 0, 0, 1}, // 0x13
	{1, 0, 0, 1, 0, 1}, // 0x14
	{1, 1, 1, 0, 0, 1}, // 0x15
	{1, 1, 1, 1, 0, 1}, // 0x16
	{1, 1, 1, 0, 0, 1}, // 0x17
	{1, 1, 1, 0, 1, 1}, // 0x18
	{1, 1, 1, 0, 0, 1}, // 0x19
	{1, 1, 1, 0, 0, 1}, // 0x20
	{1, 1, 1, 0, 0, 1}, // 0x21
	{1, 1, 1, 0, 0, 1}, // 0x22
	{1, 1, 1, 0, 1, 1}, // 0x23
	{1, 1, 1, 0, 1, 1}, // 0x24
	{1, 1, 1, 0, 1, 1}, // 0x25
	{1, 1, 1, 0, 1, 1}, // 0x26
	{0, 1, 0, 0, 0, 1}, // 0x27
	{0, 1, 0, 0, 0, 1}, // 0x28
	{0, 1, 0, 0, 0, 1}, // 0x29
	{0, 1, 0, 0, 0, 1}, // 0x30
	{0, 1, 0, 0, 0, 1}, // 0x31
	{0, 1, 0, 0, 0, 1}, // 0x32
	{0, 1, 0, 0, 0, 1}, // 0x33
	{0, 1, 0, 0, 0, 1}, // 0x34
	{0, 0, 0, 0, 0, 0}, // 0x35
};

// 0812B220
void help_context_by_var_8004__sp17D() {
	help_system_context = var_8004;
}

// 0812B234
void help_system_context_backup__sp17E() {
	help_system_context_backup = help_system_context;
}

// 0812B248
void help_system_context_restore__sp17F() {
	help_system_context = help_system_context_backup;
}

// 0812B478
void help_system_disable__sp198() {
	help_system_enabled = true;
}

// 0812B484
void help_system_enable_unless_flashback__sp199() {
	if (prev_quest_mode >= 2 && prev_quest_mode < 4)
		return;
	help_system_enabled = true;
}

// 0812B520
void sub_0812B520(u16 *arg, struct help_top_item *items) {
	u32 count = 0;
	u8 *vis = help_top_item_visibility[help_system_context];
	for (int i=0; i<6; i++) {
		u8 j = help_top_item_permutation[i];
		if (vis[j] == 1) {
			items[count].label = help_top_item_labels[j];
			items[count].number = j;
			count++;
		} 
	}
	items[count-1].number = -2; // EXIT
	arg[2] = count;
	arg[3] = count;
	arg[4] = 0;
}