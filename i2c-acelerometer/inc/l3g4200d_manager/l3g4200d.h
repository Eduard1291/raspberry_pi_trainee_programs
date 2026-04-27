#ifndef L3G4200D_H
#define L3G4200D_H

/*includes----------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

/*defines-----------------------------------------------*/

#define SET_BIT(x) (1 << (x))

#define TEMPERATURE_OFFSET 25 // The L3G4200D outputs a value of 0 at 25 degrees Celsius, so we need to add this offset to get the actual temperature in degrees Celsius

/**
 * @brief Macros for setting bits in the CTRL_REG3 register of the L3G4200D gyroscope.
 *  +-------+-------+----------+-------+--------+--------+--------+--------+
 *  |  7    |  6    |    5     |   4   |   3    |   2    |   1    |   0    |
 *  +-------+-------+----------+-------+--------+--------+--------+--------+
 *  |I1_Int1|I1_Boot|H_Lactive |PP_OD  |I2_DRDY |I2_WTM  |I2_ORun |I2_Empty|
 *  +-------+-------+----------+-------+--------+--------+--------+--------+
*/
#define CTRL_REG3_I1_INT1_EN     SET_BIT(7) // Enable interrupt on INT1 pin
#define CTRL_REG3_I1_BOOT_EN     SET_BIT(6) // Enable boot status on INT1 pin
#define CTRL_REG3_H_LACTIVE_LOW  SET_BIT(5) // Set INT1 pin to active low (default is active high)
#define CTRL_REG3_PP_OD_ODRAIN   SET_BIT(4) // Set PP_OD bit (default is push-pull)
#define CTRL_REG3_I2_DRDY_EN     SET_BIT(3) // Date Ready on DRDY/INT2. Default value 0. (0: Disable; 1: Enable)
#define CTRL_REG3_I2_WTM_EN      SET_BIT(2) // Date Ready on DRDY/INT2. Default value 0. (0: Disable; 1: Enable)
#define CTRL_REG3_I2_ORUN_EN     SET_BIT(1) // FIFO Overrun interrupt on DRDY/INT2 Default value: 0. (0: Disable; 1: Enable)
#define CTRL_REG3_I2_EMPTY_EN    SET_BIT(0) // FIFO Empty interrupt on DRDY/INT2. Default value: 0. (0: Disable; 1: Enable)

/**
 * @brief Macros for setting bits in the CTRL_REG4 register of the L3G4200D gyroscope.
 * +-----+-----+-----+-----+-----+-----+-----+-----+
 * |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 * +-----+-----+-----+-----+-----+-----+-----+-----+
 * | BDU | BLE | FS1 | FS0 |  -  | ST1 | ST0 | SIM |
 * +-----+-----+-----+-----+-----+-----+-----+-----+
 * 
 * BDU :  Block Data Update. Default value: 0 
 * (0: continous update; 1: output registers not updated until MSB and LSB
 * reading) 
 * 
 * BLE : Big/Little Endian Data Selection. Default value 0.
 * (0: Data LSB @ lower address; 1: Data MSB @ lower address)
 * 
 * FS1-FS0 :  Full Scale selection. Default value: 00
 * (00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
 * 
 * ST1-ST0 :  Self Test Enable. Default value: 00
 * (00: Self Test Disabled; Other: See table below)
 * 
 * SIM : SPI Serial Interface Mode selection. Default value: 0
 * (0: 4-wire interface; 1: 3-wire interface).
 * 
 * +-----+-----+-------------------+
 * | ST1 | ST0 | Self Test Mode    |
 * +-----+-----+-------------------+
 * |  0  |  0  | Normal mode       |
 * |  0  |  1  | Self test 0 (+)   |
 * |  1  |  0  | Reserved          |
 * |  1  |  1  | Self test 1 (-)   |
 * +-----+-----+-------------------+
 */

#define CTRL_REG4_BDU_ENABLE        SET_BIT(7) // Enable Block Data Update (BDU)

/* Endianness */
#define CTRL_REG4_BLE_MSB_FIRST     SET_BIT(6) // Set Big Endian mode (MSB at lower address)

/* Full Scale */
#define CTRL_REG4_FS250DPS              0x00 // Full scale selection: 250 dps
#define CTRL_REG4_FS500DPS              0x01 // Full scale selection: 500 dps
#define CTRL_REG4_FS2000DPS             0x02 // Full scale selection: 2000 dps
#define CTRL_REG4_FS2000DPS_ALT         0x03 // Full scale selection: 2000 dps (alternative setting)

/* Self Test bits */
#define CTRL_REG4_NORMAL_MODE             0x00 // Normal mode (self-test disabled)
#define CTRL_REG4_SELF_TEST_0             0x01 // Self test mode 0 (positive sign)
#define CTRL_REG4_SELF_TEST_1             0x03 // Self test mode 1 (negative sign)

/* SPI mode */
#define CTRL_REG4_SIM_3WIRE         SET_BIT(0) // Set 3-wire SPI mode

/**
 * @brief Macros for setting bits in the CTRL_REG5 register of the L3G4200D gyroscope.
 *
 * +-----+--------+----+------+----------+----------+----------+----------+
 * |  7  |   6    | 5  |  4   |    3     |    2     |    1     |    0     |
 * +-----+--------+----+------+----------+----------+----------+----------+
 * |BOOT | FIFO_EN| -- | HPen | INT1_Sel1| INT1_Sel0| Out_Sel1 | Out_Sel0 |
 * +-----+--------+----+------+----------+----------+----------+----------+
 *
 * BOOT :
 * Reboot memory content. Default value: 0
 * (0: normal mode; 1: reboot memory content)
 *
 * FIFO_EN :
 * FIFO enable. Default value: 0
 * (0: FIFO disable; 1: FIFO enable)
 *
 * HPen :
 * High Pass Filter enable. Default value: 0
 * (0: HPF disabled; 1: HPF enabled)
 *
 * INT1_Sel1 - INT1_Sel0 :
 * +-------+----------+----------+--------------------------------------------------+
 * | Hpen  | OUT_SEL1 | OUT_SEL0 | Description                                      |
 * +-------+----------+----------+--------------------------------------------------+
 * |   x   |    0     |    0     | Data in DataReg and FIFO are non-high-pass-      |
 * |       |          |          | filtered                                         |
 * +-------+----------+----------+--------------------------------------------------+
 * |   x   |    0     |    1     | Data in DataReg and FIFO are high-pass-filtered  |
 * +-------+----------+----------+--------------------------------------------------+
 * |   0   |    1     |    x     | Data in DataReg and FIFO are low-pass-filtered   |
 * |       |          |          | by LPF2                                          |
 * +-------+----------+----------+--------------------------------------------------+
 * |   1   |    1     |    x     | Data in DataReg and FIFO are high-pass and       |
 * |       |          |          | low-pass-filtered by LPF2                        |
 * +-------+----------+----------+--------------------------------------------------+
 *
 * Out_Sel1 - Out_Sel0 :
 * +-------+----------+----------+--------------------------------------------------+
 * | Hpen  | INT_SEL1 | INT_SEL2 | Description                                      |
 * +-------+----------+----------+--------------------------------------------------+
 * |   x   |    0     |    0     | Non-high-pass-filtered data are used for         |
 * |       |          |          | interrupt generation                             |
 * +-------+----------+----------+--------------------------------------------------+
 * |   x   |    0     |    1     | High-pass-filtered data are used for interrupt   |
 * |       |          |          | generation                                       |
 * +-------+----------+----------+--------------------------------------------------+
 * |   0   |    1     |    x     | Low-pass-filtered data are used for interrupt    |
 * |       |          |          | generation                                       |
 * +-------+----------+----------+--------------------------------------------------+
 * |   1   |    1     |    x     | High-pass and low-pass-filtered data are used    |
 * |       |          |          | for interrupt generation                         |
 * +-------+----------+----------+--------------------------------------------------+
 *
 * NOTE:
 * Bit 5 is reserved and must be kept at 0.
 */

// Boot
#define CTRL_REG5_BOOT                SET_BIT(7)   // Reboot memory content

// FIFO
#define CTRL_REG5_FIFO_ENABLE         SET_BIT(6)   // Enable FIFO

// High Pass Filter
#define CTRL_REG5_HP_ENABLE           SET_BIT(4)   // Enable High Pass Filter

// INT1 selection configuration (CTRL_REG5 bits 1 and 0)
#define CTRL_REG5_INT1_SEL_0          0x00  // Non-high-pass-filtered data are used for interrupt generation (default)
#define CTRL_REG5_INT1_SEL_1          0x01  // High-pass-filtered data are used for interrupt generation
#define CTRL_REG5_INT1_SEL_2          0x02  // Low-pass-filtered data are used for interrupt generation
#define CTRL_REG5_INT1_SEL_3          0x03  // High-pass and low-pass-filtered data are used for interrupt generation

// Output selection configuration (CTRL_REG5 bits 1 and 0)
#define CTRL_REG5_OUT_SEL_0           0x00  // Data in DataReg and FIFO are non-high-pass-filtered (default)
#define CTRL_REG5_OUT_SEL_1           0x01  // Data in DataReg and FIFO are high-pass-filtered
#define CTRL_REG5_OUT_SEL_2           0x02  // Data in DataReg and FIFO are low-pass-filtered
#define CTRL_REG5_OUT_SEL_3           0x03  // Data in DataReg and FIFO are high-pass and low-pass-filtered

/*typedefs----------------------------------------------*/

/**
 * @brief Enumeration of the L3G4200D gyroscope's data rate and bandwidth settings.
 * This enumeration defines the possible combinations of data rate and bandwidth settings for the L3G4200D gyroscope sensor. 
 * Each value corresponds to a specific configuration of the sensor's output data rate
 */
typedef enum l3g4200d_dr_and_bw {
    DRBW_100HZ_12_5 = 0x00, /**< Data rate: 100 Hz, Bandwidth: 12.5 Hz */
    DRBW_100HZ_25 = 0x01, /**< Data rate: 100 Hz, Bandwidth: 25 Hz */
    DRBW_200HZ_12_5 = 0x04, /**< Data rate: 200 Hz, Bandwidth: 12.5 Hz */
    DRBW_200HZ_25 = 0x05, /**< Data rate: 200 Hz, Bandwidth: 25 Hz */
    DRBW_200HZ_50 = 0x06, /**< Data rate: 200 Hz, Bandwidth: 50 Hz */
    DRBW_200HZ_70 = 0x07, /**< Data rate: 200 Hz, Bandwidth: 70 Hz */
    DRBW_400HZ_20 = 0x08, /**< Data rate: 400 Hz, Bandwidth: 20 Hz */
    DRBW_400HZ_25 = 0x09, /**< Data rate: 400 Hz, Bandwidth: 25 Hz */
    DRBW_400HZ_50 = 0x0A, /**< Data rate: 400 Hz, Bandwidth: 50 Hz */
    DRBW_400HZ_110 = 0x0B, /**< Data rate: 400 Hz, Bandwidth: 110 Hz */
    DRBW_800HZ_30 = 0x0C, /**< Data rate: 800 Hz, Bandwidth: 30 Hz */
    DRBW_800HZ_35 = 0x0D, /**< Data rate: 800 Hz, Bandwidth: 35 Hz */
    DRBW_800HZ_50 = 0x0E, /**< Data rate: 800 Hz, Bandwidth: 50 Hz */
    DRBW_800HZ_110 = 0x0F /**< Data rate: 800 Hz, Bandwidth: 110 Hz */
} l3g4200d_dr_bw_t;

/**
 * @brief Enumeration of the L3G4200D gyroscope's power-down modes.
 * This enumeration defines the various power-down modes for the L3G4200D gyroscope sensor. 
 * Each mode corresponds to a specific configuration of the sensor's power state and axis activation.
 */
typedef enum l3g4200d_pd_mode {
    POWER_DOWN = 0x00, /**< Power-down mode: The sensor is in a low-power state and not measuring angular velocity. */
    POWER_NORMAL = SET_BIT(3) | SET_BIT(0) | SET_BIT(1) | SET_BIT(2), /**< Normal mode: The sensor is fully operational. */
    POWER_SLEEP = SET_BIT(3), /**< Sleep mode: The sensor is in a low-power state but can be quickly reactivated. */
    POWER_Z_AXIS_ONLY = SET_BIT(3) | SET_BIT(2), /**< Power-down mode with Z-axis enabled: Only the Z-axis is active and measuring angular velocity. */
    POWER_X_AXIS_ONLY = SET_BIT(3) | SET_BIT(1), /**< Power-down mode with X-axis enabled: Only the X-axis is active and measuring angular velocity. */
    POWER_Y_AXIS_ONLY = SET_BIT(3) | SET_BIT(0), /**< Power-down mode with Y-axis enabled: Only the Y-axis is active and measuring angular velocity. */
    POWER_XY_AXIS_ONLY = SET_BIT(3) | SET_BIT(1) | SET_BIT(2), /**< Power-down mode with X and Y axes enabled: Only the X and Y axes are active and measuring angular velocity. */
    POWER_YZ_AXIS_ONLY = SET_BIT(3) | SET_BIT(0) | SET_BIT(2), /**< Power-down mode with Y and Z axes enabled: Only the Y and Z axes are active and measuring angular velocity. */
    POWER_XZ_AXIS_ONLY = SET_BIT(3) | SET_BIT(1) | SET_BIT(2) /**< Power-down mode with X and Z axes enabled: Only the X and Z axes are active and measuring angular velocity. */
} l3g4200d_pd_mode_t;

/**
 * @brief High-pass filter mode enumeration for the L3G4200D gyroscope.
 * This enumeration defines the different high-pass filter modes available for the L3G4200D gyroscope sensor.
 * The high-pass filter can be configured to operate in various modes, 
 * which affect how the sensor processes the angular velocity data. 
 * The modes are determined by the settings of the HPM1 and HPM0 bits in the control register. 
 * The table below summarizes the modes based on the HPM1
 * +--------+--------+-----------------------------------------------+
 *  | HPM1   | HPM0   | High Pass Filter Mode                         |
 *  +--------+--------+-----------------------------------------------+
 *  |   0    |   0    | Normal mode (reset reading HP_RESET_FILTER)  |
 *  |   0    |   1    | Reference signal for filtering               |
 *  |   1    |   0    | Normal mode                                  |
 *  |   1    |   1    | Autoreset on interrupt event                 |
 *  +--------+--------+-----------------------------------------------+
 */
typedef enum l3g4200d_hpf_mode
{
    HPF_MODE_NORMAL_RESET            = 0b00,  // 00
    HPF_MODE_REFERENCE_SIGNAL        = 0b01,  // 01
    HPF_MODE_NORMAL                  = 0b10,  // 10
    HPF_MODE_AUTORESET_ON_INTERRUPT  = 0b11   // 11

} l3g4200d_hpf_mode_t;

/**
 * @brief High-pass filter cutoff frequency enumeration for the L3G4200D gyroscope.
 * This enumeration defines the various cutoff frequencies for the high-pass filter of the L3G4200D gyroscope sensor.
 * The cutoff frequency is determined by the settings of the HPCF3, HPCF2, HPCF1, and HPCF0 bits in the control register. 
 * The table below summarizes the cutoff frequencies based on the HPCF[3:0] bits and the selected output data rate
 * +----------+---------+---------+---------+---------+
 * | HPCF[3:0]| ODR=100 | ODR=200 | ODR=400 | ODR=800 |
 * +----------+---------+---------+---------+---------+
 * | 0000     |   8 Hz  |  15 Hz  |  30 Hz  |  56 Hz  |
 * | 0001     |   4 Hz  |   8 Hz  |  15 Hz  |  30 Hz  |
 * | 0010     |   2 Hz  |   4 Hz  |   8 Hz  |  15 Hz  |
 * | 0011     |   1 Hz  |   2 Hz  |   4 Hz  |   8 Hz  |
 * | 0100     |  0.5 Hz |   1 Hz  |   2 Hz  |   4 Hz  |
 * | 0101     |  0.2 Hz |  0.5 Hz |   1 Hz  |   2 Hz  |
 * | 0110     |  0.1 Hz |  0.2 Hz |  0.5 Hz |   1 Hz  |
 * | 0111     | 0.05 Hz |  0.1 Hz |  0.2 Hz |  0.5 Hz |
 * | 1000     | 0.02 Hz | 0.05 Hz |  0.1 Hz |  0.2 Hz |
 * | 1001     | 0.01 Hz | 0.02 Hz | 0.05 Hz |  0.1 Hz |
 * +----------+---------+---------+---------+---------+
 */
typedef enum l3g4200d_hpf_cutoff
{
    HPF_CUTOFF_0000 = 0b0000,
    HPF_CUTOFF_0001 = 0b0001,
    HPF_CUTOFF_0010 = 0b0010,
    HPF_CUTOFF_0011 = 0b0011,
    HPF_CUTOFF_0100 = 0b0100,
    HPF_CUTOFF_0101 = 0b0101,
    HPF_CUTOFF_0110 = 0b0110,
    HPF_CUTOFF_0111 = 0b0111,
    HPF_CUTOFF_1000 = 0b1000,
    HPF_CUTOFF_1001 = 0b1001

} l3g4200d_hpf_cutoff_t;



/**
 * @brief Enumeration of the L3G4200D gyroscope's register addresses.
 * This enumeration defines the register addresses for the L3G4200D gyroscope sensor.
 */
typedef enum l3g4200d_registers{
    WHO_AM_I = 0x0F,  /**< Device identification register */
    CTRL_REG1 = 0x20, /**< Control register 1: Power management and data rate settings */
    CTRL_REG2 = 0x21, /**< Control register 2: High-pass filter settings */
    CTRL_REG3 = 0x22, /**< Control register 3: Interrupt settings */
    CTRL_REG4 = 0x23, /**< Control register 4: Full-scale selection and self-test settings */
    CTRL_REG5 = 0x24, /**< Control register 5: Reboot memory content and FIFO settings */
    REFERENCE = 0x25, /**< Reference value (uint8_t) for interrupt generation */
    OUT_TEMP = 0x26, /**< Output register for temperature data (8-bit signed value representing the temperature in degrees Centigrade) */
    STATUS_REG = 0x27, /**< Status register: Indicates the status of data availability and overrun conditions */
    OUT_X_L = 0x28, /**< Output register for X-axis data (low byte) */
    OUT_X_H = 0x29, /**< Output register for X-axis data (high byte) */
    OUT_Y_L = 0x2A, /**< Output register for Y-axis data (low byte) */
    OUT_Y_H = 0x2B, /**< Output register for Y-axis data (high byte) */
    OUT_Z_L = 0x2C, /**< Output register for Z-axis data (low byte) */
    OUT_Z_H = 0x2D, /**< Output register for Z-axis data (high byte) */
    FIFO_CTRL_REG = 0x2E, /**< FIFO control register */
    FIFO_SRC_REG = 0x2F, /**< FIFO source register */
    INT1_CFG = 0x30, /**< Interrupt 1 configuration register */
    INT1_SRC = 0x31, /**< Interrupt 1 source register */
    INT1_TSH_XH = 0x32, /**< Interrupt 1 threshold for X-axis (high byte) */
    INT1_TSH_XL = 0x33, /**< Interrupt 1 threshold for X-axis (low byte) */
    INT1_TSH_YH = 0x34, /**< Interrupt 1 threshold for Y-axis (high byte) */
    INT1_TSH_YL = 0x35, /**< Interrupt 1 threshold for Y-axis (low byte) */
    INT1_TSH_ZH = 0x36, /**< Interrupt 1 threshold for Z-axis (high byte) */
    INT1_TSH_ZL = 0x37, /**< Interrupt 1 threshold for Z-axis (low byte) */
    INT1_DURATION = 0x38 /**< Interrupt 1 duration register */
} l3g4200d_registers_t;

/*macros------------------------------------------------*/

#endif // L3G4200D_H