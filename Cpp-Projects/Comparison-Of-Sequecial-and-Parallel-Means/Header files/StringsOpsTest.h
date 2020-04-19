#pragma once
#include "gtest\gtest.h"
#include "StringsOps.h"
class StringsOpsTest :public ::testing ::Test
{
public:
	StringsOpsTest() {};
	virtual ~StringsOpsTest() {};


protected:
	virtual void TearDown();
	virtual void SetUp();

	StringsOps* mObj;

};

