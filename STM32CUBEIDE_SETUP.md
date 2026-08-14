# STM32CubeIDE Setup

This repository preserves the original CubeMX project under `original_source/`.

## Original target

```text
MCU family / target: STM32F401C(D-E)Ux
Package: UFQFPN48
System clock: 16 MHz HSI
I2C1: 100 kHz
PB6: I2C1_SCL
PB7: I2C1_SDA
```

## Option A — Open the preserved project configuration

1. Open STM32CubeIDE.
2. Choose **File → Open Projects from File System** if importing the preserved tree, or create a clean STM32 project for the same target.
3. Open:

```text
original_source/STM32F401CE.ioc
```

4. Check the pinout:

```text
PB6 → I2C1_SCL
PB7 → I2C1_SDA
```

5. In **Connectivity → I2C1**, confirm:

```text
Mode: I2C
Clock speed: 100000
Addressing mode: 7-bit
```

6. Generate the HAL/CMSIS code if your local copy requires it.

## Option B — Use the cleaned driver

Copy:

```text
firmware/Core/Inc/mpu9250.h
```

to:

```text
Core/Inc/mpu9250.h
```

Copy:

```text
firmware/Core/Src/mpu9250.c
```

to:

```text
Core/Src/mpu9250.c
```

Then copy the relevant user-code sections from:

```text
firmware/Core/Src/main_integration_example.c
```

into the CubeMX-generated `main.c`.

## MPU9250 wiring

```text
MPU9250 SDA → PB7
MPU9250 SCL → PB6
MPU9250 GND → GND
MPU9250 AD0 → Low for 0x68
```

Use the correct supply voltage for the exact MPU9250 breakout board.

## First test

Before reading sensor values, verify:

```text
WHO_AM_I = 0x71
```

The cleaned driver performs this check automatically.

## Output

The original application calls `printf()` every 100 ms.

To see that output, configure one of the normal STM32 `printf` retargeting methods in your own project, such as UART, SWO/ITM, or another debug console method appropriate to your setup.

The archived source does not contain enough information to claim one specific retargeting method was used.
