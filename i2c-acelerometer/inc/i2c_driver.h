#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

/*includes----------------------------------------------*/

#include <stdint.h>

/*typedefs----------------------------------------------*/

typedef enum i2c_error {
    I2C_SUCCESS = 0,
    I2C_ERROR_OPEN,
    I2C_ERROR_IOCTL
}i2c_error_t;

/*public functions--------------------------------------*/

/**
 * @brief Initializes the I2C driver by opening the specified I2C device.
 * @param device The path to the I2C device (e.g., "/dev/i2c-1").
 * @param device_addr The I2C address of the target device.
 * @return I2C_SUCCESS on success, or an appropriate error code on failure.
 */
i2c_error_t i2c_init(const char *device, uint8_t device_addr);

/**
 * @brief Sets the I2C address for subsequent read/write operations.
 * @param device_addr The I2C address of the target device.
 */
void i2c_set_device_address(uint8_t device_addr);

/**
 * @brief Writes a byte of data to a specific register of an I2C device.
 * @param device_addr The I2C address of the target device.
 * @param reg_addr The register address to write to.
 * @param data The byte of data to write.
 * @param len The length of the data to write (in bytes).
 * @return I2C_SUCCESS on success, or an appropriate error code on failure.
 */
i2c_error_t i2c_write(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

/**
 * @brief Reads a byte of data from a specific register of an I2C device.
 * @param device_addr The I2C address of the target device.
 * @param reg_addr The register address to read from.
 * @param data Pointer to a variable where the read byte will be stored.
 * @param len The length of the data to read (in bytes).
 * @return I2C_SUCCESS on success, or an appropriate error code on failure.
 */
i2c_error_t i2c_read(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

/**
 * @brief Deinitializes the I2C driver by closing the I2C device file descriptor.
 * This function deinitializes the I2C driver by closing the file descriptor associated with the I2C device.
 * It should be called when the I2C operations are no longer needed to free up system resources.
 */
void i2c_deinit(void);

#endif // I2C_DRIVER_H
