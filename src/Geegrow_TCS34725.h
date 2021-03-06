/*!
 * @file Geegrow_TCS34725.h
 *
 * This is a library for the GeeGrow TCS34725 Color Sensor
 * https://www.geegrow.ru
 *
 * @section author Author
 * Written by Anton Pomazanov
 *
 * @section license License
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef GEEGROW_TCS34725_H
#define GEEGROW_TCS34725_H

#include <Arduino.h>
#include <Wire.h>
#include "defines.h"

/******************************************************************************/
/*!
    @brief    I2C address of TCS34725 device
 */
/******************************************************************************/
#define TCS34725_I2C_ADDRESS   0x29

#define CALIBRATION_TIME       5000
#define MAX_CALIB_TABLE_SIZE   10

struct RGBC_value_t {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t clear;
};

/******************************************************************************/
/*!
    @brief    Class that stores state and functions for interacting with TCS34725
 */
/******************************************************************************/
class Geegrow_TCS34725 {
    public:
        Geegrow_TCS34725(
            uint8_t i2c_addr = TCS34725_I2C_ADDRESS,
            uint8_t time = RN_ATIME_INTEG_TIME_154,
            uint8_t gain = RN_CONTROL_GAIN_1X
        );
        void enable();
        void disable();
        void getRawData(int16_t &red, int16_t &green, int16_t &blue, int16_t &clear);
        void getRGB_255(int16_t &red, int16_t &green, int16_t &blue);
        void enableIRQ();
        void disableIRQ();
        void clearIRQ();
        void setLimitsIRQ(int16_t high, int16_t low);
        void calibrate();
        void calibrateManual(RGBC_value_t* array, uint8_t size);
        RGBC_value_t* getCalibrationValues(uint8_t &size);

    private:
        void setIntegrationTime(uint8_t integrationTime);
        void setGain(uint8_t gain);
        void I2C_write_8(uint8_t reg, uint8_t value);
        uint8_t I2C_read_8(uint8_t reg);
        uint16_t I2C_read_16(uint8_t reg);

        uint16_t currentIntegrationTime = 0;
        uint8_t currentGain = 0;
        uint8_t i2c_addr = 0;

        int8_t calibTableSize = 0;
        float calibCoef[MAX_CALIB_TABLE_SIZE][4];
        RGBC_value_t* calibValues;
        uint8_t calibMaxValueIndex = 0;
};

namespace Tools {
    void bubble_sort(RGBC_value_t *array, uint8_t array_size);
};

#endif /* GEEGROW_TCS34725_H */