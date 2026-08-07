#include "unity_fixture.h"
#include "io_expander_controller.h"

static void RunAllTests(void)
{
    RUN_TEST_GROUP(io_expander_test_group_init_current_state);
    RUN_TEST_GROUP(io_expander_test_group_init);
    RUN_TEST_GROUP(io_expander_test_group_init_last_write);
    RUN_TEST_GROUP(io_expander_test_group_write);
    RUN_TEST_GROUP(io_expander_test_group_read);

}


int main(int argc, const char *argv[])
{
    printf("\n\n ************ HELLO WORLD ************\n\n");
    return UnityMain(argc, argv, RunAllTests);      
}









