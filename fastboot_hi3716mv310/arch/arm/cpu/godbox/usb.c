#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/arch/platform.h>
#include <version.h>

int arch_usb_init(void)
{
	unsigned int reg;

	if (_HI3712_V100 == get_chipid()) {
		int timeout;

		reg = readl(HI3712_USB_PERI_USB0);
		reg |= HI3712_USB_ULPI_BYPASS_EN;

		reg |= HI3712_USB_WORDINTERFACE;
		reg &= ~(HI3712_USB_SS_BURST16_EN);
		reg &= ~(HI3712_USB_ATERESET1);
		reg &= ~(HI3712_USB_ATERESETX);
		writel(reg, HI3712_USB_PERI_USB0);
		udelay(100);

		reg = readl(HI3712_USB_PREI_USB1);
		reg |= HI3712_USB_FREECLK0_INV_EN;
		writel(reg, HI3712_USB_PREI_USB1);
		udelay(100);

		reg = readl(HI3712_PERI_CRG36);
		reg |= HI3712_USB_BRG_CKEN;
		reg |= HI3712_USB_OTG_CKEN;
		reg |= HI3712_USB_HOST_CKEN;
		reg |= HI3712_USB1_CKEN;
		reg |= HI3712_USB0_CKEN;
		writel(reg, HI3712_PERI_CRG36);
		udelay(100);

		timeout = 0x10;
		do {
			reg = readl(HI3712_PERI_CRG36);
			reg |= HI3712_USBPHY0_REQ;
			reg |= HI3712_USBPHY1_REQ;
			writel(reg, HI3712_PERI_CRG36);
			udelay(100);
			reg &= ~(HI3712_USBPHY0_REQ);
			reg &= ~(HI3712_USBPHY1_REQ);
			writel(reg, HI3712_PERI_CRG36);
			udelay(100);

			reg = readl(HI3712_PERI_USB3);

			switch (get_cpu_version(NULL)) {
			case _HI3712_V100A:
				if (reg & HI3712_USBPHY0_CLK_TEST)
					timeout = -1;
				break;
			default:
			case _HI3712_V100I:
				if ((reg & HI3712_USBPHY0_CLK_TEST)
				    && (reg & HI3712_USBPHY1_CLK_TEST))
					timeout = -1;
				break;
			}

		} while (--timeout > 0);

		if (!timeout) {
			DBG_BUG(("Usb phy initialize timeout.\n"));
		}

		udelay(100);
		reg = readl(HI3712_USB_PERI_USB0);
		reg |= HI3712_USB_ATERESETX;
		udelay(100);
		reg &= ~(HI3712_USB_ATERESETX);
		writel(reg, HI3712_USB_PERI_USB0);
		udelay(100);

		reg = readl(HI3712_PERI_CRG36);
		reg &= ~(HI3712_USB_BRG_SRST_REQ);
		reg &= ~(HI3712_UTMI1_HOST_REQ);
		reg &= ~(HI3712_UTMI0_HOST_REQ);
		reg &= ~(HI3712_USBPHY1_TREQ);
		reg &= ~(HI3712_USBPHY0_TREQ);
		reg &= ~(HI3712_USB_OTG_REQ);
		reg &= ~(HI3712_USB_HOST_REQ);
		reg &= ~(HI3712_USB_OTG_SRST_REQ);
		reg &= ~(HI3712_USB_HOST_SRST_REQ);
		writel(reg, HI3712_PERI_CRG36);
		udelay(100);

	} else if (_HI3716M_V310 != get_chipid()) {

		reg = readl(HI3716X_PERI_CRG36);
		reg |= HI3716X_USB_CKEN;
		reg |= HI3716X_USBPHY_REQ;
		reg &= ~(HI3716X_USBPHY_PORT1_TREQ);
		reg &= ~(HI3716X_USBPHY_PORT0_TREQ);
		reg |= HI3716X_USB_CTRL_UTMI1_REG;
		reg |= HI3716X_USB_CTRL_UTMI0_REG;
		reg |= HI3716X_USB_AHB_SRST_REQ;
		writel(reg, HI3716X_PERI_CRG36);
		udelay(20);

		reg = readl(HI3716X_USB_PERI_USB0);
		reg |= HI3716X_USB_ULPI_BYPASS_EN;
		reg &= ~(HI3716X_USB_WORDINTERFACE);
		reg &= ~(HI3716X_USB_SS_BURST16_EN);
		writel(reg, HI3716X_USB_PERI_USB0);
		udelay(100);

		reg = readl(HI3716X_PERI_CRG36);
		reg &= ~(HI3716X_USBPHY_REQ);
		writel(reg, HI3716X_PERI_CRG36);
		udelay(100);

		reg = readl(HI3716X_PERI_CRG36);
		reg &= ~(HI3716X_USB_CTRL_UTMI1_REG);
		reg &= ~(HI3716X_USB_CTRL_UTMI0_REG);
		reg &= ~(HI3716X_USB_CTRL_HUB_REG);
		reg &= ~(HI3716X_USB_AHB_SRST_REQ);
		writel(reg, HI3716X_PERI_CRG36);
		udelay(10);
		WDG_ENABLE(0x5DC0);
		readl(0x60070000);
		WDG_DISABLE();
	}

	return 0;
}
