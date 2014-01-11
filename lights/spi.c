#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "led.h"
#include "helper.h"
#include "spi.h"

static       int              fd_spi;
static const char            *device          = "/dev/spidev0.0";
static       uint8_t          mode;
static       uint8_t          bits            = 8;
static       uint32_t         speed           = 500000;
static       uint16_t         delay;
struct       spi_ioc_transfer tr;

             uint8_t          ledBar[nLEDs];

int init_spi(void){

  int ret = 0;

  fd_spi = open(device, O_RDWR);
  if (fd_spi < 0) pabort("can't open device");
  
  ret = ioctl(fd_spi, SPI_IOC_WR_MODE, &mode);
  if (ret == -1) pabort("can't set spi mode");

  ret = ioctl(fd_spi, SPI_IOC_RD_MODE, &mode);
  if (ret == -1) pabort("can't get spi mode");

  ret = ioctl(fd_spi, SPI_IOC_WR_BITS_PER_WORD, &bits);
  if (ret == -1) pabort("can't set bits per word");

  ret = ioctl(fd_spi, SPI_IOC_RD_BITS_PER_WORD, &bits);
  if (ret == -1) pabort("can't get bits per word");

  ret = ioctl(fd_spi, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
  if (ret == -1) pabort("can't set max speed hz");

  ret = ioctl(fd_spi, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
  if (ret == -1) pabort("can't get max speed hz");

  tr.tx_buf        = (unsigned long)ledBar;
  tr.len           = nLEDs;
  tr.delay_usecs   = delay;
  tr.speed_hz      = speed;
  tr.bits_per_word = bits;

  return ret;
}

void close_spi(void)
{
  close(fd_spi);
}

void loadWS2803(uint8_t led[nLEDs]){
  uint8_t index;
  for(index = 0;index < nLEDs;index++){
	  ledBar[index] = led[index];
  }
  if(ioctl(fd_spi, SPI_IOC_MESSAGE(1), &tr) < 1) pabort("can't send spi message");
  usleep(600);
}

void clearWS2803(void){
  int wsOut;

  for(wsOut = 0; wsOut < nLEDs; wsOut++){
    ledBar[wsOut] = 0x00;
  }
  if(ioctl(fd_spi, SPI_IOC_MESSAGE(1), &tr) < 1) pabort("can't send spi message");
  usleep(600);
}
