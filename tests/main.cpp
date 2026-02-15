#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4700)
#endif

#include "tmech_test.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
