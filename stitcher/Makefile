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


$(PATCH_O): ../gpu.c ../task.c ../overworld.c
