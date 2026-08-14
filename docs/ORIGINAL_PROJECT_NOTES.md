# Original Project Notes

The preserved project is named:

```text
STM32F401CE
```

## What the original source actually does

`Core/Src/main.c`:

- initializes HAL;
- runs at the configured 16 MHz HSI system clock;
- initializes `I2C1`;
- calls `MPU9250_Init()`;
- creates two `int16_t[3]` arrays for accelerometer and gyroscope data;
- calls `Read_Accel_Gyro()` continuously;
- calls `printf()` for raw acceleration and gyro values;
- waits 100 ms between reads.

`Core/Src/mpu9250.c`:

- uses the HAL I²C memory read/write API;
- uses address `0x68 << 1`;
- wakes the device through register `0x6B`;
- selects accelerometer ±2 g through `0x1C`;
- selects gyroscope ±250 °/s through `0x1B`;
- reads 14 bytes beginning at `0x3B`;
- reconstructs X/Y/Z accelerometer values;
- reconstructs X/Y/Z gyroscope values.

## What the original source does not show

The archived source does not establish:

- use of the MPU9250 magnetometer;
- calibration or sensor-fusion logic;
- a specific `printf()` retarget destination;
- USB, UART, or wireless telemetry for this project.

Those capabilities are therefore not claimed in the README.
