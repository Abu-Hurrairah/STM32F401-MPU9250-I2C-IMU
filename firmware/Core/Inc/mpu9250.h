#ifndef MPU9250_H
#define MPU9250_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define MPU9250_I2C_ADDRESS_7BIT      0x68U
#define MPU9250_I2C_ADDRESS_HAL       (MPU9250_I2C_ADDRESS_7BIT << 1U)

#define MPU9250_ACCEL_LSB_PER_G       16384.0f
#define MPU9250_GYRO_LSB_PER_DPS      131.0f

typedef enum
{
    MPU9250_OK = 0,
    MPU9250_ERROR_INVALID_ARGUMENT,
    MPU9250_ERROR_I2C,
    MPU9250_ERROR_DEVICE_ID
} MPU9250_Status;

typedef struct
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;

    int16_t temperature_raw;

    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} MPU9250_RawData;

typedef struct
{
    float accel_x_g;
    float accel_y_g;
    float accel_z_g;

    float gyro_x_dps;
    float gyro_y_dps;
    float gyro_z_dps;

    float temperature_c;
} MPU9250_Data;

MPU9250_Status MPU9250_ReadWhoAmI(
    I2C_HandleTypeDef *hi2c,
    uint8_t *device_id
);

MPU9250_Status MPU9250_Init(I2C_HandleTypeDef *hi2c);

MPU9250_Status MPU9250_ReadRaw(
    I2C_HandleTypeDef *hi2c,
    MPU9250_RawData *raw
);

void MPU9250_Convert(
    const MPU9250_RawData *raw,
    MPU9250_Data *data
);

const char *MPU9250_StatusString(MPU9250_Status status);

#ifdef __cplusplus
}
#endif

#endif
