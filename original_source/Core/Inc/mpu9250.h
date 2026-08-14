#ifndef MPU9250_H
#define MPU9250_H

#include "stm32f4xx_hal.h" // Update to your STM32 series header file

void MPU9250_Init();
void Read_Accel_Gyro(int16_t *accel, int16_t *gyro);

#endif // MPU9250_H
