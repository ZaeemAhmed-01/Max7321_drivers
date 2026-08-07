

/*

micro tests
any null pointor, 
address passed correctly
all the data that is not used by inti must be initiealized with zeros.
Does it actually, sets the input output modes of the gpios told by user
any out of bound problems.
fucntion is actually called
*/



#include "unity_fixture.h"
#include "io_expander_controller.h"
#include "fake_i2c.h"

/*---------------------------------------------------------------------------
 * Helpers
 *--------------------------------------------------------------------------*/

#define DEVICE_ADDRESS  (0x68U)

static IoExpanderHandler_t makeValidHandler(void)
{
    IoExpanderHandler_t handler;

    handler.address   = DEVICE_ADDRESS;
    handler.i2c_read  = fakeI2cRead;
    handler.i2c_write = fakeI2cWrite;

    for (int i = 0; i < NUM_OF_PORTS; i++)
    {
        handler.input_state_change_callback[i] = NULL;
        handler.gpio_port_dir[i] = io_expander_gpio_output;
    }

    return handler;
}




/*---------------------------------------------------------------------------
 * io_expander_gpio_write tests
 *--------------------------------------------------------------------------*/

TEST_GROUP(io_expander_test_group_write);

TEST_SETUP(io_expander_test_group_write)
{
    mockI2cReset();
}

TEST_TEAR_DOWN(io_expander_test_group_write)
{
}

TEST_GROUP_RUNNER(io_expander_test_group_write)
{
    /* --- null --- */
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_NULL_POINTER_when_handler_null);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_NULL_POINTER_when_i2c_write_null);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_NULL_POINTER_when_i2c_read_null);

    /* --- boundaries --- */
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_address_below_range);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_address_above_range);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_port_out_of_range);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_level_invalid);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_port_is_input);

    /* --- direction state must not be touched --- */
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_does_not_modify_current_direction_state);

    /* --- i2c call correctness --- */
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_calls_i2c_write_once);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_calls_i2c_write_with_correct_address);

    /* --- byte building logic --- */
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_sets_target_bit_high_and_preserves_rest);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_sets_target_bit_low_and_preserves_rest);

    /* --- last_write updates --- */
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_updates_last_write_on_success);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_does_not_update_last_write_when_i2c_write_fails);

    /* --- return values --- */
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_WRITE_FAILED_when_i2c_write_fails);
    RUN_TEST_CASE(io_expander_test_group_write, gpio_write_returns_SUCCESS_on_valid_write);
}






/*---------------------------------------------------------------------------
 * io_expander_gpio_read tests
 *--------------------------------------------------------------------------*/

TEST_GROUP(io_expander_test_group_read);

TEST_SETUP(io_expander_test_group_read)
{
    mockI2cReset();
}

TEST_TEAR_DOWN(io_expander_test_group_read)
{
}

TEST_GROUP_RUNNER(io_expander_test_group_read)
{
    /* --- null --- */
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_handler_null);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_i2c_write_null);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_i2c_read_null);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_level_null);

    /* --- boundaries --- */
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_INVALID_DATA_when_address_below_range);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_INVALID_DATA_when_address_above_range);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_INVALID_DATA_when_port_out_of_range);

    /* --- i2c call correctness --- */
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_calls_i2c_read_once);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_calls_i2c_read_with_read_bit_set);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_calls_i2c_read_with_correct_address_bits);

    /* --- bit extraction --- */
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_sets_level_high_when_bit_set);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_sets_level_low_when_bit_clear);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_extracts_correct_bit_from_mixed_byte);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_allows_reading_port_configured_as_output);

    /* --- failure handling --- */
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_READ_FAILED_when_i2c_read_fails);
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_does_not_modify_level_when_i2c_read_fails);

    /* --- happy path --- */
    RUN_TEST_CASE(io_expander_test_group_read, gpio_read_returns_SUCCESS_on_valid_read);
}








/*---------------------------------------------------------------------------
 * Test group for init funciton. Last write thing
 *--------------------------------------------------------------------------*/

TEST_GROUP(io_expander_test_group_init_last_write);

TEST_SETUP(io_expander_test_group_init_last_write)
{
    mockI2cReset();
}

TEST_TEAR_DOWN(io_expander_test_group_init_last_write)
{
}

TEST_GROUP_RUNNER(io_expander_test_group_init_last_write)
{
    RUN_TEST_CASE(io_expander_test_group_init_last_write, io_expander_controller_init_sets_last_write_to_0x00_when_all_ports_output);
    RUN_TEST_CASE(io_expander_test_group_init_last_write, io_expander_controller_init_sets_last_write_to_0xFF_when_all_ports_input);
    RUN_TEST_CASE(io_expander_test_group_init_last_write, io_expander_controller_init_sets_last_write_matching_i2c_write_data);
    RUN_TEST_CASE(io_expander_test_group_init_last_write, io_expander_controller_init_does_not_modify_last_write_when_init_fails);
    RUN_TEST_CASE(io_expander_test_group_init_last_write, io_expander_controller_init_does_not_modify_last_write_when_i2c_write_fails);
}


/*---------------------------------------------------------------------------
 * Test group for init funciton.
 *--------------------------------------------------------------------------*/

TEST_GROUP(io_expander_test_group_init);
TEST_GROUP(io_expander_test_group_init_current_state);


TEST_SETUP(io_expander_test_group_init)
{
    mockI2cReset();
}
TEST_SETUP(io_expander_test_group_init_current_state)
{
    mockI2cReset();
}

TEST_TEAR_DOWN(io_expander_test_group_init)
{

}

TEST_TEAR_DOWN(io_expander_test_group_init_current_state)
{

}


TEST_GROUP_RUNNER(io_expander_test_group_init_current_state)
{
    /* -- current state --- */
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_sets_current_output_state_high_for_input_ports);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_sets_current_output_state_low_for_output_ports);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_sets_current_output_state_0xFF_when_all_ports_input);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_i2c_write_data_matches_current_output_state);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_modify_current_output_state_when_init_fails);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_call_i2c_read_when_address_invalid);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_call_i2c_write_when_address_invalid);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_call_i2c_write_when_device_not_present);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_returns_STATE_FAIL_regardless_of_i2c_read_error_code);
    RUN_TEST_CASE(io_expander_test_group_init_current_state, io_expander_controller_init_returns_STATE_WRITE_FAILED_regardless_of_i2c_write_error_code);
}


TEST_GROUP_RUNNER(io_expander_test_group_init)
{
    /* --- null pointer --- */
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_NULL_POINTER_when_handler_is_null);

    /* --- i2c read called correctly --- */
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_calls_i2c_read_once_to_confirm_device);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_calls_i2c_read_with_read_bit_set_in_address);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_calls_i2c_read_with_correct_device_address_bits);

    /* --- device not present --- */
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_FAIL_when_i2c_read_returns_error);

    /* --- happy path --- */
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_SUCCESS_when_device_present);

    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_INVALID_DATA_when_address_below_valid_range);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_INVALID_DATA_when_address_above_valid_range);

    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_NULL_POINTER_when_i2c_read_is_null);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_NULL_POINTER_when_i2c_write_is_null);

    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_writes_0x00_when_all_ports_are_output);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_writes_0xFF_when_all_ports_are_input);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_writes_correct_byte_when_ports_are_mixed);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_calls_i2c_write_exactly_once);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_calls_i2c_write_with_write_bit_clear_in_address);
    RUN_TEST_CASE(io_expander_test_group_init, io_expander_controller_init_returns_STATE_WRITE_FAILED_when_i2c_write_fails);
}


/*---------------------------------------------------------------------------
 * Null pointer guard
 *--------------------------------------------------------------------------*/

TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_NULL_POINTER_when_handler_is_null)
{
    ErrorState_t ret = io_expander_controller_init(NULL);

    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

/*---------------------------------------------------------------------------
 * i2c read called correctly
 *--------------------------------------------------------------------------*/

TEST(io_expander_test_group_init, io_expander_controller_init_calls_i2c_read_once_to_confirm_device)
{
    IoExpanderHandler_t handler = makeValidHandler();

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(1, mockI2cReadGetCallCount());
}

TEST(io_expander_test_group_init, io_expander_controller_init_calls_i2c_read_with_read_bit_set_in_address)
{
    IoExpanderHandler_t handler = makeValidHandler();

    io_expander_controller_init(&handler);

    /* bit 0 must be 1 for a read transaction */
    TEST_ASSERT_EQUAL(1, mockI2cReadGetLastAddress() & 0x01);
}

TEST(io_expander_test_group_init, io_expander_controller_init_calls_i2c_read_with_correct_device_address_bits)
{
    IoExpanderHandler_t handler = makeValidHandler();

    io_expander_controller_init(&handler);

    /* bits 7:1 must match the configured device address */
    TEST_ASSERT_EQUAL(DEVICE_ADDRESS, (mockI2cReadGetLastAddress() >> 1));
}

/*---------------------------------------------------------------------------
 * Device not present
 *--------------------------------------------------------------------------*/

TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_FAIL_when_i2c_read_returns_error)
{
    IoExpanderHandler_t handler = makeValidHandler();

    mockI2cReadSetReturnValue(STATE_FAIL);

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_FAIL, ret);
}

/*---------------------------------------------------------------------------
 * Happy path
 *--------------------------------------------------------------------------*/

TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_SUCCESS_when_device_present)
{
    IoExpanderHandler_t handler = makeValidHandler();

    mockI2cReadSetReturnValue(STATE_SUCCESS);

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);
}




/*---------------------------------------------------------------------------
 * Address passed by the user is correct
 *--------------------------------------------------------------------------*/
TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_INVALID_DATA_when_address_below_valid_range)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x5F;

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}



TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_INVALID_DATA_when_address_above_valid_range)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x70;

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

/*---------------------------------------------------------------------------
 * i2c Null pointer test
 *--------------------------------------------------------------------------*/
TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_NULL_POINTER_when_i2c_read_is_null)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.i2c_read = NULL;

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_NULL_POINTER_when_i2c_write_is_null)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.i2c_write = NULL;

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

/*---------------------------------------------------------------------------
 * calling the i2c functions
 *--------------------------------------------------------------------------*/
TEST(io_expander_test_group_init, io_expander_controller_init_writes_0xFF_when_all_ports_are_input)
{
    IoExpanderHandler_t handler = makeValidHandler();

    for (int i = 0; i < NUM_OF_PORTS; i++)
        handler.gpio_port_dir[i] = io_expander_gpio_input;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0xFF, mockI2cWriteGetLastData());
}

TEST(io_expander_test_group_init, io_expander_controller_init_writes_0x00_when_all_ports_are_output)
{
    IoExpanderHandler_t handler = makeValidHandler();

    for (int i = 0; i < NUM_OF_PORTS; i++)
        handler.gpio_port_dir[i] = io_expander_gpio_output;

    ErrorState_t ret = io_expander_controller_init(&handler);
    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);
    int ret_mockI2cWriteGetCallCount = mockI2cWriteGetCallCount();
    TEST_ASSERT_EQUAL(1, ret_mockI2cWriteGetCallCount);
    TEST_ASSERT_EQUAL(0x00, mockI2cWriteGetLastData());
}

TEST(io_expander_test_group_init, io_expander_controller_init_writes_correct_byte_when_ports_are_mixed)
{
    IoExpanderHandler_t handler = makeValidHandler();

    /* ports 0 and 2 input → expected byte 0x05 (bits 0 and 2 set) */
    for (int i = 0; i < NUM_OF_PORTS; i++)
        handler.gpio_port_dir[i] = io_expander_gpio_output;

    handler.gpio_port_dir[0] = io_expander_gpio_input;
    handler.gpio_port_dir[2] = io_expander_gpio_input;

    ErrorState_t ret = io_expander_controller_init(&handler);
    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0x05, mockI2cWriteGetLastData());
}

TEST(io_expander_test_group_init, io_expander_controller_init_calls_i2c_write_exactly_once)
{
    IoExpanderHandler_t handler = makeValidHandler();

    ErrorState_t ret = io_expander_controller_init(&handler);
    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1, mockI2cWriteGetCallCount());
}

TEST(io_expander_test_group_init, io_expander_controller_init_calls_i2c_write_with_write_bit_clear_in_address)
{
    IoExpanderHandler_t handler = makeValidHandler();

    ErrorState_t ret = STATE_FAIL;
    ret = io_expander_controller_init(&handler);
    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);

    /* bit 0 must be 0 for a write transaction */
    int ret_mockI2cWriteGetCallCount = mockI2cWriteGetCallCount();
    TEST_ASSERT_EQUAL(1, ret_mockI2cWriteGetCallCount);

    uint8_t ret_mockI2cWriteGetLastAddress = mockI2cWriteGetLastAddress() & 0x01;
    TEST_ASSERT_EQUAL(0, ret_mockI2cWriteGetLastAddress );
}

TEST(io_expander_test_group_init, io_expander_controller_init_returns_STATE_WRITE_FAILED_when_i2c_write_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();

    mockI2cWriteSetReturnValue(STATE_WRITE_FAILED);

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_WRITE_FAILED, ret);
}
/*---------------------------------------------------------------------------
 * Current state of GPIO levels
 *--------------------------------------------------------------------------*/
TEST(io_expander_test_group_init_current_state, io_expander_controller_init_sets_current_output_state_high_for_input_ports)
{
    IoExpanderHandler_t handler = makeValidHandler();

    handler.gpio_port_dir[0] = io_expander_gpio_output;
    handler.gpio_port_dir[1] = io_expander_gpio_output;
    handler.gpio_port_dir[2] = io_expander_gpio_input;
    handler.gpio_port_dir[3] = io_expander_gpio_input;
    handler.gpio_port_dir[4] = io_expander_gpio_input;
    handler.gpio_port_dir[5] = io_expander_gpio_input;
    handler.gpio_port_dir[6] = io_expander_gpio_input;
    handler.gpio_port_dir[7] = io_expander_gpio_input;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL_HEX8(0xFC, handler.__current_direction_state);
}


TEST(io_expander_test_group_init_current_state, io_expander_controller_init_sets_current_output_state_low_for_output_ports)
{
    IoExpanderHandler_t handler = makeValidHandler();

    for (int i = 0; i < NUM_OF_PORTS; i++)
        handler.gpio_port_dir[i] = io_expander_gpio_output;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0x00, handler.__current_direction_state);
}

TEST(io_expander_test_group_init_current_state, io_expander_controller_init_sets_current_output_state_0xFF_when_all_ports_input)
{
    IoExpanderHandler_t handler = makeValidHandler();

    for (int i = 0; i < NUM_OF_PORTS; i++)
        handler.gpio_port_dir[i] = io_expander_gpio_input;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0xFF, handler.__current_direction_state);
}





TEST(io_expander_test_group_init_current_state, io_expander_controller_init_i2c_write_data_matches_current_output_state)
{
    IoExpanderHandler_t handler = makeValidHandler();

    handler.gpio_port_dir[0] = io_expander_gpio_output;
    handler.gpio_port_dir[1] = io_expander_gpio_input;
    handler.gpio_port_dir[2] = io_expander_gpio_output;
    handler.gpio_port_dir[3] = io_expander_gpio_input;
    handler.gpio_port_dir[4] = io_expander_gpio_output;
    handler.gpio_port_dir[5] = io_expander_gpio_input;
    handler.gpio_port_dir[6] = io_expander_gpio_output;
    handler.gpio_port_dir[7] = io_expander_gpio_input;

    io_expander_controller_init(&handler);

    /* input ports 1,3,5,7 → bits set → 0xAA, output ports 0,2,4,6 → bits clear */
    TEST_ASSERT_EQUAL(0xAA, handler.__current_direction_state);
    TEST_ASSERT_EQUAL(handler.__current_direction_state, mockI2cWriteGetLastData());
}


TEST(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_modify_current_output_state_when_init_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0xAB;

    mockI2cReadSetReturnValue(STATE_FAIL);

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0xAB, handler.__current_direction_state);
}



TEST(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_call_i2c_read_when_address_invalid)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x70;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0, mockI2cReadGetCallCount());
}

TEST(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_call_i2c_write_when_address_invalid)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x70;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0, mockI2cWriteGetCallCount());
}

TEST(io_expander_test_group_init_current_state, io_expander_controller_init_does_not_call_i2c_write_when_device_not_present)
{
    IoExpanderHandler_t handler = makeValidHandler();
    mockI2cReadSetReturnValue(STATE_FAIL);

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0, mockI2cWriteGetCallCount());
}

TEST(io_expander_test_group_init_current_state, io_expander_controller_init_returns_STATE_FAIL_regardless_of_i2c_read_error_code)
{
    IoExpanderHandler_t handler = makeValidHandler();
    mockI2cReadSetReturnValue(STATE_TIMEOUT);

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_FAIL, ret);
}

TEST(io_expander_test_group_init_current_state, io_expander_controller_init_returns_STATE_WRITE_FAILED_regardless_of_i2c_write_error_code)
{
    IoExpanderHandler_t handler = makeValidHandler();
    mockI2cWriteSetReturnValue(STATE_PORT_UNAVAILABLE);

    ErrorState_t ret = io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(STATE_WRITE_FAILED, ret);
}



TEST(io_expander_test_group_init_last_write, io_expander_controller_init_sets_last_write_to_0x00_when_all_ports_output)
{
    IoExpanderHandler_t handler = makeValidHandler();

    handler.__last_write = 0xFF;
    for (int i = 0; i < NUM_OF_PORTS; i++)
        handler.gpio_port_dir[i] = io_expander_gpio_output;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0x00, handler.__last_write);
}

TEST(io_expander_test_group_init_last_write, io_expander_controller_init_sets_last_write_to_0xFF_when_all_ports_input)
{
    IoExpanderHandler_t handler = makeValidHandler();

    for (int i = 0; i < NUM_OF_PORTS; i++)
        handler.gpio_port_dir[i] = io_expander_gpio_input;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0xFF, handler.__last_write);
}

TEST(io_expander_test_group_init_last_write, io_expander_controller_init_sets_last_write_matching_i2c_write_data)
{
    IoExpanderHandler_t handler = makeValidHandler();

    handler.gpio_port_dir[0] = io_expander_gpio_output;
    handler.gpio_port_dir[1] = io_expander_gpio_input;
    handler.gpio_port_dir[2] = io_expander_gpio_output;
    handler.gpio_port_dir[3] = io_expander_gpio_input;
    handler.gpio_port_dir[4] = io_expander_gpio_output;
    handler.gpio_port_dir[5] = io_expander_gpio_input;
    handler.gpio_port_dir[6] = io_expander_gpio_output;
    handler.gpio_port_dir[7] = io_expander_gpio_input;

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(mockI2cWriteGetLastData(), handler.__last_write);
}

TEST(io_expander_test_group_init_last_write, io_expander_controller_init_does_not_modify_last_write_when_init_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__last_write = 0xAB;

    mockI2cReadSetReturnValue(STATE_FAIL);

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0xAB, handler.__last_write);
}

TEST(io_expander_test_group_init_last_write, io_expander_controller_init_does_not_modify_last_write_when_i2c_write_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__last_write = 0xAB;

    mockI2cWriteSetReturnValue(STATE_WRITE_FAILED);

    io_expander_controller_init(&handler);

    TEST_ASSERT_EQUAL(0xAB, handler.__last_write);
}
















TEST(io_expander_test_group_write, gpio_write_returns_NULL_POINTER_when_handler_null)
{
    ErrorState_t ret = io_expander_gpio_write(NULL, 0, io_expander_gpio_high);
    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_NULL_POINTER_when_i2c_write_null)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.i2c_write = NULL;

    ErrorState_t ret = io_expander_gpio_write(&handler, 0, io_expander_gpio_high);
    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_NULL_POINTER_when_i2c_read_null)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.i2c_read = NULL;

    ErrorState_t ret = io_expander_gpio_write(&handler, 0, io_expander_gpio_high);
    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_address_below_range)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x5F;

    ErrorState_t ret = io_expander_gpio_write(&handler, 0, io_expander_gpio_high);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_address_above_range)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x70;

    ErrorState_t ret = io_expander_gpio_write(&handler, 0, io_expander_gpio_high);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_port_out_of_range)
{
    IoExpanderHandler_t handler = makeValidHandler();

    ErrorState_t ret = io_expander_gpio_write(&handler, NUM_OF_PORTS, io_expander_gpio_high);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_level_invalid)
{
    IoExpanderHandler_t handler = makeValidHandler();

    ErrorState_t ret = io_expander_gpio_write(&handler, 0, io_expander_gpio_level_max);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_INVALID_DATA_when_port_is_input)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x08; /* port 3 is input */

    ErrorState_t ret = io_expander_gpio_write(&handler, 3, io_expander_gpio_high);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_write, gpio_write_does_not_modify_current_direction_state)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0xF0;
    handler.__last_write              = 0xF0;

    io_expander_gpio_write(&handler, 0, io_expander_gpio_high);

    TEST_ASSERT_EQUAL(0xF0, handler.__current_direction_state);
}

TEST(io_expander_test_group_write, gpio_write_calls_i2c_write_once)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x00;
    handler.__last_write              = 0x00;

    io_expander_gpio_write(&handler, 0, io_expander_gpio_high);

    TEST_ASSERT_EQUAL(1, mockI2cWriteGetCallCount());
}

TEST(io_expander_test_group_write, gpio_write_calls_i2c_write_with_correct_address)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x00;
    handler.__last_write              = 0x00;

    io_expander_gpio_write(&handler, 0, io_expander_gpio_high);

    TEST_ASSERT_EQUAL(0, mockI2cWriteGetLastAddress() & 0x01);
    TEST_ASSERT_EQUAL(DEVICE_ADDRESS, mockI2cWriteGetLastAddress() >> 1);
}

TEST(io_expander_test_group_write, gpio_write_sets_target_bit_high_and_preserves_rest)
{
    IoExpanderHandler_t handler = makeValidHandler();
    /* ports 4-7 input, ports 0-3 output, port1 currently high */
    handler.__current_direction_state = 0xF0;
    handler.__last_write              = 0xF2;

    io_expander_gpio_write(&handler, 0, io_expander_gpio_high);

    /* port0 set, port1 preserved, inputs forced to 1 -> 0xF3 */
    TEST_ASSERT_EQUAL(0xF3, mockI2cWriteGetLastData());
}

TEST(io_expander_test_group_write, gpio_write_sets_target_bit_low_and_preserves_rest)
{
    IoExpanderHandler_t handler = makeValidHandler();
    /* ports 4-7 input, ports 0-3 output, ports 0 and 1 currently high */
    handler.__current_direction_state = 0xF0;
    handler.__last_write              = 0xF3;

    io_expander_gpio_write(&handler, 0, io_expander_gpio_low);

    /* port0 cleared, port1 preserved, inputs forced to 1 -> 0xF2 */
    TEST_ASSERT_EQUAL(0xF2, mockI2cWriteGetLastData());
}

TEST(io_expander_test_group_write, gpio_write_updates_last_write_on_success)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x00;
    handler.__last_write              = 0x00;

    io_expander_gpio_write(&handler, 3, io_expander_gpio_high);

    TEST_ASSERT_EQUAL(0x08, handler.__last_write);
}

TEST(io_expander_test_group_write, gpio_write_does_not_update_last_write_when_i2c_write_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x00;
    handler.__last_write              = 0xAB;

    mockI2cWriteSetReturnValue(STATE_WRITE_FAILED);

    io_expander_gpio_write(&handler, 3, io_expander_gpio_high);

    TEST_ASSERT_EQUAL(0xAB, handler.__last_write);
}

TEST(io_expander_test_group_write, gpio_write_returns_WRITE_FAILED_when_i2c_write_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x00;

    mockI2cWriteSetReturnValue(STATE_WRITE_FAILED);

    ErrorState_t ret = io_expander_gpio_write(&handler, 0, io_expander_gpio_high);

    TEST_ASSERT_EQUAL(STATE_WRITE_FAILED, ret);
}

TEST(io_expander_test_group_write, gpio_write_returns_SUCCESS_on_valid_write)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x00;

    ErrorState_t ret = STATE_FAIL;
    ret = io_expander_gpio_write(&handler, 0, io_expander_gpio_high);

    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);
}

































TEST(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_handler_null)
{
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    ErrorState_t ret = io_expander_gpio_read(NULL, 0, &level);
    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_i2c_write_null)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.i2c_write = NULL;
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    ErrorState_t ret = io_expander_gpio_read(&handler, 0, &level);
    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_i2c_read_null)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.i2c_read = NULL;
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    ErrorState_t ret = io_expander_gpio_read(&handler, 0, &level);
    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_read, gpio_read_returns_NULL_POINTER_when_level_null)
{
    IoExpanderHandler_t handler = makeValidHandler();

    ErrorState_t ret = io_expander_gpio_read(&handler, 0, NULL);
    TEST_ASSERT_EQUAL(STATE_NULL_POINTER, ret);
}

TEST(io_expander_test_group_read, gpio_read_returns_INVALID_DATA_when_address_below_range)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x5F;
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    ErrorState_t ret = io_expander_gpio_read(&handler, 0, &level);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_read, gpio_read_returns_INVALID_DATA_when_address_above_range)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.address = 0x70;
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    ErrorState_t ret = io_expander_gpio_read(&handler, 0, &level);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_read, gpio_read_returns_INVALID_DATA_when_port_out_of_range)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    ErrorState_t ret = io_expander_gpio_read(&handler, NUM_OF_PORTS, &level);
    TEST_ASSERT_EQUAL(STATE_INVALID_DATA, ret);
}

TEST(io_expander_test_group_read, gpio_read_calls_i2c_read_once)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level ;

    io_expander_gpio_read(&handler, 0, &level);

    int ret = mockI2cReadGetCallCount();
    TEST_ASSERT_EQUAL(1, ret );
}

TEST(io_expander_test_group_read, gpio_read_calls_i2c_read_with_read_bit_set)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level ;

    uint8_t byte = 0xff;
    mockI2cReadSetData(&byte, 1);
    io_expander_gpio_read(&handler, 0, &level);
    uint8_t ret = mockI2cReadGetLastAddress() & 0x01;
    TEST_ASSERT_EQUAL(1, ret);
}

TEST(io_expander_test_group_read, gpio_read_calls_i2c_read_with_correct_address_bits)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level ;

    io_expander_gpio_read(&handler, 0, &level);

    TEST_ASSERT_EQUAL(DEVICE_ADDRESS, mockI2cReadGetLastAddress() >> 1);
}

TEST(io_expander_test_group_read, gpio_read_sets_level_high_when_bit_set)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level ;
    uint8_t data = 0x01; /* port 0 high */

    mockI2cReadSetData(&data, 1);

    io_expander_gpio_read(&handler, 0, &level);

    TEST_ASSERT_EQUAL(io_expander_gpio_high, level);
}

TEST(io_expander_test_group_read, gpio_read_sets_level_low_when_bit_clear)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level ;
    uint8_t data = 0x00; /* port 0 low */

    mockI2cReadSetData(&data, 1);

    io_expander_gpio_read(&handler, 0, &level);

    TEST_ASSERT_EQUAL(io_expander_gpio_low, level);
}

TEST(io_expander_test_group_read, gpio_read_extracts_correct_bit_from_mixed_byte)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level ;
    uint8_t data = 0xA5; /* 1010 0101 */

    mockI2cReadSetData(&data, 1);

    io_expander_gpio_read(&handler, 3, &level); /* bit 3 = 0 */
    TEST_ASSERT_EQUAL(io_expander_gpio_low, level);

    io_expander_gpio_read(&handler, 5, &level); /* bit 5 = 1 */
    TEST_ASSERT_EQUAL(io_expander_gpio_high, level);
}

TEST(io_expander_test_group_read, gpio_read_allows_reading_port_configured_as_output)
{
    IoExpanderHandler_t handler = makeValidHandler();
    handler.__current_direction_state = 0x00; /* all output */
    IoExpanderGpioLevel_t level ;
    uint8_t data = 0x04; /* port 2 high */

    mockI2cReadSetData(&data, 1);

    ErrorState_t ret = io_expander_gpio_read(&handler, 2, &level);

    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);
    TEST_ASSERT_EQUAL(io_expander_gpio_high, level);
}

TEST(io_expander_test_group_read, gpio_read_returns_READ_FAILED_when_i2c_read_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level ;

    mockI2cReadSetReturnValue(STATE_READ_FAILED);

    ErrorState_t ret = io_expander_gpio_read(&handler, 0, &level);

    TEST_ASSERT_EQUAL(STATE_READ_FAILED, ret);
}

TEST(io_expander_test_group_read, gpio_read_does_not_modify_level_when_i2c_read_fails)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    mockI2cReadSetReturnValue(STATE_READ_FAILED);

    io_expander_gpio_read(&handler, 0, &level);

    TEST_ASSERT_EQUAL(io_expander_gpio_level_max, level);
}

TEST(io_expander_test_group_read, gpio_read_returns_SUCCESS_on_valid_read)
{
    IoExpanderHandler_t handler = makeValidHandler();
    IoExpanderGpioLevel_t level = io_expander_gpio_level_max;

    ErrorState_t ret = io_expander_gpio_read(&handler, 0, &level);

    TEST_ASSERT_EQUAL(STATE_SUCCESS, ret);
}