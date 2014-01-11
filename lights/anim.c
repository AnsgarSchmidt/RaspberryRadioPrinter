#include <unistd.h>
#include <stdint.h>
#include "parameter.h"
#include "led.h"
#include "spi.h"
#include "anim.h"

void anim_all(params para)
{
  uint8_t  index;	
  uint32_t loop;
  uint8_t  led[nLEDs];
	
  switch(para.sub)
  {
	case PARAM_SUB_OFF:
	  clearWS2803();
	  break;
	case PARAM_SUB_ON:
      for(index = 0; index < nLEDs; index++){
        led[index] = (uint8_t)(0xFF & para.pwm);
      }	  
      loadWS2803(led);
	  break;
	case PARAM_SUB_BLINK:
	  for(loop=0;loop < para.loopCounter; loop++){
        for(index = 0; index < nLEDs; index++){ 
			led[index] = (uint8_t)(0xFF & para.pwm); 
		}	         
        loadWS2803(led);	 
        usleep(1000 * para.sleepTime); 
  	    clearWS2803();
        usleep(1000 * para.sleepTime);         
      }
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

void anim_external(params para)
{
}
