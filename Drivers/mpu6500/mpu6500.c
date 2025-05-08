// MPU6500.c

#include "MPU6500.h"
#include "stm32l0xx_hal.h" // Or your LL driver header

// Define the I2C handle (assuming you have it initialized elsewhere)
extern I2C_HandleTypeDef hi2c1;

// Define the MPU6500 I2C address (you might want to make this configurable)
static uint8_t mpu6500_address = MPU6500_DEFAULT_ADDRESS << 1; // Shift left for 7-bit address

// Private helper function to read a single byte from an MPU6500 register
static uint8_t mpu6500_read_byte(uint8_t reg) {
    uint8_t data;
    HAL_I2C_Master_Transmit(&hi2c1, mpu6500_address, &reg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, mpu6500_address | 0x01, &data, 1, HAL_MAX_DELAY); // Add read bit
    return data;
}

// Private helper function to write a single byte to an MPU6500 register
static void mpu6500_write_byte(uint8_t reg, uint8_t value) {
    uint8_t data[2];
    data[0] = reg;
    data[1] = value;
    HAL_I2C_Master_Transmit(&hi2c1, mpu6500_address, data, 2, HAL_MAX_DELAY);
}

// Implementation of mpu6500Init()
void mpu6500Init(void) {
    // Example: Wake up the MPU6500 (clear sleep bit)
    uint8_t power_mgmt_1 = mpu6500_read_byte(MPU6500_RA_PWR_MGMT_1);
    power_mgmt_1 &= ~(1 << MPU6500_PWR1_SLEEP_BIT);
    mpu6500_write_byte(MPU6500_RA_PWR_MGMT_1, power_mgmt_1);

    // Add other initialization steps here, like setting sample rate, DLPF, etc.
}

// Implementation of mpu6500TestConnection()
bool mpu6500TestConnection(void) {
    // Check the WHO_AM_I register
    uint8_t who_am_i = mpu6500_read_byte(MPU6500_RA_WHO_AM_I);
    // The expected value depends on the MPU6500 variant
    return (who_am_i == 0x68); // Example value - consult the datasheet
}

// Implementation of mpu6500GetAcceleration()
void mpu6500GetAcceleration(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t raw_data[6];
    uint8_t accel_x_h = MPU6500_RA_ACCEL_XOUT_H;

    HAL_I2C_Master_Transmit(&hi2c1, mpu6500_address, &accel_x_h, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, mpu6500_address | 0x01, raw_data, 6, HAL_MAX_DELAY);

    *x = (int16_t)((raw_data[0] << 8) | raw_data[1]);
    *y = (int16_t)((raw_data[2] << 8) | raw_data[3]);
    *z = (int16_t)((raw_data[4] << 8) | raw_data[5]);
}

// ... Implementations for other functions declared in MPU6500.h ...