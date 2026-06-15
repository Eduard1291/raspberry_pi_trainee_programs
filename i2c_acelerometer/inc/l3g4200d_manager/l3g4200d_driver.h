#ifndef L3G4200D_DRIVER_H
#define L3G4200D_DRIVER_H

/*includes----------------------------------------------*/

#include <stdint.h>
#include "l3g4200d.h"
#include "stdbool.h"

/*defines-----------------------------------------------*/

#define L3G4200D_SET_DEFAULT_CONFIG { \
    .i2c_device = "/dev/i2c-1", \
    .device_addr = 0x68, /* Default I2C address */ \
    .dr_bw = DRBW_100HZ_12_5, \
    .pd_mode = POWER_NORMAL, \
    .hpf_mode = HPF_MODE_NORMAL_RESET, \
    .hpf_cutoff = HPF_CUTOFF_0000, \
    .int_cfg = 0x00, /* No interrupts enabled by default */ \
    .full_scale = 0x00, /* Full scale selection: 250 dps by default */ \
    .output_routing = 0x00, /* Output routing: Default value (all outputs routed to the same pins) */ \
    .reference_int = 0x00 /* Reference value for interrupt generation: Default value (0) */ \
}

/*typedefs----------------------------------------------*/

/**
 * @brief This structure defines the configuration settings for initializing the L3G4200D gyroscope sensor. 
 * It includes fields for data rate and bandwidth, power-down mode, high-pass filter settings, 
 * interrupt configuration, full-scale selection, output routing, and reference value for interrupt generation. 
 * This structure is used to configure the sensor according to the desired application requirements during initialization.
 */
typedef struct l3g4200d_init_config
{
    const char *i2c_device; /**< I2C device file (e.g., "/dev/i2c-1") */
    uint8_t device_addr; /**< I2C address of the L3G4200D sensor */
    l3g4200d_dr_bw_t dr_bw; /**< Data rate and bandwidth configuration */
    l3g4200d_pd_mode_t pd_mode; /**< Power-down mode configuration */
    l3g4200d_hpf_mode_t hpf_mode; /**< High-pass filter mode */
    l3g4200d_hpf_cutoff_t hpf_cutoff; /**< High-pass filter cutoff frequency */
    uint8_t int_cfg; /**< Interrupt configuration (CTRL_REG3 settings). using the defined macros for CTRL_REG3 settings in l3g4200d.h */
    uint8_t full_scale; /**< Full-scale selection (CTRL_REG4 settings). using the defined macros for CTRL_REG4 settings in l3g4200d.h */
    uint8_t output_routing; /**< Output routing configuration (CTRL_REG5 settings). using the defined macros for CTRL_REG5 settings in l3g4200d.h */
    uint8_t reference_int; /**< Reference value for interrupt generation (REFERENCE register) */
} l3g4200d_init_config_t;

/**
 * @brief Temperature data structure
 */
typedef struct l3g4200d_temperature_data
{
    int temp_celsius; /**< Temperature in degrees Celsius (8-bit signed value) */
} l3g4200d_temperature_data_t;

/**
 * @brief Angular velocity data structure
 */
typedef struct l3g4200d_angular_velocity_data
{
    float x; /**< X-axis angular velocity data (16-bit signed value) */
    float y; /**< Y-axis angular velocity data (16-bit signed value) */
    float z; /**< Z-axis angular velocity data (16-bit signed value) */
} l3g4200d_angular_velocity_data_t;


/**
 * @brief This structure defines the status of the L3G4200D gyroscope sensor. 
 * It includes boolean fields that indicate various status conditions of the sensor, 
 * such as data overrun conditions for each axis (X, Y, Z) and the availability of new data for each axis. 
 * The status information is typically read from the STATUS_REG register of the sensor and can be used to determine 
 * if new data is available for reading or if any data has been overwritten due to overrun conditions.
 */
typedef struct l3g4200d_status
{
    bool zyxor; /**< X, Y, Z axis data overrun. Set to 1 if new data has overwritten the previous unread data in the output registers. */
    bool zor; /**< Z-axis data overrun. Set to 1 if new Z-axis data has overwritten the previous unread Z-axis data in the output registers. */
    bool yor; /**< Y-axis data overrun. Set to 1 if new Y-axis data has overwritten the previous unread Y-axis data in the output registers. */
    bool xor; /**< X-axis data overrun. Set to 1 if new X-axis data has overwritten the previous unread X-axis data in the output registers. */
    bool zyxda; /**< X, Y, Z axis new data available. Set to 1 when new data for all axes (X, Y, Z) is available in the output registers. */
    bool zda; /**< Z-axis new data available. Set to 1 when new Z-axis data is available in the output registers. */
    bool yda; /**< Y-axis new data available. Set to 1 when new Y-axis data is available in the output registers. */
    bool xda; /**< X-axis new data available. Set to 1 when new X-axis data is available in the output registers. */
} l3g4200d_status_t;


#define FIFO_STREAM_MODE_MAX_SAMPLES 32

/**
 * @brief This structure defines the data read from the FIFO stream mode of the L3G4200D gyroscope sensor.
 */
typedef struct l3g4200d_fifo_stream_data
{
    l3g4200d_angular_velocity_data_t samples[FIFO_STREAM_MODE_MAX_SAMPLES]; /**< Array to store the angular velocity data samples read from the FIFO stream mode. Each sample contains X, Y, and Z axis data. */
    uint8_t num_samples; /**< Number of valid samples in the FIFO stream data arrays */
} l3g4200d_fifo_stream_data_t;

/**
 * @brief This structure defines the configuration settings for the interrupt generation of the L3G4200D gyroscope sensor.
 * It includes boolean fields that specify the conditions for generating interrupts on the INT1 pin, such as the combination of interrupt events (AND/OR), 
 * latching behavior, and enabling of interrupts for high and low events on each axis (X, Y, Z).
 */
typedef struct l3g4200d_int_cfg
{
    bool and_or; /**< AND/OR combination of interrupt events. Set to 1 for AND combination, or 0 for OR combination (default). */
    bool lir; /**< Latch interrupt request. Set to 1 to latch the interrupt request on INT1 pin until it is cleared by reading the INT1_SRC register, or 0 for non-latched (default). */
    bool zhie; /**< Enable interrupt on Z high event. Set to 1 to enable interrupt generation on Z-axis high event, or 0 to disable (default). */
    bool zlie; /**< Enable interrupt on Z low event. Set to 1 to enable interrupt generation on Z-axis low event, or 0 to disable (default). */
    bool yhie; /**< Enable interrupt on Y high event. Set to 1 to enable interrupt generation on Y-axis high event, or 0 to disable (default). */
    bool ylie; /**< Enable interrupt on Y low event. Set to 1 to enable interrupt generation on Y-axis low event, or  0 to disable (default). */
    bool xhie; /**< Enable interrupt on X high event. Set to 1 to enable interrupt generation on X-axis high event, or 0 to disable (default). */
    bool xlie; /**< Enable interrupt on X low event. Set to 1 to enable interrupt generation on X-axis low event, or 0 to disable (default). */
} l3g4200d_int_cfg_t;

typedef struct l3g4200d_interrupt_source
{
    bool all; /**< All interrupt events. Set to 1 if any of the enabled interrupt events has occurred, or 0 if no interrupt event has occurred. */
    bool zh; /**< Z high event. Set to 1 if a Z-axis high event has occurred, or 0 if no Z-axis high event has occurred. */
    bool zl; /**< Z low event. Set to 1 if a Z-axis low event has occurred, or 0 if no Z-axis low event has occurred. */
    bool yh; /**< Y high event. Set to 1 if a Y-axis high event has occurred, or 0 if no Y-axis high event has occurred. */
    bool yl; /**< Y low event. Set to 1 if a Y-axis low event has occurred, or 0 if no Y-axis low event has occurred. */
    bool xh; /**< X high event. Set to 1 if an X-axis high event has occurred, or 0 if no X-axis high event has occurred. */
    bool xl; /**< X low event. Set to 1 if an X-axis low event has occurred, or 0 if no X-axis low event has occurred. */
}l3g4200d_interrupt_source_t;


/**
 * @brief This structure defines the configuration settings for the interrupt duration of the L3G4200D gyroscope sensor.
 */
typedef struct l3g4200d_interrupt_duration
{
    bool wait; /**< if set to 1 the interrupt is generated during the the sample time, if set to 0 the interrupt is generated immediately when the interrupt condition is met */
    uint8_t duration; /**< Duration value for interrupt generation. this value can be set from 0 to 127,  
                      and the actual duration is calculated as follows: Duration = (duration value + 1) * (1 / ODR), 
                      where ODR is the output data rate of the sensor. This means that the duration can be set from 1/ODR to 128/ODR seconds.*/
} l3g4200d_interrupt_duration_t;

typedef enum l3g4200d_error {
    L3G4200D_SUCCESS = 0, /**< Success */
    L3G4200D_ERROR_INIT, /**< Error during initialization */
    L3G4200D_ERROR_DEINIT, /**< Error during deinitialization */
    L3G4200D_ERROR_READ, /**< Error during data reading */
    L3G4200D_ERROR_WRITE /**< Error during data writing */
} l3g4200d_error_t;

/*Prototypes--------------------------------------------*/

/**
 * @brief Initializes the L3G4200D gyroscope sensor with the specified configuration settings.
 * @param config Pointer to a structure containing the configuration settings for initializing the sensor.
 * @return L3G4200D_SUCCESS on success, L3G4200D_ERROR_INIT on failure.
 */
l3g4200d_error_t l3g4200d_init(l3g4200d_init_config_t *config);

/**
 * @brief Deinitializes the L3G4200D gyroscope sensor, resetting it to its default state and freeing any resources used by the driver.
 * @return L3G4200D_SUCCESS on success, L3G4200D_ERROR_INIT on failure.
 */
l3g4200d_error_t l3g4200d_deinit(void);

/**
 * @brief Reads the angular velocity data from the L3G4200D gyroscope sensor and stores it in the provided data structure.
 * @param data Pointer to a structure where the read angular velocity data will be stored.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_angular_velocity(l3g4200d_angular_velocity_data_t *data);

/**
 * @brief Reads the temperature data from the L3G4200D gyroscope sensor and stores it in the provided data structure.
 * @param data Pointer to a structure where the read temperature data will be stored.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_temperature(l3g4200d_temperature_data_t *data);

/**
 * @brief Reads the status information from the L3G4200D gyroscope sensor and stores it in the provided status structure.
 * @param status Pointer to a structure where the read status information will be stored.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_status(l3g4200d_status_t *status);

/**
 * @brief Enables or disables the FIFO stream mode in the L3G4200D gyroscope sensor.
 * @param enable Boolean indicating whether to enable (true) or disable (false) the FIFO stream mode.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_WRITE).
 */
l3g4200d_error_t l3g4200d_fifo_stream_mode(bool enable);

/**
 * @brief Reads the FIFO source information from the L3G4200D gyroscope sensor and stores it in the provided FIFO source structure.
 * @param fifo_src Pointer to a structure where the read FIFO source information will be stored.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_READ).
 */
l3g4200d_error_t l3g4200d_read_fifo_in_stream_mode(l3g4200d_fifo_stream_data_t *fifo_stream_data);

/**
 * @brief Configures the interrupt settings for the L3G4200D gyroscope sensor.
 * @param int_cfg Pointer to a structure containing the interrupt configuration settings.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_WRITE).
 */
l3g4200d_error_t l3g4200d_configure_interrupt(l3g4200d_int_cfg_t *int_cfg);

/**
 * @brief Configures the interrupt duration settings for the L3G4200D gyroscope sensor.
 * @param int_duration Pointer to a structure containing the interrupt duration configuration settings.
 * @return L3G4200D_SUCCESS on success, otherwise returns an appropriate error code indicating the type of failure (e.g., L3G4200D_ERROR_WRITE).
 */
l3g4200d_error_t l3g4200d_configure_interrupt_duration(l3g4200d_interrupt_duration_t *int_duration);

#endif // L3G4200D_DRIVER_H