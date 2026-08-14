# Testing Checklist

## Before power-on

- [ ] Confirm MPU9250 breakout-board supply voltage
- [ ] Connect common ground
- [ ] Connect PB6 to SCL
- [ ] Connect PB7 to SDA
- [ ] Keep AD0 low if using address 0x68
- [ ] Confirm I2C1 is configured for 100 kHz

## Firmware

- [ ] Project builds without errors
- [ ] `HAL_I2C_Init()` succeeds
- [ ] `WHO_AM_I` returns 0x71
- [ ] `MPU9250_Init()` returns `MPU9250_OK`
- [ ] 14-byte burst read returns `HAL_OK`

## Sensor behaviour

- [ ] Acceleration values change when tilting the sensor
- [ ] Gyroscope values change when rotating the sensor
- [ ] Stationary readings do not repeatedly hit ±32768
- [ ] Converted acceleration values are plausible
- [ ] Converted gyro values are plausible
- [ ] Temperature reading is plausible

## Documentation

- [ ] README diagrams render on GitHub
- [ ] Original source remains under `original_source/`
- [ ] Clean driver remains under `firmware/`
