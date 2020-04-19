#include <iostream>
#include "gtest/gtest.h" // Note: add google test headers to your Project-Settings!

using namespace std;

int main(int argc, char** argv) {


	// give a random seed, shufle the Test Cases and repeat it four times
	::testing::GTEST_FLAG(random_seed) = 12;
	::testing::GTEST_FLAG(shuffle) = true;
	::testing::GTEST_FLAG(repeat) = 2;

	// generate a test report
	::testing::GTEST_FLAG(output) = "xml:XML_Report.xhtml";

	testing::InitGoogleTest(&argc, argv);
	return  RUN_ALL_TESTS();
	return 0;
}


