#include "pwm.h"
#include "driver_init.h"

static subbus_cache_word_t pwm_cache[PWM_HIGH_ADDR-PWM_BASE_ADDR+1] = {
  // Value, Wvalue, readable, was_read, writable, written, dynamic
  // PWM Status PWM_STATUS_NREGS
  { 0, 0, true,  false, true,  false, false }, // Offset 0: RW: Command/Status
  { 0, 0, true,  false, true,  false, false }, // Offset 1: RW: PWM Period
  { 0, 0, true,  false, true,  false, false }, // Offset 2: RW: PWM Duty Cycle
  { 0, 0, true,  false, false, false, false }, // Offset 3: RW: PWM Proportional Gain
  { 0, 0, true,  false, false, false, false }, // Offset 0: RW: PWM Integral Gain
  { 0, 0, true,  false, false, false, false }, // Offset 0: R:  PWM Differential Gain
};

static void pwm_reset(void) {
  pwm_disable(&PWM_CTRL);
  subbus_write(PWM_BASE_ADDR+1, PWM_PERIOD_DEFAULT);
  subbus_write(PWM_BASE_ADDR+2, PWM_DUTY_CYCLE_DEFAULT);
}

static void pwm_poll(void) {
  uint16_t new_period, new_duty_cycle;
  uint16_t new_cmd_stat;
  new_period = pwm_cache[1].cache;
  new_duty_cycle = pwm_cache[2].cache;
  if (sb_cache_iswritten(pwm_cache, 1, &new_period) ||
      sb_cache_iswritten(pwm_cache, 2, &new_duty_cycle)) {
    pwm_set_parameters(&PWM_CTRL, (const pwm_period_t)new_period,
          (const pwm_period_t) new_duty_cycle);
    sb_cache_update(pwm_cache, 1, new_period);
    sb_cache_update(pwm_cache, 2, new_duty_cycle);
  }
  if (sb_cache_iswritten(pwm_cache, 0, &new_cmd_stat)) {
    if (new_cmd_stat & 1) {
      pwm_enable(&PWM_CTRL);
    } else {
      pwm_disable(&PWM_CTRL);
    }
    // Check for PID control here
    sb_cache_update(pwm_cache, 0, new_cmd_stat & 3); // mask off unused bits
  }
}

subbus_driver_t sb_pwm = {
  PWM_BASE_ADDR, PWM_HIGH_ADDR, // address range
  pwm_cache,
  pwm_reset,
  pwm_poll,
  0, // Dynamic function
  false // initialized
};
