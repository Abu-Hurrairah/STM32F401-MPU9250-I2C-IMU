/*
 * Integration example for a CubeMX-generated STM32F401 main.c.
 *
 * Keep CubeMX-generated HAL, clock, GPIO and I2C initialization in your
 * real project. Copy only the USER CODE sections you need.
 */

/* USER CODE BEGIN Includes */
#include "mpu9250.h"
#include <stdio.h>
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
static MPU9250_RawData imu_raw;
static MPU9250_Data imu_data;
/* USER CODE END PV */

/* After MX_I2C1_Init(): */

/* USER CODE BEGIN 2 */
MPU9250_Status imu_status = MPU9250_Init(&hi2c1);

if (imu_status != MPU9250_OK)
{
    printf(
        "MPU9250 init failed: %s\r\n",
        MPU9250_StatusString(imu_status)
    );
}
/* USER CODE END 2 */

/* Inside while (1): */

/* USER CODE BEGIN WHILE */
if (imu_status == MPU9250_OK)
{
    imu_status = MPU9250_ReadRaw(&hi2c1, &imu_raw);

    if (imu_status == MPU9250_OK)
    {
        MPU9250_Convert(&imu_raw, &imu_data);

        printf(
            "RAW A[%d,%d,%d] G[%d,%d,%d] | "
            "A[g]=[%.3f,%.3f,%.3f] "
            "G[dps]=[%.3f,%.3f,%.3f] T=%.2fC\r\n",
            imu_raw.accel_x,
            imu_raw.accel_y,
            imu_raw.accel_z,
            imu_raw.gyro_x,
            imu_raw.gyro_y,
            imu_raw.gyro_z,
            imu_data.accel_x_g,
            imu_data.accel_y_g,
            imu_data.accel_z_g,
            imu_data.gyro_x_dps,
            imu_data.gyro_y_dps,
            imu_data.gyro_z_dps,
            imu_data.temperature_c
        );
    }
}

HAL_Delay(100U);
/* USER CODE END WHILE */
