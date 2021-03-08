/* taken from https://github.com/makerbase-mks/MKS-Robin-E3-E3D/tree/master/firmware/marlin2.0%20for%20Ender3/buildroot/share/PlatformIO/ldscripts/mks_robin_e3.ld */

MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 64K - 40
  rom (rx)  : ORIGIN = 0x08007000, LENGTH = 512K - 28K
}

/* Provide memory region aliases for common.inc */
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

/* Let common.inc handle the real work. */
INCLUDE common.inc
