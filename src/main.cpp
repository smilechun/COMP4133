#include <iostream>
#include "tools.h"

#ifdef RUN_GTEST
#include <gtest/gtest.h>
#endif

using namespace std;

int main(int argc, char** argv) {
#ifdef RUN_GTEST
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
#endif
    tools mytools("data/estop.lst");
    cout << mytools.is_stop("ABC") << endl;
    cout << mytools.is_stop("Is") << endl;
    cout << mytools.is_stop("Is", 0) << endl;
    cout << mytools.is_stop("is", 0) << endl;
    cout << mytools.is_stop("is", -1) << endl;
    return 0;
}
