# STM32F401 + MPU9250 I²C IMU

### Raw accelerometer & gyroscope acquisition on STM32F401 using STM32 HAL

[![MCU](https://img.shields.io/badge/MCU-STM32F401-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white)](#)
[![Sensor](https://img.shields.io/badge/Sensor-MPU9250-111827?style=for-the-badge)](#)
[![Bus](https://img.shields.io/badge/Bus-I%C2%B2C1-0EA5E9?style=for-the-badge)](#)
[![Language](https://img.shields.io/badge/Language-Embedded%20C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)](#)
[![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-7C3AED?style=for-the-badge)](#)

</div>

---

# STM32F401 MPU9250 I2C IMU

This project demonstrates communication between an STM32F401 microcontroller and an MPU9250 IMU sensor using I2C.

The STM32 reads raw accelerometer and gyroscope data from the MPU9250, reconstructs the 16-bit sensor values, and makes the readings available for debugging or further embedded-system processing.

## Project Overview

The project was developed in STM32CubeIDE using STM32 HAL.

Main functions of the project:

- Initialize I2C1 on the STM32F401
- Initialize and wake the MPU9250
- Configure the accelerometer to ±2 g
- Configure the gyroscope to ±250 degrees/second
- Read accelerometer X, Y and Z values
- Read gyroscope X, Y and Z values
- Read the complete 14-byte sensor data block
- Repeat sensor acquisition every 100 ms
- Display raw values using `printf()`

## Hardware Used

- STM32F401 microcontroller
- MPU9250 IMU sensor
- Jumper wires
- USB/debug connection
- STM32CubeIDE

## I2C Connection

| Connection | Pin / Value |
|---|---|
| I2C1 SCL | PB6 |
| I2C1 SDA | PB7 |
| MPU9250 address | 0x68 |
| I2C speed | 100 kHz |

The exact power connection depends on the MPU9250 breakout board being used.

## System Flow

```text
MPU9250
   |
   | I2C
   |
STM32F401
   |
   |-- Accelerometer X, Y, Z
   |-- Gyroscope X, Y, Z
   |-- Temperature register
   |
   v
Raw Sensor Values
   |
   v
Debug / Further Processing
```

## MPU9250 Configuration

The original project configures:

```text
Accelerometer range : ±2 g
Gyroscope range     : ±250 degrees/second
I2C address         : 0x68
I2C speed           : 100 kHz
Main loop delay     : 100 ms
```

## Important Registers

| Register | Address | Purpose |
|---|---:|---|
| PWR_MGMT_1 | 0x6B | Wake the MPU9250 |
| GYRO_CONFIG | 0x1B | Configure gyroscope range |
| ACCEL_CONFIG | 0x1C | Configure accelerometer range |
| ACCEL_XOUT_H | 0x3B | Start of sensor data block |
| WHO_AM_I | 0x75 | Verify MPU9250 identity |

## Reading Sensor Data

The project reads 14 bytes starting from register `0x3B`.

```text
Byte 0-1    Accelerometer X
Byte 2-3    Accelerometer Y
Byte 4-5    Accelerometer Z
Byte 6-7    Temperature
Byte 8-9    Gyroscope X
Byte 10-11  Gyroscope Y
Byte 12-13  Gyroscope Z
```

Each high and low byte pair is combined into a signed 16-bit value.

Example:

```c
accel[0] = (int16_t)((buffer[0] << 8) | buffer[1]);
accel[1] = (int16_t)((buffer[2] << 8) | buffer[3]);
accel[2] = (int16_t)((buffer[4] << 8) | buffer[5]);
```

## Main Program Flow

The original project follows this structure:

```c
MPU9250_Init();

while (1)
{
    Read_Accel_Gyro(accel, gyro);

    printf("Accel: X=%d, Y=%d, Z=%d\n",
           accel[0], accel[1], accel[2]);

    printf("Gyro: X=%d, Y=%d, Z=%d\n",
           gyro[0], gyro[1], gyro[2]);

    HAL_Delay(100);
}
```

## Clean Driver Included

The repository also contains a cleaned reusable MPU9250 driver under `firmware/`.

It includes:

- WHO_AM_I verification
- HAL I2C error handling
- Raw-data structure
- Accelerometer conversion to g
- Gyroscope conversion to degrees/second
- Temperature conversion to Celsius

Example:

```c
MPU9250_RawData raw;
MPU9250_Data data;

if (MPU9250_Init(&hi2c1) == MPU9250_OK)
{
    if (MPU9250_ReadRaw(&hi2c1, &raw) == MPU9250_OK)
    {
        MPU9250_Convert(&raw, &data);
    }
}
```

## Raw Value Conversion

For the configured ranges:

```text
Acceleration (g) = raw / 16384
Gyroscope (dps)  = raw / 131
Temperature (C)  = raw / 333.87 + 21
```

## Repository Structure

```text
STM32F401-MPU9250-I2C-IMU/
|
|-- README.md
|-- GITHUB_SETUP.md
|-- STM32CUBEIDE_SETUP.md
|-- .gitignore
|
|-- original_source/
|   |-- STM32F401CE.ioc
|   `-- Core/
|       |-- Inc/
|       |-- Src/
|       `-- Startup/
|
|-- firmware/
|   `-- Core/
|       |-- Inc/
|       |   `-- mpu9250.h
|       `-- Src/
|           |-- mpu9250.c
|           `-- main_integration_example.c
|
|-- docs/
|   |-- ORIGINAL_PROJECT_NOTES.md
|   |-- BUILD_STATUS.md
|   `-- TESTING_CHECKLIST.md
|
`-- examples/
    `-- expected_output_format.txt
```

## Original Source

The complete original STM32CubeIDE source is preserved inside:

```text
original_source/
```

The original CubeMX configuration file is:

```text
original_source/STM32F401CE.ioc
```

Original project build status:

```text
Errors: 0
Warnings: 0
```

## STM32CubeIDE Setup

Open the `.ioc` file in STM32CubeIDE and verify:

```text
PB6 -> I2C1_SCL
PB7 -> I2C1_SDA
I2C speed -> 100 kHz
```

Detailed setup instructions are available in `STM32CUBEIDE_SETUP.md`.

## Notes

The MPU9250 also contains a magnetometer, but this project only uses the accelerometer and gyroscope.

The original source is kept separate from the cleaned driver so the original implementation remains preserved.
