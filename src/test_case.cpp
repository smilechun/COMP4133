#include <gtest/gtest.h>
#include "tools.h"

TEST(FacTest, IS_STOP) {
    tools mytools("data/estop.lst");
    ASSERT_TRUE(mytools.is_stop("Is"));
}
