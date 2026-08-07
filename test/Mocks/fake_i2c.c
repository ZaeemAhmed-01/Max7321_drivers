#include "fake_i2c.h"

#include <string.h>
#include <stdio.h>

/*---------------------------------------------------------------------------
 * Read spy
 *--------------------------------------------------------------------------*/

static uint8_t readLastAddress;
static uint8_t readLastLength;
static int readCallCount;

/*---------------------------------------------------------------------------
 * Read stub
 *--------------------------------------------------------------------------*/

static ErrorState_t readReturnValue;
static uint8_t readData[MOCK_I2C_MAX_DATA_LENGTH];
static uint8_t readDataLength;

/*---------------------------------------------------------------------------
 * Write spy
 *--------------------------------------------------------------------------*/

static uint8_t writeLastAddress;
static uint8_t writeLastLength;
static uint8_t writeLastData;
static int writeCallCount;

/*---------------------------------------------------------------------------
 * Write stub
 *--------------------------------------------------------------------------*/

static ErrorState_t writeReturnValue;

/*--------------------------------------------------------------------------*/

void mockI2cReset(void)
{
    readLastAddress = 0;
    readLastLength = 0;
    readCallCount = 0;

    readReturnValue = STATE_SUCCESS;
    readDataLength = 0;
    memset(readData, 0, sizeof(readData));

    writeLastAddress = 0;
    writeLastLength = 0;
    writeLastData = 0;
    writeCallCount = 0;

    writeReturnValue = STATE_SUCCESS;
}

/*--------------------------------------------------------------------------*/

void mockI2cReadSetReturnValue(ErrorState_t retVal)
{
    readReturnValue = retVal;
}

void mockI2cReadSetData(const uint8_t *data, uint8_t length)
{
    readDataLength = length;

    if (length > MOCK_I2C_MAX_DATA_LENGTH)
    {
        length = MOCK_I2C_MAX_DATA_LENGTH;
    }

    memcpy(readData, data, length);
}

void mockI2cWriteSetReturnValue(ErrorState_t retVal)
{
    writeReturnValue = retVal;
}

/*--------------------------------------------------------------------------*/

uint8_t mockI2cReadGetLastAddress(void)
{
    return readLastAddress;
}

uint8_t mockI2cReadGetLastLength(void)
{
    return readLastLength;
}

int mockI2cReadGetCallCount(void)
{
    return readCallCount;
}

/*--------------------------------------------------------------------------*/

uint8_t mockI2cWriteGetLastAddress(void)
{
    return writeLastAddress;
}

uint8_t mockI2cWriteGetLastLength(void)
{
    return writeLastLength;
}

uint8_t mockI2cWriteGetLastData(void)
{
    return writeLastData;
}

int mockI2cWriteGetCallCount(void)
{
    //printf("\n\n returning call count : %d\n", writeCallCount);
    return writeCallCount;
}

/*--------------------------------------------------------------------------*/

ErrorState_t fakeI2cRead(uint8_t *data,
                         uint8_t length,
                         uint8_t address)
{
    readCallCount++;

    readLastAddress = address;
    readLastLength = length;

    if ((data != NULL) && (readDataLength > 0))
    {
        uint8_t copyLength = (length < readDataLength) ? length : readDataLength;
        memcpy(data, readData, copyLength);
    }

    return readReturnValue;
}

/*--------------------------------------------------------------------------*/

ErrorState_t fakeI2cWrite(uint8_t data,
                          uint8_t length,
                          uint8_t address)
{
    writeCallCount++;

    writeLastAddress = address;
    writeLastLength = length;
    writeLastData = data;

    return writeReturnValue;
}