#include <iostream>
#include "tools.h"
#include "InvFile.h"
#include "query_set.h"
#include "parse_opts.h"

#ifdef RUN_GTEST
#include <gtest/gtest.h>
#endif

using namespace std;

int main(int argc, char** argv) {
    Options *opts = parse_opts(argc, argv);
    if(!opts) {
        print_usage();
        return -1;
    }

    // Initialization
    // Read stop word lists and file.txt
    Global_Tools = new tools("data/stopword/estop.lst", "data/stopword/querystop.lst", "data/file.txt");

    // Build inverted file
    InvFile inv;
    if(inv.Build("data/data/post1.txt"))
        return 1;

    CAL_TF cal_tf;
    CAL_IDF cal_idf;
    if(opts->tf_function==1) {
        cal_tf =  TF_binary;
    } else if(opts->tf_function==2) {
        cal_tf = TF_raw_freq;
    } else if(opts->tf_function==3) {
        cal_tf = TF_log_norm;
    } else if(opts->tf_function==4) {
        cal_tf = TF_double_norm;
    } else if(opts->tf_function==5) {
        cal_tf = TF_double_norm_K;
    }
    
    if(opts->idf_function==1) {
        cal_idf = IDF_unary;
    } else if(opts->idf_function==2) {
        cal_idf = IDF_idf;
    } else if(opts->idf_function==3) {
        cal_idf = IDF_idf_smooth;
    } else if(opts->idf_function==4) {
        cal_idf = IDF_idf_max;
    } else if(opts->idf_function==5) {
        cal_idf = IDF_prob_idf;
    }
    
    inv.BuildDocument(cal_tf, cal_idf);

    // Process query
    QuerySet querySet(opts->query_file);

    if(opts->retrieval_model == Options::RetrievalModel::VSM) {
        VSM vsm(&inv);
        querySet.IterateQueries(vsm);
    } else if(querySet.long_query && 
                opts->retrieval_model == Options::RetrievalModel::Boolean_NLP) {
        BooleanModelEnhanced bme(&inv);
        querySet.IterateQueries(bme);
    } else {
        BooleanModel bm(&inv, querySet.long_query);
        querySet.IterateQueries(bm);
    }

#ifdef RUN_GTEST
	::testing::InitGoogleTest(&argc, argv);
	cout << "RUN_ALL_TESTS(): " << RUN_ALL_TESTS() << endl;
    //InvFile inv;
    //inv.Build("data/post1.txt");
#endif

	return 0;
}
