/*
 * Copyright (C) 2010-2013 Freescale Semiconductor, Inc.
 * Copyright (C) 2014, Bachmann electronic GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <malloc.h>
#include <asm/arch/mx6-pins.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/mxc_i2c.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/arch/crm_regs.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <netdev.h>
#include <i2c.h>
#include <pca953x.h>
#include <asm/gpio.h>
#include <phy.h>

DECLARE_GLOBAL_DATA_PTR;

#define OUTPUT_40OHM	(PAD_CTL_SPEED_MED|PAD_CTL_DSE_40ohm)

#define UART_PAD_CTRL	(PAD_CTL_PUS_100K_UP |			\
	OUTPUT_40OHM | PAD_CTL_SRE_FAST | PAD_CTL_HYS)

#define USDHC_PAD_CTRL	(PAD_CTL_PUS_47K_UP |			\
	PAD_CTL_SPEED_LOW | PAD_CTL_DSE_80ohm |			\
	PAD_CTL_SRE_FAST | PAD_CTL_HYS)

#define ENET_PAD_CTRL	(PAD_CTL_PUS_100K_UP | OUTPUT_40OHM |	\
	PAD_CTL_HYS)

#define SPI_PAD_CTRL	(PAD_CTL_HYS | OUTPUT_40OHM |		\
	PAD_CTL_SRE_FAST)

#define I2C_PAD_CTRL	(PAD_CTL_PUS_100K_UP | OUTPUT_40OHM |	\
	PAD_CTL_HYS | PAD_CTL_ODE | PAD_CTL_SRE_FAST)

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)PHYS_SDRAM, PHYS_SDRAM_SIZE);

	return 0;
}

static iomux_v3_cfg_t const uart1_pads[] = {
	MX6_PAD_CSI0_DAT10__UART1_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_CSI0_DAT11__UART1_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(uart1_pads, ARRAY_SIZE(uart1_pads));
}

static iomux_v3_cfg_t const enet_pads[] = {
	MX6_PAD_KEY_ROW1__ENET_COL | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_KEY_COL3__ENET_CRS | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_MDIO__ENET_MDIO | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_MDC__ENET_MDC | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_GPIO_16__ENET_REF_CLK | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_GPIO_18__ENET_RX_CLK | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RXD0__ENET_RX_DATA0 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RXD1__ENET_RX_DATA1 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_KEY_COL2__ENET_RX_DATA2 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_KEY_COL0__ENET_RX_DATA3 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_CRS_DV__ENET_RX_EN | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_REF_CLK__ENET_TX_CLK | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TXD0__ENET_TX_DATA0 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TXD1__ENET_TX_DATA1 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_KEY_ROW2__ENET_TX_DATA2 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_KEY_ROW0__ENET_TX_DATA3 | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TX_EN__ENET_TX_EN | MUX_PAD_CTRL(ENET_PAD_CTRL),
};

static void setup_iomux_enet(void)
{
	imx_iomux_v3_setup_multiple_pads(enet_pads, ARRAY_SIZE(enet_pads));
}

static iomux_v3_cfg_t const ecspi1_pads[] = {
	MX6_PAD_DISP0_DAT3__ECSPI3_SS0  | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_DISP0_DAT4__ECSPI3_SS1  | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_DISP0_DAT2__ECSPI3_MISO | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_DISP0_DAT1__ECSPI3_MOSI | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_DISP0_DAT0__ECSPI3_SCLK | MUX_PAD_CTRL(SPI_PAD_CTRL),
};

static void setup_iomux_spi(void)
{
	imx_iomux_v3_setup_multiple_pads(ecspi1_pads, ARRAY_SIZE(ecspi1_pads));
}

int board_early_init_f(void)
{
	setup_iomux_uart();
	setup_iomux_spi();

	return 0;
}

static iomux_v3_cfg_t const usdhc3_pads[] = {
	MX6_PAD_SD3_CLK__SD3_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_CMD__SD3_CMD   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT0__SD3_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT1__SD3_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT2__SD3_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT3__SD3_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT4__SD3_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT5__SD3_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT6__SD3_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT7__SD3_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_RST__SD3_RESET | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

int board_mmc_getcd(struct mmc *mmc)
{
	return 1;
}

struct fsl_esdhc_cfg usdhc_cfg[] = {
	{USDHC3_BASE_ADDR},
};

int board_mmc_init(bd_t *bis)
{
	usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
	usdhc_cfg[0].max_bus_width = 8;

	imx_iomux_v3_setup_multiple_pads(usdhc3_pads, ARRAY_SIZE(usdhc3_pads));

	return fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
}

#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)

/* I2C3 - IO expander  */
static struct i2c_pads_info i2c_pad_info2 = {
	.scl = {
		.i2c_mode = MX6_PAD_EIM_D17__I2C3_SCL | PC,
		.gpio_mode = MX6_PAD_EIM_D17__GPIO3_IO17 | PC,
		.gp = IMX_GPIO_NR(3, 17)
	},
	.sda = {
		.i2c_mode = MX6_PAD_EIM_D18__I2C3_SDA | PC,
		.gpio_mode = MX6_PAD_EIM_D18__GPIO3_IO18 | PC,
		.gp = IMX_GPIO_NR(3, 18)
	}
};

static iomux_v3_cfg_t const pwm_pad[] = {
	MX6_PAD_SD1_CMD__PWM4_OUT | MUX_PAD_CTRL(OUTPUT_40OHM),
};

static void leds_on(void)
{
	/* turn on all possible leds connected via GPIO expander */
	i2c_set_bus_num(2);
	pca953x_set_dir(CONFIG_SYS_I2C_PCA953X_ADDR, 0xffff, PCA953X_DIR_OUT);
	pca953x_set_val(CONFIG_SYS_I2C_PCA953X_ADDR, 0xffff, 0x0);
}

static void backlight_lcd_off(void)
{
	unsigned gpio = IMX_GPIO_NR(2, 0);
	gpio_direction_output(gpio, 0);

	gpio = IMX_GPIO_NR(2, 3);
	gpio_direction_output(gpio, 0);
}

int board_eth_init(bd_t *bis)
{
	uint32_t base = IMX_FEC_BASE;
	struct mii_dev *bus = NULL;
	struct phy_device *phydev = NULL;
	int ret;

	setup_iomux_enet();

	bus = fec_get_miibus(base, -1);
	if (!bus)
		return 0;

	/* scan phy 0 and 5 */
	phydev = phy_find_by_mask(bus, 0x21, PHY_INTERFACE_MODE_RGMII);
	if (!phydev) {
		free(bus);
		return 0;
	}

	/* depending on the phy address we can detect our board version */
	if (phydev->addr == 0)
		setenv("boardver", "");
	else
		setenv("boardver", "mr");

	printf("using phy at %d\n", phydev->addr);
	ret = fec_probe(bis, -1, base, bus, phydev);
	if (ret) {
		printf("FEC MXC: %s:failed\n", __func__);
		free(phydev);
		free(bus);
	}
	return 0;
}

int board_init(void)
{
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	backlight_lcd_off();

	setup_i2c(2, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info2);

	leds_on();

	/* enable ecspi3 clocks */
	enable_cspi_clock(1, 2);

	return 0;
}

int checkboard(void)
{
	puts("Board: "CONFIG_SYS_BOARD"\n");
	return 0;
}

#ifdef CONFIG_CMD_BMODE
static const struct boot_mode board_boot_modes[] = {
	/* 4 bit bus width */
	{"mmc0",	MAKE_CFGVAL(0x40, 0x30, 0x00, 0x00)},
	{NULL,		0},
};
#endif

int misc_init_r(void)
{
#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes(board_boot_modes);
#endif
	return 0;
}
