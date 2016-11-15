#include <iostream>
#include "tools.h"
#include "InvFile.h"

#ifdef RUN_GTEST
#include <gtest/gtest.h>
#endif

using namespace std;

int main(int argc, char** argv) {
    // Initialization
    Global_Tools = new tools("data/estop.lst");

#ifdef RUN_GTEST
	::testing::InitGoogleTest(&argc, argv);
	cout << "RUN_ALL_TESTS(): " << RUN_ALL_TESTS() << endl;
    //InvFile inv;
    //inv.Build("data/post1.txt");
#endif

	return 0;
}
