
/*includes----------------------------------------------*/
#include "l3g4200d_driver.h"
#include "i2c_driver.h"
#include "log.h"

/*defines-----------------------------------------------*/

#define ERROR_CHECK(x, message, return_value) \
    do { \
        if (x != L3G4200D_SUCCESS){ \
            ERROR_LOG(TAG, message); \
            return return_value; \
        } \
    }while (0)

#define ANGULAR_VELOCITY_BUFFER_SIZE 6
#define FIFO_SAMPLE_COUNT 32
#define FIFO_WATERMARK_LEVEL 0x1F
#define FIFO_SRC_WTM_MASK SET_BIT(7)
#define FIFO_SRC_OVRN_MASK SET_BIT(6)
#define FIFO_SRC_EMPTY_MASK SET_BIT(5)
#define FIFO_SRC_STORED_DATA_MASK 0x1F

/*typedefs----------------------------------------------*/


/*variables---------------------------------------------*/

static const char *TAG = "l3g4200d_driver";
uint8_t device_addr; /**< I2C address of the L3G4200D sensor */
uint8_t full_scale; /**< Full-scale selection for the sensor, used to calculate the sensitivity and convert raw data to physical units. This variable is set during initialization based on the configuration provided by the user, and it determines the range of angular velocity measurements that the sensor can capture. The value of Full_scale is used in the conversion formulas to translate raw sensor readings into meaningful angular velocity values in degrees per second (dps) or radians per second (rad/s), depending on the full-scale setting selected. */

const float full_scale_sensitivity[4] = {0.00875f, 0.0175f, 0.07f, 0.07f}; /**< Sensitivity values for different full-scale settings of the L3G4200D sensor, used to convert raw angular velocity data to physical units (dps or rad/s). The sensitivity values correspond to the full-scale selections of 250 dps, 500 dps, and 2000 dps, respectively. These values are used in the conversion formulas to calculate the actual angular velocity from the raw sensor readings based on the selected full-scale setting. */
const char *full_scale_range[4] = {"250 dps", "500 dps", "2000 dps", "2000 dps"}; /**< String representations of the full-scale ranges corresponding to the different full-scale settings of the L3G4200D sensor. These strings are used for logging and debugging purposes to indicate the current full-scale range configuration of the sensor, making it easier to understand the context of the angular velocity measurements being read from the sensor. */

/*Prototypes--------------------------------------------*/

l3g4200d_error_t l3g4200d_write_register(uint8_t reg_addr, uint8_t data);
l3g4200d_error_t l3g4200d_write_multiple_register(uint8_t reg_addr, uint8_t *data, uint8_t len);
l3g4200d_error_t l3g4200d_read_register(uint8_t reg_addr, uint8_t *data);
l3g4200d_error_t l3g4200d_read_multiple_register(uint8_t reg_addr, uint8_t *data, uint8_t len);
l3g4200d_error_t l3g4200d_config(l3g4200d_init_config_t *config);


/*private functions---------------------------------------------*/

/**
 * @brief Writes a byte of data to a specific register of the L3G4200D sensor.
 * @param reg_addr The register address to write to.
 * @param data The byte of data to write.
 * @return L3G4200D_SUCCESS on success, or an appropriate error code on failure.
 */
l3g4200d_error_t l3g4200d_write_register(uint8_t reg_addr, uint8_t data){
    if (i2c_write(device_addr, reg_addr, &data, 1) != I2C_SUCCESS) {
        return L3G4200D_ERROR_WRITE;
    }
    return L3G4200D_SUCCESS;
}

/**
 * @brief Writes multiple bytes of data to consecutive registers of the L3G4200D sensor, 
 * starting from a specified register address.
 * @param reg_addr The register address to start writing from.
 * @param data Pointer to the array of bytes to write.
 * @param len The number of bytes to write.
 * @return L3G4200D_SUCCESS on success, or an appropriate error code on failure.
 */
l3g4200d_error_t l3g4200d_write_multiple_register(uint8_t reg_addr, uint8_t *data, uint8_t len){
    if (i2c_write(device_addr, reg_addr, data, len) != I2C_SUCCESS) {
        return L3G4200D_ERROR_WRITE;
    }
    return L3G4200D_SUCCESS;
}

/**
 * @brief Reads a byte of data from a specific register of the L3G4200D sensor.
 * @param reg_addr The register address to read from.
 * @param data Pointer to a variable where the read byte will be stored.
 * @return L3G4200D_SUCCESS on success, or an appropriate error code on failure.
 */
l3g4200d_error_t l3g4200d_read_register(uint8_t reg_addr, uint8_t *data){
    if (i2c_read(device_addr, reg_addr, data, 1) != I2C_SUCCESS) {
        return L3G4200D_ERROR_READ;
    }
    return L3G4200D_SUCCESS;
}

/**
 * @brief Reads multiple bytes of data from consecutive registers of the L3G4200D sensor, 
 * starting from a specified register address.
 * @param reg_addr The register address to start reading from.
 * @param data Pointer to the array where the read bytes will be stored.
 * @param len The number of bytes to read.
 * @return L3G4200D_SUCCESS on success, or an appropriate error code on failure.
 */
l3g4200d_error_t l3g4200d_read_multiple_register(uint8_t reg_addr, uint8_t *data, uint8_t len){
    if (i2c_read(device_addr, reg_addr, data, len) != I2C_SUCCESS) {
        return L3G4200D_ERROR_READ;
    }
    return L3G4200D_SUCCESS;
}

/**
 * @brief Configures the L3G4200D sensor with the specified settings, 
 * including data rate, bandwidth, power-down mode, high-pass filter settings, 
 * interrupt configuration, full-scale selection, output routing, 
 * and reference value for interrupt generation.
 * @param config Pointer to a structure containing the configuration settings for the sensor.
 * @return L3G4200D_SUCCESS on success, or an appropriate error code on failure.
 */
l3g4200d_error_t l3g4200d_config(l3g4200d_init_config_t *config){
    // configure CTRL_REG1 with data rate, bandwidth, and power-down mode settings
    uint8_t ctrl_reg1_value = (config->dr_bw << 4) | (config->pd_mode & 0x0F);
    ERROR_CHECK(l3g4200d_write_register(CTRL_REG1, ctrl_reg1_value), 
                "Failed to write CTRL_REG1", 
                L3G4200D_ERROR_INIT);
    // configure CTRL_REG2 with high-pass filter settings
    uint8_t ctrl_reg2_value = (config->hpf_mode << 4) | (config->hpf_cutoff & 0x0F);
    ERROR_CHECK(l3g4200d_write_register(CTRL_REG2, ctrl_reg2_value), 
                "Failed to write CTRL_REG2", 
                L3G4200D_ERROR_INIT);
    // configure CTRL_REG3 with interrupt settings
    ERROR_CHECK(l3g4200d_write_register(CTRL_REG3, config->int_cfg), 
                "Failed to write CTRL_REG3", 
                L3G4200D_ERROR_INIT);
    // configure CTRL_REG4 with full-scale selection
    ERROR_CHECK(l3g4200d_write_register(CTRL_REG4, config->full_scale), 
                "Failed to write CTRL_REG4", 
                L3G4200D_ERROR_INIT);
    // configure CTRL_REG5 with output routing settings
    ERROR_CHECK(l3g4200d_write_register(CTRL_REG5, config->output_routing), 
                "Failed to write CTRL_REG5", 
                L3G4200D_ERROR_INIT);
    // configure REFERENCE register with reference value for interrupt generation
    ERROR_CHECK(l3g4200d_write_register(REFERENCE, config->reference_int), 
                "Failed to write REFERENCE", 
                L3G4200D_ERROR_INIT);
    return L3G4200D_SUCCESS;
}

/*public functions---------------------------------------------*/

/**
 * @brief Initializes the L3G4200D gyroscope sensor with the specified configuration settings.
 * @param config Pointer to a structure containing the configuration settings for initializing the sensor.
 * @return L3G4200D_SUCCESS on success, L3G4200D_ERROR_INIT on failure.
 */
l3g4200d_error_t l3g4200d_init(l3g4200d_init_config_t *config){
    device_addr = config->device_addr;
    // Initialize the I2C driver with the specified device and address
    if (i2c_init(config->i2c_device, device_addr) != I2C_SUCCESS) {
        return L3G4200D_ERROR_INIT;
    }
    // Extract the full-scale selection from the configuration (assuming it's in the upper 2 bits of the full_scale field)
    full_scale = (config->full_scale >> 4) & 0x03;
    INFO_LOG(TAG, "Initializing L3G4200D with full-scale range: %s", full_scale_range[full_scale]);
    return l3g4200d_config(config);
}

/**
 * @brief Deinitializes the L3G4200D gyroscope sensor, resetting it to its default state and freeing any resources used by the driver.
 * @return L3G4200D_SUCCESS on success, L3G4200D_ERROR_INIT on failure.
 */
l3g4200d_error_t l3g4200d_deinit(void){
    // Write to the BOOT bit in CTRL_REG5 to reset the memory content and restore default values in all registers except the WHO_AM_I register. 
    // This effectively deinitializes the sensor and returns it to its default
    ERROR_CHECK(l3g4200d_write_register(CTRL_REG5, CTRL_REG5_BOOT), 
                "Failed to write CTRL_REG5", 
                L3G4200D_ERROR_DEINIT);
    i2c_deinit();
    return L3G4200D_SUCCESS;
}

/**
 * @brief Reads the angular velocity data from the L3G4200D gyroscope sensor and stores it in the provided data structure.
 * @param data Pointer to a structure where the read angular velocity data will be stored.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_angular_velocity(l3g4200d_angular_velocity_data_t *data){
    uint8_t buffer[ANGULAR_VELOCITY_BUFFER_SIZE];
    ERROR_CHECK(l3g4200d_read_multiple_register(OUT_X_L, buffer, ANGULAR_VELOCITY_BUFFER_SIZE),
                "Failed to read angular velocity data", 
                L3G4200D_ERROR_READ);
    data->x = (int16_t)(buffer[1] << 8 | buffer[0]) * full_scale_sensitivity[full_scale];
    data->y = (int16_t)(buffer[3] << 8 | buffer[2]) * full_scale_sensitivity[full_scale];
    data->z = (int16_t)(buffer[5] << 8 | buffer[4]) * full_scale_sensitivity[full_scale];
    return L3G4200D_SUCCESS;
}

/**
 * @brief Reads the temperature data from the L3G4200D gyroscope sensor and stores it in the provided data structure.
 * @param data Pointer to a structure where the read temperature data will be stored.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_temperature(l3g4200d_temperature_data_t *data){
    uint8_t temp_celsius;
    ERROR_CHECK(l3g4200d_read_register(OUT_TEMP, &temp_celsius), 
                "Failed to read OUT_TEMP", 
                L3G4200D_ERROR_READ);
    data->temp_celsius = (int)temp_celsius;
    return L3G4200D_SUCCESS;
}

/**
 * @brief Reads the status information from the L3G4200D gyroscope sensor and stores it in the provided status structure.
 * @param status Pointer to a structure where the read status information will be stored.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_status(l3g4200d_status_t *status){
    uint8_t status_reg;
    ERROR_CHECK(l3g4200d_read_register(STATUS_REG, &status_reg), 
                "Failed to read STATUS_REG", 
                L3G4200D_ERROR_READ);
    status->zyxor = (status_reg & 0x80) != 0;
    status->zor = (status_reg & 0x40) != 0;
    status->yor = (status_reg & 0x20) != 0;
    status->xor = (status_reg & 0x10) != 0;
    status->zyxda = (status_reg & 0x08) != 0;
    status->zda = (status_reg & 0x04) != 0;
    status->yda = (status_reg & 0x02) != 0;
    status->xda = (status_reg & 0x01) != 0;
    return L3G4200D_SUCCESS;
}

/**
 * @brief Enables or disables the FIFO stream mode in the L3G4200D gyroscope sensor.
 * @param enable Boolean indicating whether to enable (true) or disable (false) the FIFO stream mode.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_WRITE).
 */
l3g4200d_error_t l3g4200d_fifo_stream_mode(bool enable){
    uint8_t ctrl_reg5_value;
    uint8_t fifo_ctrl_value;
    uint8_t fifo_mode_bits;

    ERROR_CHECK(l3g4200d_read_register(CTRL_REG5, &ctrl_reg5_value), 
                "Failed to read CTRL_REG5",
                L3G4200D_ERROR_READ);
    if (enable) {
        // Set the FIFO enable bit in CTRL_REG5 to enable FIFO functionality. 
        fifo_mode_bits = ctrl_reg5_value | CTRL_REG5_FIFO_ENABLE;
        fifo_ctrl_value = (FIFO_MODE_STREAM << 5) | FIFO_WATERMARK_LEVEL;
    } else {
        // Set the FIFO disable bit in CTRL_REG5 to enable FIFO functionality
        fifo_mode_bits = ctrl_reg5_value & ~CTRL_REG5_FIFO_ENABLE;
        fifo_ctrl_value = FIFO_MODE_BYPASS << 5;
    }

    ERROR_CHECK(l3g4200d_write_register(CTRL_REG5, fifo_mode_bits), 
                "Failed to write CTRL_REG5", 
                L3G4200D_ERROR_WRITE);
    ERROR_CHECK(l3g4200d_write_register(FIFO_CTRL_REG, fifo_ctrl_value), 
                "Failed to write FIFO_CTRL_REG", 
                L3G4200D_ERROR_WRITE);
    return L3G4200D_SUCCESS;
}

/**
 * @brief Reads the FIFO source information from the L3G4200D gyroscope sensor and stores it in the provided FIFO source structure.
 * @param fifo_src Pointer to a structure where the read FIFO source information will be stor   ed.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_fifo_in_stream_mode(l3g4200d_fifo_stream_data_t *fifo_stream_data) {
    uint8_t fifo_src_reg;
    uint8_t stored_data;

    ERROR_CHECK(l3g4200d_read_register(FIFO_SRC_REG, &fifo_src_reg),
                "Failed to read FIFO_SRC_REG",
                L3G4200D_ERROR_READ);

    fifo_stream_data->num_samples = 0;

    if ((fifo_src_reg & FIFO_SRC_EMPTY_MASK) != 0) {
        return L3G4200D_SUCCESS;
    }

    stored_data = fifo_src_reg & FIFO_SRC_STORED_DATA_MASK;
    fifo_stream_data->num_samples = stored_data;

    for (uint8_t i = 0; i < fifo_stream_data->num_samples; i++) {
        ERROR_CHECK(l3g4200d_read_angular_velocity(&fifo_stream_data->samples[i]),
                    "Failed to read FIFO angular velocity data",
                    L3G4200D_ERROR_READ);
    }

    return L3G4200D_SUCCESS;
}

/**
 * @brief Configures the interrupt settings for the L3G4200D gyroscope sensor.
 * @param int_cfg Pointer to a structure containing the interrupt configuration settings.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_WRITE).
 */
l3g4200d_error_t l3g4200d_configure_interrupt(l3g4200d_int_cfg_t *int_cfg)  {
    
}

/**
 * @brief Configures the interrupt duration settings for the L3G4200D gyroscope sensor.
 * @param int_duration Pointer to a structure containing the interrupt duration configuration settings.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_WRITE).
 */
l3g4200d_error_t l3g4200d_configure_interrupt_duration(l3g4200d_interrupt_duration_t *int_duration) {
    
}
