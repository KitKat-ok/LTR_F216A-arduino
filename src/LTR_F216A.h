// LTR-F216A Arduino Library
#include <Wire.h>

#ifndef LTR_F216A_H
#define LTR_F216A_H

#define LTR_F216A_ADDRESS 0x53

// Register Addresses
#define REG_MAIN_CTRL        0x00
#define REG_ALS_MEAS_RES     0x04
#define REG_ALS_GAIN         0x05
#define REG_ALS_DATA_0       0x0D
#define REG_ALS_DATA_1       0x0E
#define REG_ALS_DATA_2       0x0F
#define REG_CLEAR_DATA_0     0x0A
#define REG_CLEAR_DATA_1     0x0B
#define REG_CLEAR_DATA_2     0x0C

class LTR_F216A {
public:
    LTR_F216A();

    void begin();
    void setActiveMode();
    void setStandbyMode();
    void configureMeasurement(uint8_t resolution, uint8_t rate);
    void setGain(uint8_t gain);
    void configureLowLuxDetection();
    float readBrightnessInLux();
    void readClearAndAlsData(uint32_t &clearData, uint32_t &alsData);

private:
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    uint32_t readRegister24(uint8_t reg);
    uint32_t combineRegisterData(uint8_t msb, uint8_t mid, uint8_t lsb);
};

#endif