#include <unistd.h>
#include <stdint.h>
#include "parameter.h"
#include "led.h"
#include "spi.h"
#include "anim.h"
#include "helper.h"

void anim_all(params para)
{
  uint8_t  index;	
  uint32_t loop;
  uint32_t loop2;
  uint8_t  led[nLEDs];
  uint8_t  ledCW[] = {LEFT_BOTTOM, LEFT_MIDDLE, LEFT_TOP, RIGHT_TOP, RIGHT_MIDDLE, RIGHT_BOTTOM, CIRCLE_RIGHT_BOTTOM, CIRCLE_LEFT_BOTTOM, CIRCLE_LEFT_TOP, CIRCLE_RIGHT_TOP, CIRCLE_RIGHT_BOTTOM, CIRCLE_LEFT_BOTTOM};
	
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
	case PARAM_SUB_CLOCK_WISE:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2< arraysize(ledCW);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
				led[index] = 0;
			  }
              led[ledCW[loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;
	case PARAM_SUB_COUNTER_CLOCK_WISE:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2 < arraysize(ledCW)+1;loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
				led[index] = 0;
			  }
              led[ledCW[arraysize(ledCW) - loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
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
