#include <iostream>
#include "tools.h"
#include "InvFile.h"
#include "query_set.h"

#ifdef RUN_GTEST
#include <gtest/gtest.h>
#endif

using namespace std;

int main(int argc, char** argv) {
    if(argc != 2) {
        cout << "Usage: ./comp4133 <query file>" << endl;
        return -1;
    }

    // Initialization
    Global_Tools = new tools("data/stopword/estop.lst", "data/file.txt");
    InvFile inv;
    inv.Build("data/data/post1.txt");
    inv.BuildDocument(TF_double_norm_K, IDF_idf);
    QuerySet querySet(argv[1]);
    BooleanModelEnhanced bme(&inv);
    querySet.IterateQueries(bme);
    //BooleanModel bm(&inv);
    //querySet.IterateQueries(bm);
    //VSM vsm(&inv);
    //querySet.IterateQueries(vsm);

#ifdef RUN_GTEST
	::testing::InitGoogleTest(&argc, argv);
	cout << "RUN_ALL_TESTS(): " << RUN_ALL_TESTS() << endl;
    //InvFile inv;
    //inv.Build("data/post1.txt");
#endif

	return 0;
}
