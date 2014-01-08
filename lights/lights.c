#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "parameter.h"
#include "led.h"

static void pabort(const char *s){
  perror(s);
  abort();
}

typedef struct{
	uint8_t  master;
	uint16_t sub;
	uint16_t pwm;
	uint16_t loopCounter;
	uint32_t sleepTime;
	uint8_t  remainOn;
} params;

static const char    *device = "/dev/spidev0.0";
static       uint8_t  mode;
static       uint8_t  bits   = 8;
static       uint32_t speed  = 500000;
static       uint16_t delay;
             int      fd_spi;
             uint8_t  ledBar[nLEDs];
             
struct       spi_ioc_transfer tr;

int init_spi(){
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

void loadWS2803(){
  if(ioctl(fd_spi, SPI_IOC_MESSAGE(1), &tr) < 1) pabort("can't send spi message");
  usleep(600);
}

void clearWS2803(){
  int wsOut;

  for(wsOut = 0; wsOut < nLEDs; wsOut++){
    ledBar[wsOut] = 0x00;
    loadWS2803();
  }
}

void anim_all(params para)
{
  uint32_t wsOut;
  uint32_t iOut;
  uint32_t loop;	
	
  switch(para.sub)
  {
	case 0: // OFF
	  clearWS2803();
	  break;
	case 1: // ON
      for(wsOut = 0; wsOut < nLEDs; wsOut++){
        ledBar[wsOut] = (uint8_t)(0xFF & para.pwm);
      }	  
      loadWS2803();
	  break;
	case 2: // BLINK
	  for(loop=0;loop < para.loopCounter; loop++){
        for(wsOut = 0; wsOut < nLEDs; wsOut++){ ledBar[wsOut] = (uint8_t)(0xFF & para.pwm); }	         
        loadWS2803();	 
        usleep(para.sleepTime); 
  	    clearWS2803();
        usleep(para.sleepTime);         
      }
	  break;
	case 3:
	  break;
	default:
	  break;
	  /*
    for(iOut = 0; iOut < 256; iOut++){
      for(wsOut = 9; wsOut < 15; wsOut++){
        ledBar[wsOut] = (uint8_t)(0xFF & iOut);
        loadWS2803();
        clearWS2803();
      }
    }
    */
  }
}

void anim_suround(params para)
{
}

void anim_circle(params para)
{
}

void anim_head(params para)
{
}

int main(int argc, char *argv[]){
  if(init_spi() == -1) pabort("can't initialize ISP");

  clearWS2803();

  if(argc != 7) pabort("Wrong amount of parameters");
  
  params para;
  para.master      = atoi(argv[PARAM_MASTER]);
  para.sub         = atoi(argv[PARAM_SUB]);
  para.pwm         = atoi(argv[PARAM_PWM]);
  para.loopCounter = atoi(argv[PARAM_LOOPCOUNTER]);
  para.sleepTime   = atoi(argv[PARAM_SLEEPTIME]);
  para.remainOn    = atoi(argv[PARAM_ON_OFF]);

  switch(para.master){
    case PARAM_MASTER_ALL:
      anim_all(para);
      break;
    case PARAM_MASTER_CIRCLE:
      anim_circle(para);
      break;
    case PARAM_MASTER_SURROUND:
      anim_suround(para);
      break;
    case PARAM_MASTER_HEAD:
      anim_head(para);
      break;
    default:      
      break;
  }
  if(para.remainOn == 0) { clearWS2803(); }
  close(fd_spi);
  return 0;
}
