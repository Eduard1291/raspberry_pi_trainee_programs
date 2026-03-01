#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "i2c_driver.h"
#include "log.h"

#define I2C_DEVICE "/dev/i2c-1"
#define ACCELEROMETER_ADDR 0x68

#define TAG "i2c-acelerometer: "



struct i2c_msg msgs[2];
struct i2c_rdwr_ioctl_data rdwr;
const uint8_t reg_addr = 0x0F; 

int main (void) {
    
    i2c_error_t error = i2c_init(I2C_DEVICE, ACCELEROMETER_ADDR);
    if (error != I2C_SUCCESS) {
        return -1;
    }

    uint8_t data;
    error = i2c_read(ACCELEROMETER_ADDR, reg_addr, &data, 1);
    if (error != I2C_SUCCESS) {
        ERROR_LOG(TAG, "Failed to read from I2C device");
        i2c_deinit();
        return -1;
    }

    data = data >> 1;
    INFO_LOG(TAG, "Read data: %X", data);

    i2c_deinit();
    return 0;
}