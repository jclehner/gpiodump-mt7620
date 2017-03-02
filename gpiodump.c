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
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>

#define REGWORD(b, n) *((uint32_t*)((char*)(b) + n))

static void die(const char *message)
{
	perror(message);
	exit(1);
}

static uint32_t bits(uint32_t src, uint32_t off, uint32_t len)
{
	if (!len || (off + len) > 32) {
		return 0;
	}

	return (src >> off) & (0xffffffff >> (32 - len));
}

static void dump(bool read, uint32_t gpiomode)
{
	if (read) {
		if (access("/dev/mem", F_OK) != 0) {
			if (mknod("/dev/mem", 0600 | S_IFCHR, makedev(1, 1)) < 0) {
				die("mknod");
			}
		}

		int fd = open("/dev/mem", O_RDONLY);
		if (fd < 0) {
			die("open");
		}

		char *sysctl = mmap(NULL, 256, PROT_READ, MAP_PRIVATE, fd, 0x10000000);
		if (sysctl == MAP_FAILED) {
			die("mmap");
		}

		if (strstr(sysctl, "MT7620") != sysctl) {
			fprintf(stderr, "error: not an mt7620 chip\n");
			exit(1);
		}

		gpiomode = REGWORD(sysctl, 0x60);
	}

	struct {
		int bitoff;
		int bitlen;
		int defval;
		char label[32];
		char modes[8][32];
	} mux[] = {
		{ 30, 2, 0, "SUTIF_SHARE_MODE",   { "disabled", "UARTLITE", "I2C", "reserved" }},
		{ 21, 2, 0, "WDT_RST_MODE",       { "WDT_RST_N", "REFCLK0_OUT", "GPIO", "reserved" }},
		{ 20, 1, 1, "PA_G_GPIO_MODE",     { "normal", "GPIO" }},
		{ 18, 2, 2, "ND_SD_GPIO_MODE",    { "NAND", "SD", "GPIO", "reserved" }},
		{ 16, 2, 2, "PERST_GPIO_MODE",    { "PERST_N", "REFCLK0_OUT", "GPIO", "reserved" }},
		{ 15, 1, 0, "EPHY_LED_GPIO_MODE", { "normal", "GPIO" }},
		{ 13, 1, 1, "WLED_GPIO_MODE",     { "normal", "GPIO" }},
		{ 12, 1, 1, "SPI_REFCLK0_MODE",   { "normal", "GPIO" }},
		{ 11, 1, 0, "SPI_GPIO_MODE",      { "normal", "GPIO" }},
		{ 10, 1, 1, "RGMII2_GPIO_MODE",   { "normal", "GPIO" }},
		{  9, 1, 1, "RGMII1_GPIO_MODE",   { "normal", "GPIO" }},
		{  7, 2, 2, "MDIO_GPIO_MODE",     { "normal", "REF_CLK", "GPIO", "reserved" }},
		{  5, 1, 1, "UARTL_GPIO_MODE",    { "normal", "GPIO" }},
		{  2, 3, 7, "UARTF_SHARE_MODE",   { "UARTF", "PCM,UARTF", "PCM,I2S", "I2S,UARTF",
										   "PCM,GPIO", "GPIO,UARTF", "GPIO,I2S", "GPIO" }},
		{  0, 1, 1, "I2C_GPIO_MODE",      { "normal", "GPIO" }}
	};

	printf("GPIOMODE = 0x%08x\n", gpiomode);

	int i = 0;
	for (; i < sizeof(mux) / sizeof(mux[0]); ++i) {
		uint32_t mode = bits(gpiomode, mux[i].bitoff, mux[i].bitlen);
		printf("  %-20s = %d, %-14s", mux[i].label, mode, mux[i].modes[mode]);
		printf("%s\n", mode == mux[i].defval ? " [default]" : "");
	}
}

int main(int argc, char **argv)
{
	if (argc > 1) {
		dump(false, strtoul(argv[1], NULL, 16));
	} else {
#ifdef __mips
		dump(true, 0);
#else
		fprintf(stderr, "usage: gpiodump <value>\n");
		return 1;
#endif
	}

	return 0;
}
