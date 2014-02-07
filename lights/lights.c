#include <stdlib.h>
#include <stdint.h>
#include "parameter.h"
#include "led.h"
#include "helper.h"
#include "lights.h"
#include "spi.h"
#include "anim.h"

int main(int argc, char *argv[]){

  params para;

  if(init_spi() == -1){
    pabort("can't initialize ISP");
  }
  clearWS2803();
  if(argc != 7){
    pabort("Wrong amount of parameters");
  }
  
  para.master      = atoi(argv[PARAM_MASTER]);
  para.sub         = atoi(argv[PARAM_SUB]);
  para.pwm         = atoi(argv[PARAM_PWM]);
  para.loopCounter = atoi(argv[PARAM_LOOPCOUNTER]);
  para.sleepTime   = atoi(argv[PARAM_SLEEPTIME]);
  para.remainOn    = atoi(argv[PARAM_ON_OFF]);

  switch(para.master){
    case PARAM_MASTER_ALL:
      anim_all(para);
      if(para.remainOn == 1) {
       para.sub = PARAM_SUB_ON;
       anim_all(para);
      }
      break;
    case PARAM_MASTER_CIRCLE:
      anim_circle(para);
      if(para.remainOn == 1) {
       para.sub = PARAM_SUB_ON;
       anim_circle(para);
      }
      break;
    case PARAM_MASTER_SURROUND:
      anim_suround(para);
      if(para.remainOn == 1) {
       para.sub = PARAM_SUB_ON;
       anim_suround(para);
      }
      break;
    case PARAM_MASTER_HEAD:
      anim_head(para);
      if(para.remainOn == 1) {
       para.sub = PARAM_SUB_ON;
       anim_head(para);
      }
      break;
    case PARAM_MASTER_EXTERNAL:
      anim_external(para);
      if(para.remainOn == 1) {
       para.sub    = PARAM_SUB_ON;
       anim_external(para);
      }
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
