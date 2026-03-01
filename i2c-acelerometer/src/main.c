#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#define I2C_DEVICE "/dev/i2c-1"
#define ACCELEROMETER_ADDR 0x68

#define TAG "i2c-acelerometer: "

#define ERROR_CHECK(x, message) \
    do { \
        if (x <0){ \
            printf("\e[31m%s%s\n\e[0m", TAG, message); \
            return -1; \
        } \
    }while (0)

#define PRINT_LOG(message) \
    do { \
        printf("\033[0;32m%s%s\n\e[0m", TAG, message); \
    }while (0)

struct i2c_msg msgs[2];
struct i2c_rdwr_ioctl_data rdwr;
const uint8_t reg_addr = ACCELEROMETER_ADDR; 

int main (void) {
    // Open the I2C device
    int i2c_fd = open(I2C_DEVICE, O_RDWR);
    ERROR_CHECK(i2c_fd, "Failed to open I2C device");

    // set register to read from the accelerometer
    msgs[0].addr = ACCELEROMETER_ADDR;
    msgs[0].flags = 0; // Write
    msgs[0].len = 1;
    msgs[0].buf = (uint8_t *)&reg_addr;

    // Prepare to read data from the accelerometer
    uint8_t data;
    msgs[1].addr = ACCELEROMETER_ADDR;
    msgs[1].flags = I2C_M_RD; // Read
    msgs[1].len = 1;
    msgs[1].buf = &data;

    // Set up the ioctl data structure
    rdwr.msgs = msgs;
    rdwr.nmsgs = 2;     

    // Perform the I2C transaction
    int ret = ioctl(i2c_fd, I2C_RDWR, &rdwr);
    ERROR_CHECK(ret, "Failed to perform I2C transaction");

    // Print the read data
    data = data >> 1;
    printf("Read data: %d\n", data);
    
    close(i2c_fd);
    return 0;
}