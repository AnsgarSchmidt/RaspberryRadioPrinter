#ifndef PARAMETER_H_
#define PARAMETER_H_

#define PARAM_APPNAME     0
#define PARAM_MASTER      1
#define PARAM_SUB         2
#define PARAM_PWM         3
#define PARAM_LOOPCOUNTER 4
#define PARAM_SLEEPTIME   5
#define PARAM_ON_OFF      6

#define PARAM_MASTER_ALL      0
#define PARAM_MASTER_CIRCLE   1
#define PARAM_MASTER_SURROUND 2
#define PARAM_MASTER_HEAD     3
#define PARAM_MASTER_EXTERNAL 4

#define PARAM_SUB_OFF                0
#define PARAM_SUB_ON                 1
#define PARAM_SUB_BLINK              2
#define PARAM_SUB_CLOCK_WISE         3
#define PARAM_SUB_COUNTER_CLOCK_WISE 4
#define PARAM_SUB_FADE               5
#define PARAM_SUB_FADE_CW            6
#define PARAM_SUB_FADE_CCW           7
#define PARAM_SUB_UP                 8
#define PARAM_SUB_FADE_UP            9

typedef struct{
	uint8_t  master;
	uint16_t sub;
	uint16_t pwm;
	uint16_t loopCounter;
	uint32_t sleepTime;
	uint8_t  remainOn;
} params;

#endif
