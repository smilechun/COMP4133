#include <gtest/gtest.h>
#include <iostream>
#include "tools.h"
#include "InvFile.h"
#include "boolean.h"

//int is_stop(string word, int ignore_case = 1);
TEST(ToolsTest, is_stop__ignore_case) {
	tools mytools("data/estop.lst");
	ASSERT_TRUE(mytools.is_stop("is"));
	ASSERT_TRUE(mytools.is_stop("Is"));
	ASSERT_TRUE(mytools.is_stop("IS"));
	ASSERT_TRUE(mytools.is_stop("iS"));
	ASSERT_FALSE(mytools.is_stop("issue"));
	ASSERT_FALSE(mytools.is_stop("Issue"));
}

TEST(ToolsTest, is_stop__case_sensitive) {
	tools mytools("data/estop.lst");
	ASSERT_TRUE(mytools.is_stop("is", 0));
	ASSERT_FALSE(mytools.is_stop("Is", 0));
	ASSERT_FALSE(mytools.is_stop("iS", 0));
	ASSERT_FALSE(mytools.is_stop("IS", 0));
	ASSERT_FALSE(mytools.is_stop("issue", 0));
	ASSERT_FALSE(mytools.is_stop("Issue", 0));
}

TEST(ToolsTest, stem__1) {
	string str = "original";
	string str2 = "origin";
	tools mytools("data/estop.lst");
	mytools.stem(str);
	mytools.stem(str2);
	ASSERT_STREQ(str.c_str(), str2.c_str());
}

TEST(ToolsTest, stem__2) {
	string str = "changed";
	string str2 = "change";
	string str3 = "changing";
	tools mytools("data/estop.lst");
	mytools.stem(str);
	mytools.stem(str2);
	mytools.stem(str3);
	ASSERT_STREQ(str.c_str(), str2.c_str());
	ASSERT_STREQ(str.c_str(), str3.c_str());
}

TEST(InvFileTest, add__1) {
    InvFile inv_file;
    ASSERT_EQ(0, inv_file.GetDF("star"));
    inv_file.Add("star", 1);
    ASSERT_EQ(1, inv_file.GetDF("star"));
    inv_file.Add("star2", 1);
    inv_file.Add("oneonestar", 1);
    ASSERT_EQ(1, inv_file.GetDF("star"));
    inv_file.Add("star", 2);
    ASSERT_EQ(2, inv_file.GetDF("star"));
    inv_file.Add("star", 2);
    inv_file.Add("star", 2);
    ASSERT_EQ(2, inv_file.GetDF("star"));
}

TEST(InvFileTest, RetrievalBoolean__1) {
    /*
    InvFile inv_file;
    inv_file.Build("data/post2.txt");
    vector<string> a({"parT", "helP", "Star"});
    inv_file.RetrievalBoolean(a);
    */
}

TEST(BooleanTest, GetNextToken__1) {
    Boolean("word AND (b OR c)  ");
}
