/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-11-04
 *
******************************************************************************/

#include <common.h>
#include <bootimg.h>
#include <linux/err.h>

void show_bootimg_header(void *buf)
{
	struct boot_img_hdr *hdr = buf;

	if (!hdr)
		return;

	printf("\nFound Initrd at 0x%08X (Size %d Bytes), align at %d Bytes\n", \
		hdr->ramdisk_addr, hdr->ramdisk_size, hdr->page_size);
	printf("\n");
}

int check_bootimg(char *buf, unsigned int len)
{ 
	printf("______check_booting____\n");
	struct boot_img_hdr *hdr = (struct boot_img_hdr *)buf;

	if (len < sizeof(struct boot_img_hdr))
	{
		printf("_________len < sizeof(struct boot_img_hdr)\n");
		return -EINVAL;
	}
	if (memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE))
	{	
		printf("_____memcmp true____\n");
		printf("_________%d_________\n", -ENODATA);
		return -ENODATA;
	}
	return 0;
}

int bootimg_get_kernel(char *buf, unsigned int len, unsigned int *entry)
{
	printf("_______booting_get_kernel_____\n");
	int ret;
	struct boot_img_hdr *hdr = (struct boot_img_hdr *)buf;

	ret = check_bootimg(buf, len);
	printk("%d\n", ret);
	if (ret)
	{	
		printf("______%d\n______", ret);
		return ret;
	}
	if (hdr->page_size > len)
	{
		printf("hdr->page_size > len \n");
		return -EINVAL;
		}
	if (entry)
	{
		printf("entry here\n");
		*entry = (unsigned int)((unsigned int)hdr + hdr->page_size);
		}

	return 0;
}

int bootimg_get_ramfs(char *buf, unsigned int len, unsigned int *initrd_start,
		      unsigned int *initrd_size)
{
	int ret;
	struct boot_img_hdr *hdr = (struct boot_img_hdr *)buf;

	ret = check_bootimg(buf, len);
	if (ret)
		return ret;

	if (!hdr->ramdisk_size || (hdr->kernel_size + hdr->page_size) > len)
		return -EINVAL;

	if (initrd_start) {
		unsigned int initrd_img_addr = (unsigned int)hdr \
			+ roundup(hdr->kernel_size + hdr->page_size, hdr->page_size);

		memmove((void *)hdr->ramdisk_addr, (const void *)initrd_img_addr,
			hdr->ramdisk_size);
		*initrd_start = hdr->ramdisk_addr;
	}

	if (initrd_size)
		*initrd_size = hdr->ramdisk_size;

	return 0;
}
