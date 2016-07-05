ROM = firered.gba
TOOLCHAIN = arm-none-eabi
OBJDIR = out
PATCHES = $(OBJDIR)/patch.o

LDS        = $(OBJDIR)/out.ldscript
OUT_O      = $(OBJDIR)/out.o
OUT_S      = $(OBJDIR)/out.s
PATCH_JSON = $(OBJDIR)/patch.json
PATCH_O    = $(OBJDIR)/patch.o
OUT_ELF    = out.elf
OUT_GBA    = out.gba


PATCH_CFLAGS =
PATCH_CFLAGS += -ffunction-sections -fno-inline -fomit-frame-pointer -mlong-calls
PATCH_CFLAGS += -mthumb -mcpu=arm7tdmi
PATCH_CFLAGS += -O2 -gdwarf-2 -gstrict-dwarf # vbam forces dwarf version <=2

$(OUT_ELF): $(LDS) $(OUT_O) $(PATCHES)
	$(TOOLCHAIN)-ld -A arm7tdmi --fix-v4bx -o $(OUT_ELF) -T $(LDS) $(OUT_O) $(PATCHES)

$(OUT_GBA): $(OUT_ELF)
	$(TOOLCHAIN)-objcopy -S -O binary $(OUT_ELF) $(OUT_GBA)

$(PATCH_O): patch.c Makefile
	mkdir -p $(OBJDIR)
	$(TOOLCHAIN)-gcc $(PATCH_CFLAGS) -Wno-implicit -c -o $(PATCH_O) patch.c

$(PATCH_JSON): $(PATCH_O)
	rabin2 -Sj $(PATCH_O) > $(PATCH_JSON)

$(OUT_O): $(OUT_S)
	$(TOOLCHAIN)-as -mthumb -o $(OUT_O) $(OUT_S)

$(OUT_S) $(LDS): build.py info.json $(PATCH_JSON) $(ROM)
	python build.py $(ROM) $(PATCH_JSON) $(OUT_S) $(LDS)

clean:
	rm -rf out.gba out.sav ./$(OBJDIR)
	mkdir ./$(OBJDIR)


$(PATCH_O): \
	../bag.c \
	../bag_interface.c \
	../battle_ai.c \
	../battle_ai_trainer.c \
	../battle.c \
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
	../gpu_alloc_pal.c \
	../gpu_alloc_tile.c \
	../gpu.c \
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
	../object_anim.c \
	../object.c \
	../overworld.c \
	../overworld_door.c \
	../overworld_effect_interp.c \
	../overworld_effects_desc.c \
	../overworld_effects_impl.c \
	../overworld_loading.c \
	../overworld_navigation.c \
	../rbox.c \
	../rfu.c \
	../save.c \
	../startmenu.c \
	../task.c \
	../text.c \
	../u0802a_moves.c \
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
	../battle_ai_trainer.h \
	../battle.h \
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
	../flags.h \
	../gpu.h \
	../items.h \
	../lcd_registers.h \
	../main.h \
	../memory.h \
	../move_anim.h \
	../moves.h \
	../npc_cmds.h \
	../npc.h \
	../npc_interp.h \
	../object_anim.h \
	../object.h \
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
	../trainer_tower.h \
	../types.h \
	../u0807d_overworld.h \
	../uncategorized.h \
	../vars.h \
	../whiteout.h \
	../wild_pokemon_encounter.h \
	../wild_pokemon.h
