// LTR-F216A.cpp
#include "LTR_F216A.h"

LTR_F216A::LTR_F216A() {}

void LTR_F216A::begin() {
    Wire.begin();
}

void LTR_F216A::setActiveMode() {
    writeRegister(REG_MAIN_CTRL, 0x02);
}

void LTR_F216A::setStandbyMode() {
    writeRegister(REG_MAIN_CTRL, 0x00);
}

void LTR_F216A::configureMeasurement(uint8_t resolution, uint8_t rate) {
    uint8_t value = ((resolution & 0x07) << 4) | (rate & 0x07);
    writeRegister(REG_ALS_MEAS_RES, value);
}

void LTR_F216A::setGain(uint8_t gain) {
    writeRegister(REG_ALS_GAIN, gain & 0x07);
}

void LTR_F216A::configureLowLuxDetection() {
    setGain(0x04); // Gain x18
    configureMeasurement(0x00, 0x02); // 20-bit resolution, 400ms integration time
}

float LTR_F216A::readBrightnessInLux() {
    uint32_t clearData, alsData;
    readClearAndAlsData(clearData, alsData);
    float gain = 18.0f; // Gain x18
    float integrationTime = 400.0f; // Integration time in ms
    float lux = (0.45f * alsData) / (gain * (integrationTime / 100.0f));
    return lux;
}

void LTR_F216A::readClearAndAlsData(uint32_t &clearData, uint32_t &alsData) {
    Wire.beginTransmission((uint8_t)LTR_F216A_ADDRESS);
    Wire.write(REG_CLEAR_DATA_0);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)LTR_F216A_ADDRESS, (uint8_t)6);

    uint8_t data[6] = {0};
    for (int i = 0; i < 6 && Wire.available(); i++) {
        data[i] = Wire.read();
    }

    clearData = combineRegisterData(data[2], data[1], data[0]);
    alsData = combineRegisterData(data[5], data[4], data[3]);
}

uint32_t LTR_F216A::combineRegisterData(uint8_t msb, uint8_t mid, uint8_t lsb) {
    return ((uint32_t)msb << 16) | ((uint32_t)mid << 8) | lsb;
}

void LTR_F216A::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(LTR_F216A_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t LTR_F216A::readRegister(uint8_t reg) {
    Wire.beginTransmission((uint8_t)LTR_F216A_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)LTR_F216A_ADDRESS, (uint8_t)1);
    return Wire.read();
}

uint32_t LTR_F216A::readRegister24(uint8_t reg) {
    Wire.beginTransmission((uint8_t)LTR_F216A_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)LTR_F216A_ADDRESS, (uint8_t)3);
    uint32_t value = 0;
    if (Wire.available() >= 3) {
        value |= Wire.read();
        value |= ((uint32_t)Wire.read() << 8);
        value |= ((uint32_t)Wire.read() << 16);
    }
    return value;
}
