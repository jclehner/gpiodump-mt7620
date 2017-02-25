/*
 * MT7620 GPIOMODE register dumper
 *
 * Copyright (C) Joseph C. Lehner 2017
 *
 * Licensed under the GNU GPLv3
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>

void die(const char *message)
{
	perror(message);
	exit(1);
}

int main()
{
	if (access("/dev/mem", F_OK) != 0) {
		if (mknod("/dev/mem", 0600 | S_IFCHR, makedev(1, 1)) < 0) {
			die("mknod");
		}
	}

	int fd = open("/dev/mem", O_RDONLY);
	if (fd < 0) {
		die("open");
	}

	uint32_t *sysctl = mmap(NULL, 256, PROT_READ, MAP_PRIVATE, fd, 0x10000000);
	if (sysctl == MAP_FAILED) {
		die("mmap");
	}

	struct {
		uint32_t bitoff;
		uint32_t bitlen;
		char label[32];
		char modes[8][32];
	} mux[] = {
		{ 30, 2, "SUTIF_SHARE_MODE",   { "disabled", "UARTLITE", "I2C", "reserved" }},
		{ 21, 2, "WDT_RST_MODE",       { "WDT_RST_N", "REFCLK0_OUT", "GPIO", "reserved" }},
		{ 20, 1, "PA_G_GPIO_MODE",     { "normal", "GPIO" }},
		{ 18, 2, "ND_SD_GPIO_MODE",    { "NAND", "SD", "GPIO", "reserved" }},
		{ 16, 2, "PERST_GPIO_MODE",    { "PERST_N", "REFCLK0_OUT", "GPIO", "reserved" }},
		{ 15, 1, "EPHY_LED_GPIO_MODE", { "normal", "GPIO" }},
		{ 13, 1, "WLED_GPIO_MODE",     { "normal", "GPIO" }},
		{ 12, 1, "SPI_REFCLK0_MODE",   { "normal", "GPIO" }},
		{ 11, 1, "SPI_GPIO_MODE",      { "normal", "GPIO" }},
		{ 10, 1, "RGMII2_GPIO_MODE",   { "normal", "GPIO" }},
		{  9, 1, "RGMII1_GPIO_MODE",   { "normal", "GPIO" }},
		{  7, 2, "MDIO_GPIO_MODE",     { "normal", "REF_CLK", "GPIO", "reserved" }},
		{  5, 1, "UARTL_GPIO_MODE",    { "normal", "GPIO" }},
		{  2, 3, "UARTF_SHARE_MODE",   { "UARTF", "PCM,UARTF", "PCM,I2S", "I2S,UARTF",
										   "PCM,GPIO", "GPIO,UARTF", "GPIO,I2S", "GPIO" }},
		{  0, 1, "I2C_GPIO_MODE",      { "normal", "GPIO" }}
	};

	uint32_t masks[] = { 0, 1, 3, 7  };
	uint32_t gpiomode = sysctl[24];

	printf("%.8s GPIOMODE = 0x%08x\n", (char*)sysctl, gpiomode);
	printf("------------------------------\n");

	int i = 0;
	for (; i < sizeof(mux) / sizeof(mux[0]); ++i) {
		uint32_t mode = (gpiomode & (masks[mux[i].bitlen] << mux[i].bitoff)) >> mux[i].bitoff;
		printf("  %-20s = %d, %s\n", mux[i].label, mode, mux[i].modes[mode]);
	}

	return 0;

}
