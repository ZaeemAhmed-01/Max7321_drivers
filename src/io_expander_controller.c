#include <stddef.h>

#include "application_defs.h"
#include "printf_logging.h"
#include "io_expander_controller.h"

#ifdef USE_PRINTF_LOGGING
    #include <stdio.h>
#endif





ErrorState_t io_expander_controller_init( IoExpanderHandler_t *handler)
{
    uint8_t byte_read[2];
    uint8_t byte_wrote;

    if(handler==NULL)
        return STATE_NULL_POINTER;

    if(handler->i2c_write == NULL || handler->i2c_read == NULL)
        return STATE_NULL_POINTER;

    if(handler->address > 0x6f || handler->address < 0x60)
        return STATE_INVALID_DATA;

    // see if the device exits
    ErrorState_t ret = handler->i2c_read(byte_read, 1, ((handler->address)<<1) | 1);
    if(ret != STATE_SUCCESS)    return STATE_FAIL;

    #ifdef USE_PRINTF_LOGGING
        printf("\n\n Error conditions passed ! \n");
    #endif
    
    byte_wrote = 0;
    handler->__current_direction_state = 0xFF;
    for(uint8_t i=0; i<NUM_OF_PORTS; i++)
    {
        uint8_t bit = handler->gpio_port_dir[i] == io_expander_gpio_input ? io_expander_gpio_input : io_expander_gpio_output;
        // only setting the required bit
        if(bit == io_expander_gpio_input) byte_wrote |=  (1<<i);
        if(bit == io_expander_gpio_output) handler->__current_direction_state &=  ~(1<<i);
    }
    ret = handler->i2c_write(byte_wrote, 1, ( (handler->address)<<1) | 0);
    if(ret != STATE_SUCCESS) return STATE_WRITE_FAILED;

    handler->__last_write = byte_wrote;


    #ifdef USE_PRINTF_LOGGING
        printf(" Bytes wrote : %X\n", byte_wrote);
    #endif

    #ifdef USE_PRINTF_LOGGING
        printf(" Task Completed ! \n");
    #endif
    return STATE_SUCCESS;
}




ErrorState_t io_expander_gpio_write(IoExpanderHandler_t *handler, uint8_t port, IoExpanderGpioLevel_t level)
{
    uint8_t byte_wrote = 0;

    if(handler==NULL)
        return STATE_NULL_POINTER;

    if(handler->i2c_write == NULL || handler->i2c_read == NULL)
        return STATE_NULL_POINTER;

    if(handler->address > 0x6f || handler->address < 0x60)
        return STATE_INVALID_DATA;

    if(port > (NUM_OF_PORTS -1) ) return STATE_INVALID_DATA;
    
    if(level >= io_expander_gpio_level_max) return STATE_INVALID_DATA;

    // checking if the port is really output
    if(  (handler->__current_direction_state >> port) & 1 ) return STATE_INVALID_DATA;

    #ifdef USE_PRINTF_LOGGING
        printf("\n\n Error conditions passed ! \n");
    #endif

    if(level == io_expander_gpio_low)
    {
        byte_wrote = ( handler->__last_write ) & ~(1<<port ) ;
    }

    if(level == io_expander_gpio_high)
    {
        byte_wrote = ( handler->__last_write ) | (1<<port ) ;
    }

    ErrorState_t ret = handler->i2c_write(byte_wrote, 1, ( (handler->address)<<1) | 0);
    if(ret != STATE_SUCCESS) return STATE_WRITE_FAILED;

    handler->__last_write = byte_wrote;

    return STATE_SUCCESS;

}



ErrorState_t io_expander_gpio_read(IoExpanderHandler_t *handler, uint8_t port, IoExpanderGpioLevel_t *level)
{
    
    #ifdef USE_PRINTF_LOGGING
        printf("\n\n Function is called ! \n");
    #endif

    uint8_t byte_read = 0;

    if(handler==NULL)
        return STATE_NULL_POINTER;

    if(level == NULL) return STATE_NULL_POINTER;

    if(handler->i2c_write == NULL || handler->i2c_read == NULL)
        return STATE_NULL_POINTER;

    if(handler->address > 0x6f || handler->address < 0x60)
        return STATE_INVALID_DATA;

    if(port > (NUM_OF_PORTS -1) ) return STATE_INVALID_DATA;
    

    #ifdef USE_PRINTF_LOGGING
        printf("\n\n Error conditions passed ! \n");
    #endif

    ErrorState_t ret = handler->i2c_read(&byte_read, 1, ( (handler->address)<<1) | 1);
    if(ret != STATE_SUCCESS) return STATE_READ_FAILED;

    if( ((byte_read >> port) & 1) == io_expander_gpio_low)
        *level = io_expander_gpio_low;
    else if( ((byte_read >> port) & 1) == io_expander_gpio_high)
        *level = io_expander_gpio_high;
    else return STATE_FAIL;
    
    return STATE_SUCCESS;
    
}



/*
void io_expander_int_handler(IoExpanderHandler_t *handler);

*/

void io_expander_int_handler(IoExpanderHandler_t *handler)
{

}







