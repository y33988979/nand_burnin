################################################################################
#    Create By Czyong
################################################################################
include $(TOPDIR)/config.mk

################################################################################

PWD           = $(shell pwd)
CROSS_COMPILE ?= arm-hisiv200-linux-
TOPDIR        =
BINIMAGE      = $(TOPDIR)/fastboot-burn.bin

################################################################################
CC       := $(CROSS_COMPILE)gcc
AR       := $(CROSS_COMPILE)ar
LD       := $(CROSS_COMPILE)ld
OBJCOPY  := $(CROSS_COMPILE)objcopy

################################################################################
BOOT     := mini-boot
TEXTBASE := 0x80800000

CFLAGS   := -g -Os -fno-strict-aliasing -fno-common -ffixed-r8 \
	-D__KERNEL__ -DTEXT_BASE=$(TEXTBASE) \
	-I$(TOPDIR)/include  \
	-fno-builtin -ffreestanding \
	-pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux  \
	-mno-thumb-interwork -march=armv7-a $(MKFLAGS)

################################################################################
START := start.o
COBJS := chip.o \
	lowlevel_init.o ddrphy_train.o \
	startup.o cache-cp15.o uart.o\
	mmu.o image_data.o irq.o \
	ddrphy_training.o ddrphy_training_print.o \
	ddr_dataeye_training.o hi3716mv310_ddr_training.o

SSRC  := arch/arm/cpu/hi3716mv310/start.S \
	arch/arm/cpu/hi3716mv310/chip.c \
	arch/arm/cpu/hi3716mv310/irq.S \
	arch/arm/cpu/hi3716mv310/lowlevel_init.c \
	arch/arm/cpu/hi3716mv310/ddrphy_train.S \
	arch/arm/cpu/hi3716mv310/ddrphy_training_print.S \
	arch/arm/cpu/hi3716mv310/ddrphy_training.c \
	arch/arm/cpu/hi3716mv310/ddr_dataeye_training.c \
	arch/arm/cpu/hi3716mv310/ddr_dataeye_training.h \
	arch/arm/cpu/hi3716mv310/hi3716mv310_ddr_training.c \
	arch/arm/cpu/hi3716mv310/hi3716mv310_ddrphy_train.S \
	arch/arm/cpu/hi3716mv310/uart.S \
	arch/arm/lib/mmu.S \
	arch/arm/lib/cache-cp15.c \
	lib/unlzma.c

REG   := $(wildcard $(TOPDIR)/*.reg $(TOPDIR)/.reg)
SRC   := $(notdir $(SSRC))

################################################################################
.PHONY: $(BOOT).bin
$(BOOT).bin: $(BOOT).tmp regfile
	@dd if=./$(BOOT).tmp of=./tmp1 bs=1 count=64 2>/dev/null
	@dd if=$(REG) of=./tmp2 bs=8192 conv=sync 2>/dev/null
	@dd if=./$(BOOT).tmp of=./tmp3 bs=1 skip=8256 2>/dev/null
	@cat tmp1 tmp2 tmp3 > $(BOOT).bin
	@rm -f tmp1 tmp2 tmp3
	@chmod 754 $(BOOT).bin
	@cp -fv $@ $(TOPDIR)
	@echo $(BOOT).bin is Ready.

$(BOOT).tmp: $(BOOT).elf
	$(OBJCOPY) -O srec $< $(BOOT).srec
	$(OBJCOPY) -j .text -O binary $< $(BOOT).text
	$(OBJCOPY) --gap-fill=0xff -O binary $< $@

$(BOOT).elf: image_data.lzma $(SRC) $(START) $(COBJS)
	$(LD) -Bstatic -T mini-boot.lds -Ttext $(TEXTBASE) $(START) \
		$(COBJS) -Map $(BOOT).map -o $@

.PHONY: regfile
regfile:
	@if [ "$(words $(REG))" = "0" ]; then ( \
		echo '***' Need '.reg' or '*.reg' file in directory $(TOPDIR); \
		exit 1; \
	) fi
	@if [ "$(words $(REG))" != "1" ]; then ( \
		echo '***' Found multi '.reg' or '*.reg' file in directory $(TOPDIR); \
		echo '***' Files: $(notdir $(REG)); \
		exit 1; \
	) fi

################################################################################
start.o: start.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c

image_data.lzma: $(BINIMAGE)
	lzma -fkzc -7 $< > $@

%.o: %.c
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes \
		-fno-stack-protector -o $@ $< -c

%.o: %.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c

image_data.o: image_data.S image_data.lzma
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c

$(SRC):
	ln -sf ../../../../../$(filter %/$@,$(SSRC)) $@

################################################################################
TMPS := $(COBJS) start.o $(SRC) \
	$(BOOT).map $(BOOT).elf $(BOOT).srec $(BOOT).bin \
	$(BOOT).tmp $(BOOT).text image_data.lzma

distclean: clean

clean:
	@rm -f $(TMPS)

################################################################################
.PHONY: clean
################################################################################
