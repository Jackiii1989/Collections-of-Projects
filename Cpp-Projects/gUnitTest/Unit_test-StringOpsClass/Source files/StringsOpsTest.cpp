#include "StringsOpsTest.h"


void StringsOpsTest::SetUp()
{
	mObj = new StringsOps();

}


void StringsOpsTest::TearDown()
{
	delete mObj; mObj = 0;
}



TEST_F(StringsOpsTest, AnalyseString) {
	size_t Chars;
	size_t Digits;
	size_t SpecialChar;
	
	mObj->AnalyseString("A-99xs / 23ff", Chars, Digits, SpecialChar);
	EXPECT_EQ(5, Chars);
	EXPECT_EQ(4, Digits) ;
	EXPECT_EQ(4, SpecialChar);


	mObj->AnalyseString("I have no Idea What I am doing, 999999999", Chars, Digits, SpecialChar);
	EXPECT_EQ(23, Chars);
	EXPECT_EQ(9, Digits);
	EXPECT_EQ(9, SpecialChar);


	mObj->AnalyseString("111111111111111111111111111111111", Chars, Digits, SpecialChar);
	EXPECT_EQ(0, Chars);
	EXPECT_EQ(33, Digits);
	EXPECT_EQ(0, SpecialChar);
}


TEST_F(StringsOpsTest, AnalyseStringSpecialCases) {

	size_t Chars;
	size_t Digits;
	size_t SpecialChar;

	mObj->AnalyseString("", Chars, Digits, SpecialChar);
	EXPECT_EQ(0, Chars) ;
	EXPECT_EQ(0, Digits);
	EXPECT_EQ(0, SpecialChar);

	mObj->AnalyseString(" ", Chars, Digits, SpecialChar);
	EXPECT_EQ(0, Chars);
	EXPECT_EQ(0, Digits);
	EXPECT_EQ(1, SpecialChar);

	mObj->AnalyseString("²³{[]}|µ@€~", Chars, Digits, SpecialChar);
	EXPECT_EQ(0, Chars) ;
	EXPECT_EQ(0, Digits);
	EXPECT_EQ(11, SpecialChar) ;

	mObj->AnalyseString("\\\\\\\\\\", Chars, Digits, SpecialChar);
	EXPECT_EQ(0, Chars);
	EXPECT_EQ(0, Digits);
	EXPECT_EQ(5, SpecialChar);

}

TEST_F(StringsOpsTest, CrackUncPathNormal) {
	string Hostname;
	string ShareFolder;
	vector<string> Path;
	string FileName;
	string Extension;


	mObj->CrackUncPath("\\Roadrunner\\shared\\Datasheets\\Specs\\2017\\uart.pdf", Hostname, ShareFolder, Path, FileName, Extension);
	EXPECT_STREQ("Roadrunner", Hostname.c_str()) << "Hostname variable is different;";
	EXPECT_STREQ("shared", ShareFolder.c_str()) << "ShareFolder variable is different;";
	EXPECT_STREQ("Datasheets", Path[0].c_str()) << "Path[0] variable is different;";
	EXPECT_STREQ("Specs", Path[1].c_str()) << "Path[1] variable is different;";
	EXPECT_STREQ("2017", Path[2].c_str()) << "Path[2] variable is different;";
	EXPECT_STREQ("uart", FileName.c_str()) << "FileName variable is different;";
	EXPECT_STREQ("pdf", Extension.c_str()) << "Extension variable is different;";

	Path.clear();
	mObj->CrackUncPath("Host\\Shared\\Datas\\About\\999\\About.obj", Hostname, ShareFolder, Path, FileName, Extension);
	EXPECT_STREQ("Host", Hostname.c_str()) << "Hostname variable is different;";
	EXPECT_STREQ("Shared", ShareFolder.c_str()) << "ShareFolder variable is different;";
	EXPECT_STREQ("Datas", Path[0].c_str()) << "Path[0] variable is different;";
	EXPECT_STREQ("About", Path[1].c_str()) << "Path[1] variable is different;";
	EXPECT_STREQ("999", Path[2].c_str()) << "Path[2] variable is different;";
	EXPECT_STREQ("About", FileName.c_str()) << "FileName variable is different;";
	EXPECT_STREQ("obj", Extension.c_str()) << "Extension variable is different;";

}



TEST_F(StringsOpsTest, CrackUncPathSpecialCases) {

	string ShareFolder;
	string Hostname;
	vector<string> Path;
	string FileName;
	string Extension;

	mObj->CrackUncPath("\\a\\b\\c\\d\\e\\", Hostname, ShareFolder, Path, FileName, Extension);

	EXPECT_STREQ("a", Hostname.c_str());
	EXPECT_STREQ("b", ShareFolder.c_str());
	EXPECT_STREQ("c", Path[0].c_str());
	EXPECT_STREQ("d", Path[1].c_str());
	EXPECT_STREQ("e", Path[2].c_str());
	EXPECT_STREQ("", FileName.c_str());
	EXPECT_STREQ("", Extension.c_str());

	Path.clear();
	mObj->CrackUncPath("\\\\\\\\\\\\", Hostname, ShareFolder, Path, FileName, Extension);
	EXPECT_STREQ("", Hostname.c_str()) ;
	EXPECT_STREQ("", ShareFolder.c_str()) ;
	EXPECT_STREQ("", FileName.c_str()) ;
	EXPECT_STREQ("", Extension.c_str()) ;

}

TEST_F(StringsOpsTest, CrackUncPathShortPath) {

	string ShareFolder;
	string Hostname;
	vector<string> Path;
	string FileName;
	string Extension;


	mObj->CrackUncPath("\\a\\game.exe", Hostname, ShareFolder, Path, FileName, Extension);
	EXPECT_STREQ("a", Hostname.c_str());
	EXPECT_STREQ("game", FileName.c_str());
	EXPECT_STREQ("exe", Extension.c_str());

	Path.clear();
	mObj->CrackUncPath("\\a\\abc\\image.jpg", Hostname, ShareFolder, Path, FileName, Extension);
	EXPECT_STREQ("a", Hostname.c_str());
	EXPECT_STREQ("abc", ShareFolder.c_str());
	EXPECT_STREQ("image", FileName.c_str());
	EXPECT_STREQ("jpg", Extension.c_str());

	Path.clear();
	mObj->CrackUncPath("\\MyFolder\\abc\\c\\action.vhdl", Hostname, ShareFolder, Path, FileName, Extension);
	EXPECT_STREQ("MyFolder", Hostname.c_str());
	EXPECT_STREQ("abc", ShareFolder.c_str());
	EXPECT_STREQ("c", Path[0].c_str());
	EXPECT_STREQ("action", FileName.c_str());
	EXPECT_STREQ("vhdl", Extension.c_str());

}