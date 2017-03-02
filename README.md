# gpiodump-mt7620

This utility dumps the MT7620's current `GPIOMODE` register setting. It
can help in writing device tree files for these devices, which is required
when porting LEDE / OpenWrt.

Detailed info on the `GPIOMODE` register can be found in the MT7620
[Datasheet](https://www.anz.ru/files/mediatek/MT7620_Datasheet.pdf)

A precompiled static mips binary is available, but the utility can also be
used to decode the value on any other machine.

####### Usage

Dumping GPIOMODE directly (on an MT7620 machine only):

```
# ./gpiodump
GPIOMODE = 0x001a331d
  SUTIF_SHARE_MODE     = 0, disabled       [default]
  WDT_RST_MODE         = 0, WDT_RST_N      [default]
  PA_G_GPIO_MODE       = 1, GPIO           [default]
  ND_SD_GPIO_MODE      = 2, GPIO           [default]
  PERST_GPIO_MODE      = 2, GPIO           [default]
  EPHY_LED_GPIO_MODE   = 0, normal         [default]
  WLED_GPIO_MODE       = 1, GPIO           [default]
  SPI_REFCLK0_MODE     = 1, GPIO           [default]
  SPI_GPIO_MODE        = 0, normal         [default]
  RGMII2_GPIO_MODE     = 0, normal
  RGMII1_GPIO_MODE     = 1, GPIO           [default]
  MDIO_GPIO_MODE       = 2, GPIO           [default]
  UARTL_GPIO_MODE      = 0, normal
  UARTF_SHARE_MODE     = 7, GPIO           [default]
  I2C_GPIO_MODE        = 1, GPIO           [default]
```

On any other machine:

```
# ./gpiodump 0x001a331d
GPIOMODE = 0x001a331d
  SUTIF_SHARE_MODE     = 0, disabled       [default]
  WDT_RST_MODE         = 0, WDT_RST_N      [default]
  PA_G_GPIO_MODE       = 1, GPIO           [default]
  ND_SD_GPIO_MODE      = 2, GPIO           [default]
  PERST_GPIO_MODE      = 2, GPIO           [default]
  EPHY_LED_GPIO_MODE   = 0, normal         [default]
  WLED_GPIO_MODE       = 1, GPIO           [default]
  SPI_REFCLK0_MODE     = 1, GPIO           [default]
  SPI_GPIO_MODE        = 0, normal         [default]
  RGMII2_GPIO_MODE     = 0, normal
  RGMII1_GPIO_MODE     = 1, GPIO           [default]
  MDIO_GPIO_MODE       = 2, GPIO           [default]
  UARTL_GPIO_MODE      = 0, normal
  UARTF_SHARE_MODE     = 7, GPIO           [default]
  I2C_GPIO_MODE        = 1, GPIO           [default]
```

