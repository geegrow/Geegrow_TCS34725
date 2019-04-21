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

#include "Geegrow_TCS34725.h"

/******************************************************************************/
/*!
    @brief    Constructor
    @param    addr    I2C address of device
    @param    time    Intergration time for RGBC
    @param    gain    Gain value
 */
/******************************************************************************/
Geegrow_TCS34725::Geegrow_TCS34725(uint8_t addr, uint8_t time, uint8_t gain) {
    this->i2c_addr = addr;
    Wire.begin();
    this->setIntegrationTime(time);
    this->setGain(gain);
    this->enable();
}

/******************************************************************************/
/*!
    @brief    Enables power on device and switches on ADCs
 */
/******************************************************************************/
void Geegrow_TCS34725::enable() {
    this->I2C_write_8(RN_ENABLE, RN_ENABLE_PON);
    delay(3);
    this->I2C_write_8(RN_ENABLE, RN_ENABLE_PON | RN_ENABLE_AEN);
}

/******************************************************************************/
/*!
    @brief    Disables power on device and switches off ADCs
 */
/******************************************************************************/
void Geegrow_TCS34725::disable() {
    uint8_t t = this->I2C_read_8(RN_ENABLE);
    this->I2C_write_8(RN_ENABLE, t & (~RN_ENABLE_PON));
}

/******************************************************************************/
/*!
    @brief    Reads actual data from sensor and writes it to references of vars
    @param    red     Reference to variable for red color
    @param    green   Reference to variable for green color
    @param    blue    Reference to variable for blue color
    @param    clear   Reference to variable for sum of colors
    @note     There is a delay in this function for getting a valid RGBC value
 */
/******************************************************************************/
void Geegrow_TCS34725::getRawData(uint16_t &red, uint16_t &green, uint16_t &blue, uint16_t &clear) {
    delay(this->currentIntegrationTime);
    red   = this->I2C_read_16(RN_RDATAL);
    green = this->I2C_read_16(RN_GDATAL);
    blue  = this->I2C_read_16(RN_BDATAL);
    clear = this->I2C_read_16(RN_CDATAL);
}

/******************************************************************************/
/*!
    @brief    Reads actual data from sensor in 255 format
    @param    red     Reference to variable for red color
    @param    green   Reference to variable for green color
    @param    blue    Reference to variable for blue color
 */
/******************************************************************************/
void Geegrow_TCS34725::getRGB_255(uint16_t &red, uint16_t &green, uint16_t &blue) {
    uint16_t temp_red, temp_green, temp_blue, temp_clear;
    this->getRawData(temp_red, temp_green, temp_blue, temp_clear);

    if (temp_clear == 0) {
        red = green = blue = 0;
        return;
    }

    red = (uint16_t)((float)temp_red / (float)temp_clear * 255.0);
    green = (uint16_t)((float)temp_green / (float)temp_clear * 255.0);
    blue = (uint16_t)((float)temp_blue / (float)temp_clear * 255.0);
}

/******************************************************************************/
/*!
    @brief    Sets value of integration time
    @param    time    Value of integration time
 */
/******************************************************************************/
void Geegrow_TCS34725::setIntegrationTime(uint8_t time) {
    this->I2C_write_8(RN_ATIME, time);
    switch (time) {
        case RN_ATIME_INTEG_TIME_2_4:
            this->currentIntegrationTime = 3;
            break;
        case RN_ATIME_INTEG_TIME_24:
            this->currentIntegrationTime = 24;
            break;
        case RN_ATIME_INTEG_TIME_101:
            this->currentIntegrationTime = 101;
            break;
        case RN_ATIME_INTEG_TIME_154:
            this->currentIntegrationTime = 154;
            break;
        case RN_ATIME_INTEG_TIME_700:
            this->currentIntegrationTime = 700;
            break;
    }
}

/******************************************************************************/
/*!
    @brief    Sets value of gain
    @param    time    Value of gain
 */
/******************************************************************************/
void Geegrow_TCS34725::setGain(uint8_t gain) {
    this->I2C_write_8(RN_CONTROL, gain);
    this->currentGain = gain;
}

/******************************************************************************/
/*!
    @brief    Enables interrupts on RGBC limit values
 */
/******************************************************************************/
void Geegrow_TCS34725::enableIRQ() {
    uint8_t t = this->I2C_read_8(RN_ENABLE);
    this->I2C_write_8(RN_ENABLE, t | RN_ENABLE_AIEN);
}

/******************************************************************************/
/*!
    @brief    Disables interrupts on RGBC limit values
 */
/******************************************************************************/
void Geegrow_TCS34725::disableIRQ() {
    Wire.beginTransmission(this->i2c_addr);
    Wire.write(RN_COMMAND_CMD | RN_COMMAND_TYPE_SF | RN_COMMAND_ADDRSF_CLR_IRQ);
    Wire.endTransmission();
}

/******************************************************************************/
/*!
    @brief    Sets values of IRQ limits
    @param    high    Higher limit
    @param    low     Lower limit
 */
/******************************************************************************/
void Geegrow_TCS34725::setLimitsIRQ(uint16_t high, uint16_t low) {
    this->I2C_write_8(RN_AIHTH, high >> 8);
    this->I2C_write_8(RN_AIHTL, high & 0xFF);
    this->I2C_write_8(RN_AILTH, low >> 8);
    this->I2C_write_8(RN_AILTL, low & 0xFF);
}

/******************************************************************************/
/*!
    @brief    Write 8 bit value to device register
    @param    reg     Register to edit
    @param    value   New value
 */
/******************************************************************************/
void Geegrow_TCS34725::I2C_write_8(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(this->i2c_addr);
    Wire.write(RN_COMMAND_CMD | reg);
    Wire.write(value & 0xFF);
    Wire.endTransmission();
}

/******************************************************************************/
/*!
    @brief    Read 8 bit value from device register
    @param    reg     Register to be read
 */
/******************************************************************************/
uint8_t Geegrow_TCS34725::I2C_read_8(uint8_t reg) {
    Wire.beginTransmission(this->i2c_addr);
    Wire.write(RN_COMMAND_CMD | reg);
    Wire.endTransmission();
    Wire.requestFrom(this->i2c_addr, 1);
    return Wire.read();
}

/******************************************************************************/
/*!
    @brief    Read 16 bit value from device register
    @param    reg     Register to be read
 */
/******************************************************************************/
uint16_t Geegrow_TCS34725::I2C_read_16(uint8_t reg) {
    uint16_t x, t;
    Wire.beginTransmission(this->i2c_addr);
    Wire.write(RN_COMMAND_CMD | reg);
    Wire.endTransmission();
    Wire.requestFrom(this->i2c_addr, 2);
    t = Wire.read();
    x = Wire.read();
    x <<= 8;
    x |= t;
    return x;
}
