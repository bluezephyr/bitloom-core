#include "unity_fixture.h"

static void run_tests(void)
{
    RUN_TEST_GROUP(fake_timer);
    RUN_TEST_GROUP(scheduler);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, run_tests);
}
