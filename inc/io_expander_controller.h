#pragma once

#include "error.h"

/**
 * @file This is the interface file of the IO expander controller.\
 *
 * This module targets the MAX7321 chip connected over I2C.
 * 
 */

/**
 * @brief This is the main hander strct which is the actual object for the max7321 ic.
 *
 * @note The functions registered in the  **input_state_change_callback** would be in
 * the interrupt scope so avoid any large processing here.
 */
typedef struct IoExpanderHander
{
    uint8_t address ;
    void (*input_state_change_callback) (void) [8];
}IoExpanderHander_t;


/**
 * @brief This funtion should be called by the application developer in the ISR
 * of the GPIO which is connected to INT line of the max7321 chip.
 * 
 */
void io_expander_int_hander(void);





/**
 * @brief Initializes the IO expander MAX7321 w.r.t the parameters defined in the IoExpanderHander_t.
 * 
 */
INIT->

/*
Takes the port through enum
takes the level as value
returns the error state
Use the printf logging if in case of any possible error.
Use the printf logging in case of success.
*/

Write data( port, level)


/*
Takes the port through enum
takes the level as reference
returns the error state
Use the printf logging if in case of any possible error.
Use the printf logging in case of success.
*/
Read data(port, level)


