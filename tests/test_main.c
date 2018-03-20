#include "unity_fixture.h"

static void run_tests(void)
{
    RUN_TEST_GROUP(fake_timer);
    RUN_TEST_GROUP(spy_task);
    RUN_TEST_GROUP(scheduler);
    RUN_TEST_GROUP(i2c_master);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, run_tests);
}
