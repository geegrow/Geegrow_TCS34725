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
void Geegrow_TCS34725::getRawData(int16_t &red, int16_t &green, int16_t &blue, int16_t &clear) {
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
void Geegrow_TCS34725::getRGB_255(int16_t &red, int16_t &green, int16_t &blue) {
    int16_t temp_red, temp_green, temp_blue, temp_clear;
    this->getRawData(temp_red, temp_green, temp_blue, temp_clear);

    if (temp_clear == 0) {
        red = green = blue = 0;
        return;
    }

    float clearDiv = this->calibValues[0].clear / (float)temp_clear;
    uint8_t calibIdx = 0;
    for (uint8_t i = this->calibTableSize - 1; i > 0; i--) {
        if (clearDiv > this->calibCoef[i][3]) {
            calibIdx = i;
            break;
        }
    }

    temp_red   *= this->calibCoef[calibIdx][0];
    temp_green *= this->calibCoef[calibIdx][1];
    temp_blue  *= this->calibCoef[calibIdx][2];
    temp_clear *= this->calibCoef[calibIdx][3];

    red   = (float)temp_red   / (float)(this->calibValues[0].red) * 255.0;
    green = (float)temp_green / (float)(this->calibValues[0].green) * 255.0;
    blue  = (float)temp_blue  / (float)(this->calibValues[0].blue) * 255.0;

    if (red > 255) red = 255;
    if (green > 255) green = 255;
    if (blue > 255) blue = 255;

    if (red < 0) red = 0;
    if (green < 0) green = 0;
    if (blue < 0) blue = 0;
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
    uint8_t t = this->I2C_read_8(RN_ENABLE);
    this->I2C_write_8(RN_ENABLE, t & ~RN_ENABLE_AIEN);
}

/******************************************************************************/
/*!
    @brief    Clears interrupt flag in device
 */
/******************************************************************************/
void Geegrow_TCS34725::clearIRQ() {
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
void Geegrow_TCS34725::setLimitsIRQ(int16_t high, int16_t low) {
    this->I2C_write_8(RN_AIHTH, high >> 8);
    this->I2C_write_8(RN_AIHTL, high & 0xFF);
    this->I2C_write_8(RN_AILTH, low >> 8);
    this->I2C_write_8(RN_AILTL, low & 0xFF);
}

/******************************************************************************/
/*!
    @brief    Realize auto-calibration of the sensor
 */
/******************************************************************************/
void Geegrow_TCS34725::calibrate() {
    Serial.println("Calibration starts. Bring a white sample to the sensor in 5 sec");
    delay(5000);
    Serial.println("Calibrating..");
    int16_t red, green, blue, clear;
    /* Number of calibration samples */
    uint16_t temp = CALIBRATION_TIME / this->currentIntegrationTime;
    this->calibTableSize = (temp > MAX_CALIB_TABLE_SIZE) ? MAX_CALIB_TABLE_SIZE : temp;
    /* Calibration table */
    this->calibValues = new RGBC_value_t[this->calibTableSize];
    /* Delay between calibration samples */
    uint16_t delayDelta = CALIBRATION_TIME / this->calibTableSize - this->currentIntegrationTime;

    /* Record samples */
    for (uint8_t i = 0; i < this->calibTableSize; i++) {
        this->getRawData(red, green, blue, clear);
        this->calibValues[i].red = red;
        this->calibValues[i].green = green;
        this->calibValues[i].blue = blue;
        this->calibValues[i].clear = clear;
        delay(delayDelta);
    }
    /* Range recorded samples from MAX to MIN value of clear component */
    Tools::bubble_sort(this->calibValues, this->calibTableSize);
    /* Calculate scaling coefs */
    /* Row 1 without any corrections */
    for (uint8_t i = 0; i < 4; i++)
        this->calibCoef[0][i] = 1.0;
    /* Calculate other rows */
    for (uint8_t i = 1; i < this->calibTableSize; i++) {
        this->calibCoef[i][0] = (float)this->calibValues[0].red / (float)this->calibValues[i].red;
        this->calibCoef[i][1] = (float)this->calibValues[0].green / (float)this->calibValues[i].green;
        this->calibCoef[i][2] = (float)this->calibValues[0].blue / (float)this->calibValues[i].blue;
        this->calibCoef[i][3] = (float)this->calibValues[0].clear / (float)this->calibValues[i].clear;
    }
}

/******************************************************************************/
/*!
    @brief    Upload user's calibration table instead of auto-calibration
    @param    array   Pointer to array of user's RGBC values for calibration
    @param    size    Number of samples in array
 */
/******************************************************************************/
void Geegrow_TCS34725::calibrateManual(RGBC_value_t* array, uint8_t size) {
    if (!array || size < 2)
        return;
    if (size > MAX_CALIB_TABLE_SIZE)
        size = MAX_CALIB_TABLE_SIZE;
    this->calibValues = array;
    this->calibTableSize = size;
    /* Range recorded samples from MAX to MIN value of clear component */
    Tools::bubble_sort(this->calibValues, this->calibTableSize);
    /* Calculate scaling coefs */
    /* Row 1 without any corrections */
    for (uint8_t i = 0; i < 4; i++)
        this->calibCoef[0][i] = 1.0;
    /* Calculate other rows */
    for (uint8_t i = 1; i < this->calibTableSize; i++) {
        this->calibCoef[i][0] = (float)this->calibValues[0].red / (float)this->calibValues[i].red;
        this->calibCoef[i][1] = (float)this->calibValues[0].green / (float)this->calibValues[i].green;
        this->calibCoef[i][2] = (float)this->calibValues[0].blue / (float)this->calibValues[i].blue;
        this->calibCoef[i][3] = (float)this->calibValues[0].clear / (float)this->calibValues[i].clear;
    }
}

/******************************************************************************/
/*!
    @brief    Get values, recorded during calibration
    @param    size    Reference to size of array of calibration values
    @return   Pointer to array of calibration values
 */
/******************************************************************************/
RGBC_value_t* Geegrow_TCS34725::getCalibrationValues(uint8_t &size) {
    size = this->calibTableSize;
    return this->calibValues;
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
    @return   8-bit value of register
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
    @return   16-bit value of register
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

/******************************************************************************/
/*!
    @brief    Sort array from MAX to MIN
    @param    array   Pointer to array of RGBC values for sorting
    @param    size    Number of samples in array
 */
/******************************************************************************/
void Tools::bubble_sort(RGBC_value_t *array, uint8_t array_size) {
    uint8_t i = 0;
    RGBC_value_t buf;
    bool swap_cnt = false;
    if (array_size == 0)
        return;
    while (i < array_size) {
        if (i + 1 != array_size && array[i].clear < array[i + 1].clear) {
            buf = array[i];
            array[i] = array[i + 1];
            array[i + 1] = buf;
            swap_cnt = true;
        }
        i++;
        if (i == array_size && swap_cnt) {
            swap_cnt = false;
            i = 0;
        }
    }
}