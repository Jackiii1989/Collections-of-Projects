#include "TimeMeas.h"

// retrieve time stamp counter of CPU
__int64 ReadTSC()
{
	int dummy[4];  // for unused returns
	volatile int dontSkip; // volatile to prevernt optimization
	__int64 clock = 0;		// cycles

	__cpuid(dummy, 0);		// serialize to prevent out-of-order exection
	dontSkip = dummy[0];	// prevetn optimizing away cpiud 

	clock = __rdtsc(); // read time stamp counter
	__cpuid(dummy, 0);  // serialiaze to prevent out-of-order execution
	dontSkip = dummy[0]; // prevent optimizing away cpuid
	return clock;	// cycles since cpu start

}

