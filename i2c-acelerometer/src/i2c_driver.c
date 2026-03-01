
/*includes-----------------------------------------*/

#include "i2c_driver.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "log.h"
#include <fcntl.h>
#include <string.h>

/*defines-----------------------------------------------*/

#define TAG "i2c_driver"

#define READ_TRANSFER_MSGS 2
#define WRITE_TRANSFER_MSGS 1

// Macro to check for errors and log them
#define ERROR_CHECK(x, message, return_value) \
    do { \
        if (x <0){ \
            ERROR_LOG(TAG, message); \
            return return_value; \
        } \
    }while (0)

// Macro to set up an I2C message for a transaction
#define SET_TRANSACTION_MSG(msg, device_addr, msg_flags, buff_len, data) \
    do { \
        (msg).addr = device_addr; \
        (msg).flags = msg_flags; \
        (msg).len = buff_len; \
        (msg).buf = data; \
    }while (0)

// Macro to clear the rdwr structure after a transaction
#define CLEAR_RDWR_STRUCT(i2c_driver) \
    do { \
        (i2c_driver).rdwr.msgs = NULL; \
        (i2c_driver).rdwr.nmsgs = 0; \
    }while (0)

/*typedefs----------------------------------------------*/

typedef struct i2c_driver {
    int fd; // File descriptor for the I2C device
    struct i2c_rdwr_ioctl_data rdwr; // Structure for I2C read/write operations
    uint8_t device_addr; // The I2C address of the target device
} i2c_driver_t;

/*variables---------------------------------------------*/

// Global instance of the I2C driver
i2c_driver_t i2c_driver_handler;

/*public functions--------------------------------------*/

/**
 * @brief Initializes the I2C driver by opening the specified I2C device.
 * @param device The path to the I2C device (e.g., "/dev/i2c-1").
 * @return I2C_SUCCESS on success, or an appropriate error code on failure.
 */
i2c_error_t i2c_init(const char *device, uint8_t device_addr){
    // Open the I2C device
    i2c_driver_handler.fd = open(device, O_RDWR);
    ERROR_CHECK(i2c_driver_handler.fd, "Failed to open I2C device", I2C_ERROR_OPEN);
    // Set the I2C slave address for subsequent operations
    i2c_driver_handler.device_addr = device_addr;
    INFO_LOG(TAG, "I2C driver initialized successfully");
    return I2C_SUCCESS;
}

/**
 * @brief Sets the I2C address for subsequent read/write operations.
 * @param device_addr The I2C address of the target device.
 */
void i2c_set_device_address(uint8_t device_addr){
    i2c_driver_handler.device_addr = device_addr;
    INFO_LOG(TAG, "I2C device address set successfully");
}

/**
 * @brief Writes a byte of data to a specific register of an I2C device.
 * @param device_addr The I2C address of the target device.
 * @param reg_addr The register address to write to.
 * @param data The byte of data to write.
 * @param len The length of the data to write (in bytes).
 * @return I2C_SUCCESS on success, or an appropriate error code on failure.
 */
i2c_error_t i2c_write(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len){
    struct i2c_msg transaction_msg[WRITE_TRANSFER_MSGS];
    uint8_t buffer[len + 1];
    buffer[0] = reg_addr; // First byte is the register address
    memcpy(buffer + 1, data, len); // Copy the data to be written after the register address

    // Set up the I2C message for writing
    SET_TRANSACTION_MSG(transaction_msg[0], device_addr, 0, len + 1, buffer);

    // Set up the ioctl data structure
    i2c_driver_handler.rdwr.msgs = transaction_msg;
    i2c_driver_handler.rdwr.nmsgs = WRITE_TRANSFER_MSGS;

    // Perform the I2C transaction
    int result = ioctl(i2c_driver_handler.fd, I2C_RDWR, &i2c_driver_handler.rdwr);
    ERROR_CHECK(result, "Failed to perform I2C write transaction", I2C_ERROR_IOCTL);

    // Clear the rdwr structure for future transactions
    CLEAR_RDWR_STRUCT(i2c_driver_handler);

    return I2C_SUCCESS;
}

/**
 * @brief Reads a byte of data from a specific register of an I2C device.
 * @param device_addr The I2C address of the target device.
 * @param reg_addr The register address to read from.
 * @param data Pointer to a variable where the read byte will be stored.
 * @param len The length of the data to read (in bytes).
 * @return I2C_SUCCESS on success, or an appropriate error code on failure.
 */
i2c_error_t i2c_read(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len){
    struct i2c_msg transaction_msg[WRITE_TRANSFER_MSGS];

    // Set up the I2C message for writing the register address
    SET_TRANSACTION_MSG(transaction_msg[0], device_addr, 0, 1, &reg_addr);
    // Set et the I2C message for reading the data
    SET_TRANSACTION_MSG(transaction_msg[1], device_addr, I2C_M_RD, len, data);
    // Set up the ioctl data structure
    i2c_driver_handler.rdwr.msgs = transaction_msg;
    i2c_driver_handler.rdwr.nmsgs = READ_TRANSFER_MSGS;

    // Perform the I2C transaction
    int result = ioctl(i2c_driver_handler.fd, I2C_RDWR, &i2c_driver_handler.rdwr);
    ERROR_CHECK(result, "Failed to perform I2C read transaction", I2C_ERROR_IOCTL);

    // Clear the rdwr structure for future transactions
    CLEAR_RDWR_STRUCT(i2c_driver_handler);
    return I2C_SUCCESS;
}

/**
 * @brief Deinitializes the I2C driver by closing the I2C device file descriptor.
 * This function deinitializes the I2C driver by closing the file descriptor associated with the I2C device.
 * It should be called when the I2C operations are no longer needed to free up system resources.
 */
void i2c_deinit(void){
    close(i2c_driver_handler.fd);
    INFO_LOG(TAG, "I2C driver deinitialized successfully");
}
