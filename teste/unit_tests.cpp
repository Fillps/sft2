/**
 * Testes unitários da biblioteca cthread.
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include "../include/t2fs.h"

#ifdef __cplusplus
}
#endif

#include "gtest/gtest.h"


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
