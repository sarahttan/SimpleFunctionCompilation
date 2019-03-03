#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spidev_control.h"

#define DEFAULT_MODE 0
#define DEFAULT_DELAY 0
#define BITS_PER_WORD 8
#define NUM_SPI_DEVICES 5

#define DEBUG	//comment this out to commont out debug print statements

static void pabort(const char *s)
{
	perror(s);
	exit_spidev_control();
}

static const char *devices[NUM_SPI_DEVICES] = {"/dev/spidev0.0", "/dev/spidev0.1", "/dev/spidev1.0", "/dev/spidev1.1", "/dev/spidev1.2"};
static uint32_t    spiSpeeds [NUM_SPI_DEVICES] ;
static int         spiFds [NUM_SPI_DEVICES] ;


/*
 * channel_to_index: 
 *	Get the index corresponding to the desired channel.
 *	Return the index.
 */
int channel_to_index(uint32_t channel)
{
	switch (channel) {
		case 00:
			return 0;
		case 01:
			return 1;
		case 10:
			return 2;	
		case 11:
			return 3;
		case 12:
			return 4;
			break;
		default:
			pabort("Unable to locate channel:\n") ;
			return -1;
	}
	return 0;
}

/*
 * get_fd_speed:
 *	Get the fd and speed given the channel.
 *  Return 0 on success.
 */
int get_fd_speed(uint32_t channel, int *fd, uint32_t *speed)
{
	int index = channel_to_index(channel);
	if (index < 0) {
		pabort("Unable to locate SPI device fd or speed:\n") ;
			return -1;
	}
	
	*fd = spiFds[index];
	*speed = spiSpeeds[index];
	return 0;
}

/*
 * set_fd_speed:
 *	Set the fd and speed given the channel.
 *  Return 0 on success.
 */
int set_fd_speed(uint32_t channel, int fd, uint32_t speed)
{
	int index = channel_to_index(channel);
	if (index < 0) {
		pabort("Unable to locate SPI device to set fd or speed:\n") ;
			return -1;
	}
	
	spiFds[index] = fd;
	spiSpeeds[index] = speed;
	return 0;
}
/*
 * transfer: transfers array tx into the given spi channel (00, 01, 10, 11, 12) and returns rx
 */
extern void spi_transfer(uint32_t channel, uint8_t tx[], uint8_t rx[], int len) 
{
	int fd;
	uint32_t speed;
	int ret = get_fd_speed(channel, &fd, &speed);
	
	// set the defaults for bits and default speed.
	uint8_t bits = BITS_PER_WORD;
	uint16_t delay = DEFAULT_DELAY;
	
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	
	// Make sure the fd was opened correctly
	if (ret < 0) 
		pabort("Can't read from file descriptor");
	
	// Send the SPI message
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0)
		pabort("can't send spi message");

	// Print out the return array
#ifdef DEBUG
	for (ret = 0; ret < len; ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	printf("\n");
#endif
}

extern void exit_spidev_control()
{
#ifdef DEBUG
	printf("Closing all open spi ports\n");
#endif
	for (int i = 0; i < NUM_SPI_DEVICES; i++) {
		close(spiFds[i]);
	}
	abort();
}

extern void init_spidev_control(uint32_t channel, uint32_t speed)
{
	int index = channel_to_index(channel);
	if (index < 0) {
		pabort("Unable to locate SPI device to set fd or speed:\n") ;
	}
	
	int ret = 0;
	const char *device = devices[index];
	int fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");
	
	// set the defaults for bits and default speed.
	uint8_t mode = DEFAULT_MODE;
	uint8_t bits = BITS_PER_WORD;
	
	// update the global variables.
	set_fd_speed(channel, fd, speed);

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

#ifdef DEBUG
	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
#endif
}