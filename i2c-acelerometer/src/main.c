#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "l3g4200d_driver.h"
#include "log.h"

#define I2C_DEVICE "/dev/i2c-1"
#define ACCELEROMETER_ADDR 0x68
#define SAMPLE_PERIOD_SECONDS 1
#define L3G4200D_250DPS_SENSITIVITY 0.00875f

#define TAG "i2c-acelerometer: "

static float raw_to_dps(uint16_t raw_value)
{
    return (int16_t)raw_value * L3G4200D_250DPS_SENSITIVITY;
}

int main (void) {
    l3g4200d_init_config_t config = L3G4200D_SET_DEFAULT_CONFIG;
    config.i2c_device = I2C_DEVICE;
    config.device_addr = ACCELEROMETER_ADDR;

    if (l3g4200d_init(&config) != L3G4200D_SUCCESS) {
        ERROR_LOG(TAG, "No se pudo inicializar el L3G4200D");
        return 1;
    }

    INFO_LOG(TAG, "Test L3G4200D iniciado");

    while (1) {
        l3g4200d_angular_velocity_data_t angular_velocity;
        l3g4200d_temperature_data_t temperature;

        if (l3g4200d_read_angular_velocity(&angular_velocity) != L3G4200D_SUCCESS) {
            ERROR_LOG(TAG, "No se pudo leer la velocidad angular");
            break;
        }

        if (l3g4200d_read_temperature(&temperature) != L3G4200D_SUCCESS) {
            ERROR_LOG(TAG, "No se pudo leer la temperatura");
            break;
        }

        printf("\r\033[KVelocidad angular: X=%7.2f dps, Y=%7.2f dps, Z=%7.2f dps | Temperatura=%d C",
               raw_to_dps(angular_velocity.x),
               raw_to_dps(angular_velocity.y),
               raw_to_dps(angular_velocity.z),
               temperature.temp_celsius);
        fflush(stdout);

        sleep(SAMPLE_PERIOD_SECONDS);
    }

    l3g4200d_deinit();

    return 1;
}
