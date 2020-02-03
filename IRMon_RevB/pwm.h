#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED
#include "subbus.h"

#define PWM_BASE_ADDR 0x40
#define PWM_NREGS 6
#define PWM_HIGH_ADDR (PWM_BASE_ADDR+PWM_NREGS-1)
#define PWM_ENABLE_DEFAULT true
#define PWM_PERIOD_DEFAULT 40000
#define PWM_DUTY_CYCLE_DEFAULT 10000

extern subbus_driver_t sb_pwm;

#endif
