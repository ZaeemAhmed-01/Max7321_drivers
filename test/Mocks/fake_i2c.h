#pragma once

#include <stdint.h>
#include "error.h"

#define MOCK_I2C_MAX_DATA_LENGTH    (16U)

/* Reset */
void mockI2cReset(void);

/* Read stub configuration */
void mockI2cReadSetReturnValue(ErrorState_t retVal);
void mockI2cReadSetData(const uint8_t *data, uint8_t length);

/* Write stub configuration */
void mockI2cWriteSetReturnValue(ErrorState_t retVal);

/* Read spies */
uint8_t mockI2cReadGetLastAddress(void);
uint8_t mockI2cReadGetLastLength(void);
int mockI2cReadGetCallCount(void);

/* Write spies */
uint8_t mockI2cWriteGetLastAddress(void);
uint8_t mockI2cWriteGetLastLength(void);
uint8_t mockI2cWriteGetLastData(void);
int mockI2cWriteGetCallCount(void);

/* Fake I2C APIs */
ErrorState_t fakeI2cRead(uint8_t *data,
                         uint8_t length,
                         uint8_t address);

ErrorState_t fakeI2cWrite(uint8_t data,
                          uint8_t length,
                          uint8_t address);