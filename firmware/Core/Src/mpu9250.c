#include "mpu9250.h"

#define MPU9250_REG_SMPLRT_DIV      0x19U
#define MPU9250_REG_CONFIG          0x1AU
#define MPU9250_REG_GYRO_CONFIG     0x1BU
#define MPU9250_REG_ACCEL_CONFIG    0x1CU
#define MPU9250_REG_ACCEL_XOUT_H    0x3BU
#define MPU9250_REG_PWR_MGMT_1      0x6BU
#define MPU9250_REG_WHO_AM_I        0x75U

#define MPU9250_WHO_AM_I_VALUE      0x71U
#define MPU9250_TIMEOUT_MS          100U

static int16_t MPU9250_CombineSigned(uint8_t high_byte, uint8_t low_byte)
{
    return (int16_t)(
        ((uint16_t)high_byte << 8U) |
        (uint16_t)low_byte
    );
}

static MPU9250_Status MPU9250_WriteRegister(
    I2C_HandleTypeDef *hi2c,
    uint8_t reg,
    uint8_t value
)
{
    if (hi2c == NULL)
    {
        return MPU9250_ERROR_INVALID_ARGUMENT;
    }

    if (HAL_I2C_Mem_Write(
            hi2c,
            MPU9250_I2C_ADDRESS_HAL,
            reg,
            I2C_MEMADD_SIZE_8BIT,
            &value,
            1U,
            MPU9250_TIMEOUT_MS
        ) != HAL_OK)
    {
        return MPU9250_ERROR_I2C;
    }

    return MPU9250_OK;
}

MPU9250_Status MPU9250_ReadWhoAmI(
    I2C_HandleTypeDef *hi2c,
    uint8_t *device_id
)
{
    if ((hi2c == NULL) || (device_id == NULL))
    {
        return MPU9250_ERROR_INVALID_ARGUMENT;
    }

    if (HAL_I2C_Mem_Read(
            hi2c,
            MPU9250_I2C_ADDRESS_HAL,
            MPU9250_REG_WHO_AM_I,
            I2C_MEMADD_SIZE_8BIT,
            device_id,
            1U,
            MPU9250_TIMEOUT_MS
        ) != HAL_OK)
    {
        return MPU9250_ERROR_I2C;
    }

    return MPU9250_OK;
}

MPU9250_Status MPU9250_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t device_id = 0U;
    MPU9250_Status status;

    status = MPU9250_ReadWhoAmI(hi2c, &device_id);
    if (status != MPU9250_OK)
    {
        return status;
    }

    if (device_id != MPU9250_WHO_AM_I_VALUE)
    {
        return MPU9250_ERROR_DEVICE_ID;
    }

    /* Wake the MPU9250. */
    status = MPU9250_WriteRegister(
        hi2c,
        MPU9250_REG_PWR_MGMT_1,
        0x00U
    );
    if (status != MPU9250_OK)
    {
        return status;
    }

    HAL_Delay(100U);

    /*
     * Keep a conservative configuration close to the original project:
     * accelerometer = +/-2 g
     * gyroscope     = +/-250 deg/s
     */
    status = MPU9250_WriteRegister(
        hi2c,
        MPU9250_REG_ACCEL_CONFIG,
        0x00U
    );
    if (status != MPU9250_OK)
    {
        return status;
    }

    status = MPU9250_WriteRegister(
        hi2c,
        MPU9250_REG_GYRO_CONFIG,
        0x00U
    );
    if (status != MPU9250_OK)
    {
        return status;
    }

    return MPU9250_OK;
}

MPU9250_Status MPU9250_ReadRaw(
    I2C_HandleTypeDef *hi2c,
    MPU9250_RawData *raw
)
{
    uint8_t buffer[14];

    if ((hi2c == NULL) || (raw == NULL))
    {
        return MPU9250_ERROR_INVALID_ARGUMENT;
    }

    if (HAL_I2C_Mem_Read(
            hi2c,
            MPU9250_I2C_ADDRESS_HAL,
            MPU9250_REG_ACCEL_XOUT_H,
            I2C_MEMADD_SIZE_8BIT,
            buffer,
            sizeof(buffer),
            MPU9250_TIMEOUT_MS
        ) != HAL_OK)
    {
        return MPU9250_ERROR_I2C;
    }

    raw->accel_x = MPU9250_CombineSigned(buffer[0], buffer[1]);
    raw->accel_y = MPU9250_CombineSigned(buffer[2], buffer[3]);
    raw->accel_z = MPU9250_CombineSigned(buffer[4], buffer[5]);

    raw->temperature_raw =
        MPU9250_CombineSigned(buffer[6], buffer[7]);

    raw->gyro_x = MPU9250_CombineSigned(buffer[8], buffer[9]);
    raw->gyro_y = MPU9250_CombineSigned(buffer[10], buffer[11]);
    raw->gyro_z = MPU9250_CombineSigned(buffer[12], buffer[13]);

    return MPU9250_OK;
}

void MPU9250_Convert(
    const MPU9250_RawData *raw,
    MPU9250_Data *data
)
{
    if ((raw == NULL) || (data == NULL))
    {
        return;
    }

    data->accel_x_g =
        (float)raw->accel_x / MPU9250_ACCEL_LSB_PER_G;
    data->accel_y_g =
        (float)raw->accel_y / MPU9250_ACCEL_LSB_PER_G;
    data->accel_z_g =
        (float)raw->accel_z / MPU9250_ACCEL_LSB_PER_G;

    data->gyro_x_dps =
        (float)raw->gyro_x / MPU9250_GYRO_LSB_PER_DPS;
    data->gyro_y_dps =
        (float)raw->gyro_y / MPU9250_GYRO_LSB_PER_DPS;
    data->gyro_z_dps =
        (float)raw->gyro_z / MPU9250_GYRO_LSB_PER_DPS;

    data->temperature_c =
        ((float)raw->temperature_raw / 333.87f) + 21.0f;
}

const char *MPU9250_StatusString(MPU9250_Status status)
{
    switch (status)
    {
        case MPU9250_OK:
            return "OK";

        case MPU9250_ERROR_INVALID_ARGUMENT:
            return "INVALID_ARGUMENT";

        case MPU9250_ERROR_I2C:
            return "I2C_ERROR";

        case MPU9250_ERROR_DEVICE_ID:
            return "DEVICE_ID_MISMATCH";

        default:
            return "UNKNOWN";
    }
}
