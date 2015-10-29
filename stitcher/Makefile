ROM = firered.gba
TOOLCHAIN = arm-none-eabi
OBJDIR = obj
PATCHES = $(OBJDIR)/patch.o

LDS        = $(OBJDIR)/out.ldscript
OUT_O      = $(OBJDIR)/out.o
OUT_S      = $(OBJDIR)/out.s
PATCH_JSON = $(OBJDIR)/patch.json
PATCH_O    = $(OBJDIR)/patch.o
OUT_ELF    = out.elf
OUT_GBA    = out.gba


# vbam forces dwarf version <=2

PATCH_CFLAGS = -gdwarf-2 -gstrict-dwarf -ffunction-sections -fomit-frame-pointer -O2 -mthumb -mcpu=arm7tdmi

$(OUT_ELF): $(LDS) $(OUT_O) $(PATCHES)
	$(TOOLCHAIN)-ld -o $(OUT_ELF) -T $(LDS) $(OUT_O) $(PATCHES)

$(OUT_GBA): $(OUT_ELF)
	$(TOOLCHAIN)-objcopy -O binary $(OUT_ELF) $(OUT_GBA)

$(PATCH_O): patch.c Makefile
	mkdir -p obj
	$(TOOLCHAIN)-gcc $(PATCH_CFLAGS) -c -o $(PATCH_O) patch.c

$(PATCH_JSON): $(PATCH_O)
	rabin2 -Sj $(PATCH_O) > $(PATCH_JSON)

$(OUT_O): $(OUT_S)
	$(TOOLCHAIN)-as -o $(OUT_O) $(OUT_S)

$(OUT_S) $(LDS): build.py info.json $(PATCH_JSON) $(ROM)
	python build.py $(ROM) $(PATCH_JSON) $(OUT_S) $(LDS)

clean:
	rm -rf out.gba out.sav ./$(OBJDIR)
	mkdir ./$(OBJDIR)
