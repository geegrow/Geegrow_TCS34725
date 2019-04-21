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

/******************************************************************************/
/*!
    @brief    Class that stores state and functions for interacting with TCS34725
 */
/******************************************************************************/
class Geegrow_TCS34725 {
    public:
        Geegrow_TCS34725(
            uint8_t i2c_addr = TCS34725_I2C_ADDRESS,
            uint8_t time = RN_ATIME_INTEG_TIME_700,
            uint8_t gain = RN_CONTROL_GAIN_1X
        );
        void enable();
        void disable();
        void getRawData(uint16_t &red, uint16_t &green, uint16_t &blue, uint16_t &clear);
        void getRGB_255(uint16_t &red, uint16_t &green, uint16_t &blue);
        void setIntegrationTime(uint8_t integrationTime);
        void setGain(uint8_t gain);
        void enableIRQ();
        void disableIRQ();
        void setLimitsIRQ(uint16_t high, uint16_t low);

    private:
        void I2C_write_8(uint8_t reg, uint8_t value);
        uint8_t I2C_read_8(uint8_t reg);
        uint16_t I2C_read_16(uint8_t reg);

        uint16_t currentIntegrationTime = 0;
        uint8_t currentGain = 0;
        uint8_t i2c_addr = 0;
};

#endif /* GEEGROW_TCS34725_H */