#include "mpu9250.h"

extern I2C_HandleTypeDef hi2c1; // Use the I2C handle defined in `i2c.c`

#define MPU9250_ADDR 0x68 << 1

uint8_t I2C_Write(uint8_t reg, uint8_t data) {
    return HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}

uint8_t I2C_Read(uint8_t reg, uint8_t *buffer, uint8_t length) {
    return HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buffer, length, HAL_MAX_DELAY);
}

void MPU9250_Init() {
    I2C_Write(0x6B, 0x00); // Wake up
    I2C_Write(0x1C, 0x00); // Accelerometer ±2g
    I2C_Write(0x1B, 0x00); // Gyroscope ±250°/s
}

void Read_Accel_Gyro(int16_t *accel, int16_t *gyro) {
    uint8_t buffer[14];
    I2C_Read(0x3B, buffer, 14);

    accel[0] = (buffer[0] << 8) | buffer[1];
    accel[1] = (buffer[2] << 8) | buffer[3];
    accel[2] = (buffer[4] << 8) | buffer[5];

    gyro[0] = (buffer[8] << 8) | buffer[9];
    gyro[1] = (buffer[10] << 8) | buffer[11];
    gyro[2] = (buffer[12] << 8) | buffer[13];
}
