#include "continuegame.h"
#include "save.h"
#include "vars.h"

#ifndef NO_RAM
u16 var_8000; // 020370B8
u16 var_8001; // 020370BA
u16 var_8002; // 020370BC
u16 var_8003; // 020370BE
u16 var_8004; // 020370C0
u16 var_8005; // 020370C2
u16 var_8006; // 020370C4
u16 var_8007; // 020370C6
u16 var_8008; // 020370C8
u16 var_8009; // 020370CA
u16 var_800A; // 020370CC
u16 var_800B; // 020370CE
u16 var_800D; // 020370D0
u16 var_800F; // 020370D2
u16 var_800C; // 020370D4
u16 var_8010; // 020370D6
u16 var_8011; // 020370D8
u16 var_8012; // 020370DA Current text color
u16 var_8013; // 020370DC Alternative text color
u16 var_8014; // 020370DE

u16 var_800E; // 0203AD30

// 0815FD0C
u16 *var_80xx[] = {
    &var_8000, &var_8001, &var_8002, &var_8003, &var_8004, &var_8005, &var_8006,
    &var_8007, &var_8008, &var_8009, &var_800A, &var_800B, &var_800C, &var_800D,
    &var_800E, &var_800F, &var_8010, &var_8011, &var_8012, &var_8013, &var_8014
};
#endif

// 0806E454
u16 *var_access(u16 nr) {
    if (nr <  0x4000) return (u16*)0;
    if (nr >= 0x8000) return var_80xx[nr-0x8000];
    u8 m = pq_npc_recording;
    if (m == 1) {
        u16 *z;
        if (z = (u16*)sub_8112D40(0, nr))
            saveblock1_mapdata->var_4000[nr-0x3800] = *z;
    } else if (m == 2) {
        if (sub_806E528(nr-0x4000, 1)) {
            *(u16*)0x0300507C = nr-0x4000;
            sub_8112DB0(0, nr, saveblock1_mapdata->var_4000[nr-0x3800]);
        }
    }
    return &saveblock1_mapdata->var_4000[nr-0x3800];
}
