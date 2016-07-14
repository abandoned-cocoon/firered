ROM = firered.gba
TOOLCHAIN = arm-none-eabi
OBJDIR = out
PATCHES = $(OBJDIR)/patch.o #$(OBJDIR)/patch_asm.o

LDS        = $(OBJDIR)/out.ldscript
INC_S      = $(OBJDIR)/inc.s
INC_O      = $(OBJDIR)/inc.o
PATCH_JSON = $(OBJDIR)/patch.json
PATCH_O    = $(OBJDIR)/patch.o
ASM_O      = $(OBJDIR)/patch_asm.o
OUT_ELF    = out.elf
OUT_GBA    = out.gba


PATCH_CFLAGS =
PATCH_CFLAGS += -ffunction-sections -fomit-frame-pointer -mlong-calls #-fno-inline
PATCH_CFLAGS += -nostdlib -mthumb -mcpu=arm7tdmi -mabi=apcs-gnu
PATCH_CFLAGS += -O2 -gdwarf-2 -gstrict-dwarf # vbam forces dwarf version <=2

ASFLAGS = -meabi=gnu

$(OUT_GBA): $(OUT_ELF)
	$(TOOLCHAIN)-objcopy -S -O binary $(OUT_ELF) $(OUT_GBA)

$(OUT_ELF): $(LDS) $(INC_O) $(PATCHES)
	$(TOOLCHAIN)-ld -A arm7tdmi -o $(OUT_ELF) -T $(LDS) $(INC_O) $(PATCHES)

$(ASM_O): ../helpers.s ../overworld_effects_desc.s
	$(TOOLCHAIN)-as $(ASFLAGS) -o $(ASM_O) ../helpers.s ../overworld_effects_desc.s

$(PATCH_O): patch.c Makefile
	mkdir -p $(OBJDIR)
	$(TOOLCHAIN)-gcc $(PATCH_CFLAGS) -Wno-implicit -c -o $(PATCH_O) patch.c

$(PATCH_JSON): $(PATCH_O)
	rabin2 -Sj $(PATCH_O) > $(PATCH_JSON)

$(INC_O): $(INC_S)
	$(TOOLCHAIN)-as $(ASFLAGS) -mthumb -o $(INC_O) $(INC_S)

$(INC_S) $(LDS): build.py info.json $(PATCH_JSON) $(ROM)
	python build.py $(ROM) $(PATCH_JSON) $(INC_S) $(LDS)

clean:
	rm -rf out.gba out.sav ./$(OBJDIR)
	mkdir ./$(OBJDIR)


$(PATCH_O): \
	../bag.c \
	../bag_interface.c \
	../battle.c \
	../battle_ai.c \
	../battle_ai_trainer.c \
	../battle_cmds.c \
	../battle_config.c \
	../battle_interface.c \
	../battle_rpc_recv.c \
	../battle_rpc_send.c \
	../choice.c \
	../continuegame.c \
	../ctxmenu.c \
	../dialog.c \
	../environment.c \
	../fade.c \
	../fbox.c \
	../fishing.c \
	../flash.c \
	../gpu.c \
	../gpu_alloc_pal.c \
	../gpu_alloc_tile.c \
	#../helpers.s \
	../helpsystem.c \
	../hm.c \
	../intro.c \
	../main.c \
	../mevent_server.c \
	../move_anim.c \
	../move_cmds.c \
	../move_etc.c \
	../move_interp.c \
	../npc.c \
	../npc_cmds.c \
	../npc_interp.c \
	../object.c \
	../object_anim.c \
	../overworld.c \
	../overworld_door.c \
	../overworld_effect_interp.c \
	../overworld_effects_desc.s \
	../overworld_effects_impl.c \
	../overworld_loading.c \
	../overworld_navigation.c \
	../rbox.c \
	#../rfu.c \
	../save.c \
	../startmenu.c \
	../task.c \
	../text.c \
	../u0802a_moves.c \
	../u08044.c \
	../u08078_intro.c \
	../u0807d_overworld.c \
	../u08086.c \
	../u080f6_gpu.c \
	../u08150_rbox.c \
	../uncategorized.c \
	../vars.c \
	../whiteout.c \
	../wild_pokemon_encounter.c \
	../audio.h \
	../bag.h \
	../battle.h \
	../battle_ai_trainer.h \
	../battle_config.h \
	../battle_interface.h \
	../battle_intro.h \
	../battle_rpc.h \
	../battle_rpc_recv.impl.h \
	../choice.h \
	../continuegame.h \
	../ctxmenu.h \
	../dialog.h \
	../engine_scripts.h \
	../environment.h \
	../evolution.h \
	../fade.h \
	../fbox.h \
	../flags.h \
	../gpu.h \
	../helpsystem.h \
	../hm.h \
	../items.h \
	../lcd_registers.h \
	../main.h \
	../memory.h \
	../move_anim.h \
	../move_etc.h \
	../move_interp.h \
	../moves.h \
	../mplay_stuff.h \
	../npc.h \
	../npc_cmds.h \
	../npc_interp.h \
	../object.h \
	../object_anim.h \
	../overworld.h \
	../overworld_effects.h \
	../overworld_navigation.h \
	../pokedude.h \
	../pokemon.h \
	../rbox.h \
	../rfu.h \
	../roaming_pokemon.h \
	../save.h \
	../startmenu.h \
	../task.h \
	../text.h \
	../trainer_tower.h \
	../types.h \
	../u08044.h \
	../u08078_intro.h \
	../u0807d_overworld.h \
	../uncategorized.h \
	../vars.h \
	../whiteout.h \
	../wild_pokemon.h \
	../wild_pokemon_encounter.h
