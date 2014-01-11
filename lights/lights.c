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
#include "helper.h"
#include "lights.h"
#include "spi.h"

void anim_all(params para)
{
  uint8_t  index;	
  uint32_t loop;
  uint8_t  led[nLEDs];
	
  switch(para.sub)
  {
	case 0: // OFF
	  clearWS2803();
	  break;
	case 1: // ON
      for(index = 0; index < nLEDs; index++){
        led[index] = (uint8_t)(0xFF & para.pwm);
      }	  
      loadWS2803(led);
	  break;
	case 2: // BLINK
	  for(loop=0;loop < para.loopCounter; loop++){
        for(index = 0; index < nLEDs; index++){ 
			led[index] = (uint8_t)(0xFF & para.pwm); 
		}	         
        loadWS2803(led);	 
        usleep(para.sleepTime); 
  	    clearWS2803();
        usleep(para.sleepTime);         
      }
	  break;
	case 3:
	  break;
	default:
	  break;
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
  if(para.remainOn == 0) {
   clearWS2803(); 
  }
  close_spi();
  return 0;
}
