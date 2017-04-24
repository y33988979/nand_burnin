

/*
 *  SDK Version
 * 
 */
#define SDK_VERSION_STRING	"HiSTBLinuxV100R004C00SPC024"


/*
 *  nand flash support
 * 
 */
#define CONFIG_NAND_SUPPORT	y


/*
 *  SPI_NAND flash support
 *  #define CONFIG_SPI_NAND_SUPPORT
 */


/*
 *  spi flash support
 *  #define CONFIG_SPIFLASH_SUPPORT
 */


/*
 *  eth support
 * 
 */
#define CONFIG_NET_SUPPORT	y


/*
 *  eth upport phy address
 * 
 */
#define HISFV_PHY_U	2


/*
 *  eth downport phy address
 * 
 */
#define HISFV_PHY_D	1


/*
 *  emmc flash support
 *  #define CONFIG_EMMC_SUPPORT
 */


/*
 *  usb support
 *  #define CONFIG_USB_SUPPORT
 */


/*
 *  fat filesystem support
 *  #define CONFIG_FAT_SUPPORT
 */


/*
 *  usb2eth support
 *  #define CONFIG_USB_HOST_ETHER
 */


/*
 *  boot environment variables start address
 * 
 */
#define CFG_ENV_ADDR	0xC0000


/*
 *  boot environment variables size
 * 
 */
#define CFG_ENV_SIZE	0x20000


/*
 *  boot environment variables range
 * 
 */
#define CFG_ENV_RANGE	0x20000


/*
 *  Bakup boot environment variables support
 *  #define CONFIG_ENV_BACKUP
 */


/*
 *  Bakup boot environment variables start address
 *  #define CFG_ENV_BACKUP_ADDR
 */


/*
 *  build product with boot
 * 
 */
#define CONFIG_PRODUCT_WITH_BOOT	y


/*
 *  macro for hi3712
 * 
 */
#define hi3712	1


/*
 *  macro for hi3715
 * 
 */
#define hi3715	2


/*
 *  macro for hi3716c
 * 
 */
#define hi3716c	3


/*
 *  macro for hi3716h
 * 
 */
#define hi3716h	4


/*
 *  macro for hi3716m
 * 
 */
#define hi3716m	5


/*
 *  macro for hi3716mv310
 * 
 */
#define hi3716m310	6


/*
 *  macro for hi3716mv320
 * 
 */
#define hi3716m320	7


/*
 *  macro for hi3110ev500
 * 
 */
#define hi3110ev500	8


/*
 *  chip type
 * 
 */
#define CONFIG_CHIP_TYPE	hi3716mv310


/*
 *  boot regfile name
 * 
 */
#define CONFIG_BOOT_REG_NAME	hi3716m31dma_hi3716mv310_ddr3_512Mbyte_16bit_2layers_nand.reg


/*
 *  platform hi3712
 *  #define CONFIG_HI3712_PLATFORM
 */


/*
 *  platform hi3716c/h/m
 * 
 */
#define CONFIG_HI3716X_PLATFORM	y


/*
 *  platform hi3715
 *  #define CONFIG_HI3715_PLATFORM
 */
