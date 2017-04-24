#include <config.h>


__blank2_zone_start:
	.incbin "init.reg2"
. = __blank2_zone_start + 0x2000
__blank2_zone_end:

.balignl 16,0xdeadbeef

.globl _blank2_zone_start
_blank2_zone_start:
	.word __blank2_zone_start

.globl _blank2_zone_end
_blank2_zone_end:
	.word __blank2_zone_end
