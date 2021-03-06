#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED
#include "subbus.h"

#define I2C_BASE_ADDR 0x20
#define I2C_STATUS_OFFSET 0x00
#define I2C_STATUS_NREGS 1
#define I2C_ADS_OFFSET (I2C_STATUS_OFFSET+I2C_STATUS_NREGS)
#define I2C_ADS_NREGS 3
#define I2C_LTR_OFFSET (I2C_ADS_OFFSET+I2C_ADS_NREGS)
#define I2C_LTR_NREGS 4
#define I2C_MPL_OFFSET (I2C_LTR_OFFSET+I2C_LTR_NREGS)
#define I2C_MPL_NREGS 3
#define I2C_NREGS (I2C_MPL_OFFSET+I2C_MPL_NREGS)
#define I2C_HIGH_ADDR (I2C_BASE_ADDR+I2C_NREGS-1)
#define I2C_ENABLE_DEFAULT true

extern subbus_driver_t sb_i2c;
void i2c_enable(bool value);

#endif
