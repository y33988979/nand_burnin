
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
 . = 0x00000000;

 . = ALIGN(4);
 .text :
 {
  arch/arm/cpu/hi3716mv310/start.o (.text)
  arch/arm/cpu/hi3716mv310/lowlevel_init.o (.text)
  arch/arm/cpu/hi3716mv310/chip.o (.text)
  arch/arm/cpu/hi3716mv310/ddrphy_train.o (.text)
  arch/arm/cpu/hi3716mv310/ddrphy_training.o (.text)
  arch/arm/cpu/hi3716mv310/ddr_dataeye_training.o (.text)
  arch/arm/cpu/hi3716mv310/irq.o (.text)
  arch/arm/cpu/hi3716mv310/uart.o (.text)
  *(.text)
 }
 __text_end = .;
 . = . + 0x84;
 . = ALIGN(4);
 .rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }

 . = ALIGN(4);
 .data : { *(.data) }

 . = ALIGN(4);
 .got : { *(.got) }

 __u_boot_cmd_start = .;
 .u_boot_cmd : { *(.u_boot_cmd) }
 __u_boot_cmd_end = .;

 . = ALIGN(4);
 .image : { *(.image) }

 . = ALIGN(4);
 __bss_start = .;
 .bss : { *(.bss) }
 _end = .;
}
