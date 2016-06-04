/*typedef unsigned short u16;

struct npc_state {
	char padding[16];
	struct coord {
		u16 x;
		u16 y;
	} to, from;
};

// 0x0805F700
void npc_coords_shift(struct npc_state *n, u16 x, u16 y) {
	n->from.x = n->to.x;
	n->from.y = n->to.y;
	n->to.x = x;
	n->to.y = y;
}*/


// Definitions to ease porting stuff from the db over
#define mapdata_header data
#define saveblock1_mapdata sav1
#define saveblock2_trainerdata sav2
#define saveblock3_boxdata sav3

#define NO_RAM
#include "../gpu.c"
//#include "../overworld.c"
#include "../task.c"
