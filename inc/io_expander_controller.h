#pragma once

#include "stdint.h"
#include "stdio.h"

#include "error.h"

#define NUM_OF_PORTS 8
/**
 * @file This is the interface file of the IO expander controller.\
 *
 * This module targets the MAX7321 chip connected over I2C.
 * 
 */


typedef enum IoExpanderGpioState
{
    io_expander_gpio_output=0,
    io_expander_gpio_input,
    io_expander_gpio_state_max
}IoExpanderGpioState_t;

typedef enum IoExpanderGpioLevel
{
    io_expander_gpio_low=0,
    io_expander_gpio_high,
    io_expander_gpio_level_max
}IoExpanderGpioLevel_t;


/**
 * @brief This is the main hander strct which is the actual object for the max7321 ic.
 *
 * The address member is the 7 bit address, MSB should be free
 * @note The functions registered in the  **input_state_change_callback** would be in
 * the interrupt scope so avoid any large processing here.
 * 
 * The address parameter in the i2c apis consider bit 1~7 for address and bit 0 for the R/W.
 * 
 * Do not  use __current_direction_state as it is intended for private use.
 */
typedef struct IoExpanderHandler
{
    uint8_t address ;
    void (*input_state_change_callback[NUM_OF_PORTS]) (void);
    ErrorState_t (*i2c_read) (uint8_t *data, uint8_t lenght, uint8_t address);
    ErrorState_t (*i2c_write) (uint8_t data, uint8_t lenght, uint8_t address);
    IoExpanderGpioState_t gpio_port_dir[NUM_OF_PORTS];
    uint8_t __current_direction_state;
    uint8_t __last_write;
}IoExpanderHandler_t;


/**
 * @brief Initializes the IO expander MAX7321 w.r.t the parameters defined in the IoExpanderHandler_t.
 * 
 * Handler should be provided with the valid address, valid i2c function and IO state of the ports.
 * It checks if the device with the respective address is present and configure the IO direction.
 * 
 * Output pins are at level 0 after init.
 * 
 * @return ErrorState_t value based on the nature of error, other wise STATE_SUCCESS
 */
ErrorState_t io_expander_controller_init( IoExpanderHandler_t *handler);


/**
 * @brief This funtion should be called by the application developer in the ISR
 * of the GPIO which is connected to INT line of the max7321 chip.
 * 
 * If this handler thinks that it should call the input_state_change_callback[1] and 
 * that is null, it will simple ignore that call.
 * 
 */
void io_expander_int_handler(IoExpanderHandler_t *handler);



/**
 * @brief sets the level pf the pin (port) to HIGH or LOW as specified in level.
 * 
 * 
 * @param handler Handler to the IC, should be initially passed to init function. and should not be modified by the user after initialization.
 * @param port pin number , valid range = 0~7
 * @param level High or low, provided by the enum IoExpanderGpioLevel_t
 * @return ErrorState_t returns success if the write was successful without any problems.
 */
ErrorState_t io_expander_gpio_write(IoExpanderHandler_t *handler, uint8_t port, IoExpanderGpioLevel_t level);



/**
 * @brief Reads the state of the pin weather its input pin or output pin
 * 
 * @param handler address of the handler variable
 * @param port pin number to read 0~7
 * @param level address of the variable of type IoExpanderGpioLevel_t
 * @return ErrorState_t 
 */
ErrorState_t io_expander_gpio_read(IoExpanderHandler_t *handler, uint8_t port, IoExpanderGpioLevel_t *level);


