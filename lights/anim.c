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
  uint8_t  ledCWAll[]  = {LEFT_BOTTOM,  LEFT_MIDDLE,  LEFT_TOP,  RIGHT_TOP, RIGHT_MIDDLE, RIGHT_BOTTOM, CIRCLE_RIGHT_BOTTOM, CIRCLE_LEFT_BOTTOM,  CIRCLE_LEFT_TOP, CIRCLE_RIGHT_TOP, CIRCLE_RIGHT_BOTTOM, CIRCLE_LEFT_BOTTOM };
  uint8_t  ledCCWAll[] = {RIGHT_BOTTOM, RIGHT_MIDDLE, RIGHT_TOP, LEFT_TOP,  LEFT_MIDDLE,  LEFT_BOTTOM,  CIRCLE_LEFT_BOTTOM,  CIRCLE_RIGHT_BOTTOM, CIRCLE_RIGHT_TOP, CIRCLE_LEFT_TOP, CIRCLE_LEFT_BOTTOM,  CIRCLE_RIGHT_BOTTOM};
	uint8_t  ledUp[]     = {LEFT_BOTTOM, RIGHT_BOTTOM, LEFT_MIDDLE, RIGHT_MIDDLE, LEFT_TOP, RIGHT_TOP, HEAD, HEAD};
	
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
		  for(loop2=0; loop2< arraysize(ledCWAll);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
				led[index] = 0;
			  }
              led[ledCWAll[loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;
	case PARAM_SUB_COUNTER_CLOCK_WISE:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2 < arraysize(ledCWAll)+1;loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
				led[index] = 0;
			  }
              led[ledCWAll[arraysize(ledCWAll) - loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }	
	  break;
	case PARAM_SUB_FADE:
	  for(loop=0;loop < para.loopCounter; loop++){
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs;index++){
					led[index] = (uint8_t)(0xFF & ((para.pwm/100)*loop2));
				}
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs;index++){
					led[index] = (uint8_t)(0xFF & ((para.pwm/100)*(100-loop2)));
				}
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
	  }
	  break;  
	case PARAM_SUB_FADE_CW:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(ledCWAll);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[ledCWAll[(loop2 + arraysize(ledCWAll) - 0) % arraysize(ledCWAll)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  1.0 )));			  
        led[ledCWAll[(loop2 + arraysize(ledCWAll) - 1) % arraysize(ledCWAll)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  5.0 )));			  
        led[ledCWAll[(loop2 + arraysize(ledCWAll) - 2) % arraysize(ledCWAll)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm / 10.0 )));			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;
	case PARAM_SUB_FADE_CCW:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(ledCWAll);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[ledCCWAll[(loop2 + arraysize(ledCCWAll) - 0) % arraysize(ledCCWAll)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  1.0 )));			  
        led[ledCCWAll[(loop2 + arraysize(ledCCWAll) - 1) % arraysize(ledCCWAll)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  5.0 )));			  
        led[ledCCWAll[(loop2 + arraysize(ledCCWAll) - 2) % arraysize(ledCCWAll)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm / 10.0 )));			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;	  
	case PARAM_SUB_UP:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(ledUp);loop2+=2){
				for(index=0; index<nLEDs; index++){
					led[index] = 0;
				}
				for(index=0; index<loop2+2; index++){					
          led[ledUp[index]] = (uint8_t)(0xFF & para.pwm);			  
			  }
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }			
    }	  
	  break;
	default:
	  break;
  }
}

void anim_circle(params para)
{
	uint8_t index;
	uint8_t loop;
	uint8_t loop2;
  uint8_t circleAll[] = {CIRCLE_LEFT_BOTTOM,  CIRCLE_RIGHT_BOTTOM, CIRCLE_LEFT_TOP,  CIRCLE_RIGHT_TOP};
  uint8_t circleCW[]  = {CIRCLE_LEFT_BOTTOM,  CIRCLE_LEFT_TOP,     CIRCLE_RIGHT_TOP, CIRCLE_RIGHT_BOTTOM};
  uint8_t circleCCW[] = {CIRCLE_RIGHT_BOTTOM, CIRCLE_RIGHT_TOP,    CIRCLE_LEFT_TOP,  CIRCLE_LEFT_BOTTOM};
  uint8_t circleUp[]  = {CIRCLE_LEFT_BOTTOM,  CIRCLE_RIGHT_BOTTOM, CIRCLE_LEFT_TOP,  CIRCLE_RIGHT_TOP};    
  uint8_t led[nLEDs];

  switch(para.sub)
  {
	case PARAM_SUB_OFF:
	  clearWS2803();
	  break;
	case PARAM_SUB_ON:
	  clearWS2803();
	  for(index = 0; index < nLEDs; index++){
		  led[index] = 0;
		}
    for(index = 0; index < arraysize(circleAll); index++){
      led[circleAll[index]] = (uint8_t)(0xFF & para.pwm);
    }	 
    loadWS2803(led);
	  break;
	case PARAM_SUB_BLINK:
	  for(loop=0;loop < para.loopCounter; loop++){
        for(index = 0; index < nLEDs; index++){ 
					led[index] = 0;
				}
				for(index = 0; index < arraysize(circleAll);index++){
		     	led[circleAll[index]] = (uint8_t)(0xFF & para.pwm); 
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
		  for(loop2=0; loop2< arraysize(circleCW);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[circleCW[loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;
	case PARAM_SUB_COUNTER_CLOCK_WISE:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2 < arraysize(circleCCW)+1;loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
 				  led[index] = 0;
			  }
        led[circleCW[arraysize(circleCW) - loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }	
	  break;
	case PARAM_SUB_FADE:
	  for(loop=0;loop < para.loopCounter; loop++){
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}
				for(index = 0; index < arraysize(circleAll); index++){
					led[circleAll[index]] = (uint8_t)(0xFF & ((para.pwm/100)*loop2));
				}
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}				
				for(index = 0; index < arraysize(circleAll);index++){
					led[circleAll[index]] = (uint8_t)(0xFF & ((para.pwm/100)*(100-loop2)));
				}
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
	  }
	  break;  
	case PARAM_SUB_FADE_CW:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(circleCW);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[circleCW[(loop2 + arraysize(circleCW) - 0) % arraysize(circleCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  1.0 )));			  
        led[circleCW[(loop2 + arraysize(circleCW) - 1) % arraysize(circleCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  5.0 )));			  
        led[circleCW[(loop2 + arraysize(circleCW) - 2) % arraysize(circleCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm / 10.0 )));			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;
	case PARAM_SUB_FADE_CCW:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(circleCCW);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[circleCCW[(loop2 + arraysize(circleCCW) - 0) % arraysize(circleCCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  1.0 )));			  
        led[circleCCW[(loop2 + arraysize(circleCCW) - 1) % arraysize(circleCCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  5.0 )));			  
        led[circleCCW[(loop2 + arraysize(circleCCW) - 2) % arraysize(circleCCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm / 10.0 )));			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;	  
	case PARAM_SUB_UP:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(circleUp);loop2+=2){
				for(index=0; index<nLEDs; index++){
					led[index] = 0;
				}
				for(index=0; index<loop2+2; index++){					
          led[circleUp[index]] = (uint8_t)(0xFF & para.pwm);			  
			  }
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
	uint8_t index;
	uint8_t loop;
	uint8_t loop2;
  uint8_t suroundAll[] = {LEFT_BOTTOM,  RIGHT_BOTTOM, LEFT_MIDDLE, RIGHT_MIDDLE, LEFT_TOP,     RIGHT_TOP};
  uint8_t suroundCW[]  = {LEFT_BOTTOM,  LEFT_MIDDLE,  LEFT_TOP,    RIGHT_TOP,    RIGHT_MIDDLE, RIGHT_BOTTOM};
  uint8_t suroundCCW[] = {RIGHT_BOTTOM, RIGHT_MIDDLE, RIGHT_TOP,   LEFT_TOP,     LEFT_MIDDLE,  LEFT_BOTTOM};
  uint8_t suroundUp[]  = {LEFT_BOTTOM,  RIGHT_BOTTOM, LEFT_MIDDLE, RIGHT_MIDDLE, LEFT_TOP,     RIGHT_TOP};    
  uint8_t led[nLEDs];

  switch(para.sub)
  {
	case PARAM_SUB_OFF:
	  clearWS2803();
	  break;
	case PARAM_SUB_ON:
	  clearWS2803();
	  for(index = 0; index < nLEDs; index++){
		  led[index] = 0;
		}
    for(index = 0; index < arraysize(suroundAll); index++){
      led[suroundAll[index]] = (uint8_t)(0xFF & para.pwm);
    }	 
    loadWS2803(led);
	  break;
	case PARAM_SUB_BLINK:
	  for(loop=0;loop < para.loopCounter; loop++){
        for(index = 0; index < nLEDs; index++){ 
					led[index] = 0;
				}
				for(index = 0; index < arraysize(suroundAll);index++){
		     	led[suroundAll[index]] = (uint8_t)(0xFF & para.pwm); 
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
		  for(loop2=0; loop2< arraysize(suroundCW);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[suroundCW[loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;
	case PARAM_SUB_COUNTER_CLOCK_WISE:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2 < arraysize(suroundCCW)+1;loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
 				  led[index] = 0;
			  }
        led[suroundCW[arraysize(suroundCW) - loop2]] = (uint8_t)(0xFF & para.pwm);			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }	
	  break;
	case PARAM_SUB_FADE:
	  for(loop=0;loop < para.loopCounter; loop++){
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}
				for(index = 0; index < arraysize(suroundAll); index++){
					led[suroundAll[index]] = (uint8_t)(0xFF & ((para.pwm/100)*loop2));
				}
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}				
				for(index = 0; index < arraysize(suroundAll);index++){
					led[suroundAll[index]] = (uint8_t)(0xFF & ((para.pwm/100)*(100-loop2)));
				}
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
	  }
	  break;  
	case PARAM_SUB_FADE_CW:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(suroundCW);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[suroundCW[(loop2 + arraysize(suroundCW) - 0) % arraysize(suroundCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  1.0 )));			  
        led[suroundCW[(loop2 + arraysize(suroundCW) - 1) % arraysize(suroundCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  5.0 )));			  
        led[suroundCW[(loop2 + arraysize(suroundCW) - 2) % arraysize(suroundCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm / 10.0 )));			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;
	case PARAM_SUB_FADE_CCW:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(suroundCCW);loop2++){
			  for(index = 0; index < nLEDs;index++)
			  {
  				led[index] = 0;
			  }
        led[suroundCCW[(loop2 + arraysize(suroundCCW) - 0) % arraysize(suroundCCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  1.0 )));			  
        led[suroundCCW[(loop2 + arraysize(suroundCCW) - 1) % arraysize(suroundCCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm /  5.0 )));			  
        led[suroundCCW[(loop2 + arraysize(suroundCCW) - 2) % arraysize(suroundCCW)]] = (uint8_t)(0xFF & ((uint8_t)(para.pwm / 10.0 )));			  
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }
	  }
	  break;	  
	case PARAM_SUB_UP:
	  for(loop=0;loop < para.loopCounter; loop++)
	  {
		  for(loop2=0; loop2<arraysize(suroundUp);loop2+=2){
				for(index=0; index<nLEDs; index++){
					led[index] = 0;
				}
				for(index=0; index<loop2+2; index++){					
          led[suroundUp[index]] = (uint8_t)(0xFF & para.pwm);			  
			  }
			  loadWS2803(led);
			  usleep(1000 * para.sleepTime);         		  		  
		  }			
    }	  
	  break;
	default:
	  break;
  }	
}

void anim_head(params para)
{
	uint8_t index;
	uint8_t loop;
	uint8_t loop2;
  uint8_t led[nLEDs];

  switch(para.sub)
  {
	case PARAM_SUB_OFF:
	  clearWS2803();
	  break;
	case PARAM_SUB_ON:
	  clearWS2803();
	  for(index = 0; index < nLEDs; index++){
		  led[index] = 0;
		}
    led[HEAD] = (uint8_t)(0xFF & para.pwm);
    loadWS2803(led);
	  break;
	case PARAM_SUB_BLINK:
	case PARAM_SUB_UP:
	case PARAM_SUB_CLOCK_WISE:
	case PARAM_SUB_COUNTER_CLOCK_WISE:
	  for(loop=0;loop < para.loopCounter; loop++){
        for(index = 0; index < nLEDs; index++){ 
					led[index] = 0;
				}
				led[HEAD] = (uint8_t)(0xFF & para.pwm); 
        loadWS2803(led);	 
        usleep(1000 * para.sleepTime); 
  	    clearWS2803();
        usleep(1000 * para.sleepTime);         
      }
	  break;
	case PARAM_SUB_FADE:
	case PARAM_SUB_FADE_CW:
	case PARAM_SUB_FADE_CCW:
	  for(loop=0;loop < para.loopCounter; loop++){
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}
				led[HEAD] = (uint8_t)(0xFF & ((para.pwm/100)*loop2));
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}				
				led[HEAD] = (uint8_t)(0xFF & ((para.pwm/100)*(100-loop2)));
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
	  }
	  break;  
	default:
	  break;
  }		
}

void anim_external(params para)
{
	uint8_t index;
	uint8_t loop;
	uint8_t loop2;
  uint8_t led[nLEDs];

  switch(para.sub)
  {
	case PARAM_SUB_OFF:
	  clearWS2803();
	  break;
	case PARAM_SUB_ON:
	  clearWS2803();
	  for(index = 0; index < nLEDs; index++){
		  led[index] = 0;
		}
    led[EXTERNAL] = (uint8_t)(0xFF & para.pwm);
    loadWS2803(led);
	  break;
	case PARAM_SUB_BLINK:
	case PARAM_SUB_UP:
	case PARAM_SUB_CLOCK_WISE:
	case PARAM_SUB_COUNTER_CLOCK_WISE:
	  for(loop=0;loop < para.loopCounter; loop++){
        for(index = 0; index < nLEDs; index++){ 
					led[index] = 0;
				}
				led[EXTERNAL] = (uint8_t)(0xFF & para.pwm); 
        loadWS2803(led);	 
        usleep(1000 * para.sleepTime); 
  	    clearWS2803();
        usleep(1000 * para.sleepTime);         
      }
	  break;
	case PARAM_SUB_FADE:
	case PARAM_SUB_FADE_CW:
	case PARAM_SUB_FADE_CCW:
	  for(loop=0;loop < para.loopCounter; loop++){
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}
				led[EXTERNAL] = (uint8_t)(0xFF & ((para.pwm/100)*loop2));
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
		  for(loop2=0;loop2<100;loop2++){
				for(index = 0; index < nLEDs; index++){
					led[index] = 0;
				}				
				led[EXTERNAL] = (uint8_t)(0xFF & ((para.pwm/100)*(100-loop2)));
				loadWS2803(led);
				usleep((1000 * para.sleepTime)/(100 * 2));         		  		  
			}
	  }
	  break;  
	default:
	  break;
  }			
}
