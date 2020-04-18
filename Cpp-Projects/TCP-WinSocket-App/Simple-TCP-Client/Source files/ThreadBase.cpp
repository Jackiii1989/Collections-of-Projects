// A simple encapsulation for threads

// Implementation

#include<cassert>
#include"ThreadBase.h"

ThreadBase::ThreadBase() {

	// created the thread,  but fo not start it so for
	mThreadHdl = CreateThread(nullptr, 0, ThreadFunc, this, CREATE_SUSPENDED, &mThreadId);

	// sanity check if thread has  been created
	assert(mThreadHdl != nullptr);

}


ThreadBase::~ThreadBase(){
	CloseHandle(mThreadHdl);
}

// calls the thread run method; param is the 'this' ponter to access our own members
unsigned long WINAPI ThreadBase::ThreadFunc(void * param) {

	assert(param != nullptr);
	ThreadBase *pObj = static_cast<ThreadBase *> (param);
	return pObj->Run(); // now call the thread function	
	// if you are on this position the thread is going to terminate
}

// launch the thread to overcume CREATE_SUSpend
void ThreadBase::Start() const {
	ResumeThread(mThreadHdl);
}

// stop the executing user mode
void ThreadBase::Stop() const {
	SuspendThread(mThreadHdl);
}

//return true if the thread has been created
// note : this does not mean that thr thread is running
bool ThreadBase::IsThreadCreated() const {
	return mThreadHdl != nullptr;
}


// return the internal thread handle 
// thius handle is invalid after destrying the thread object
HANDLE ThreadBase::GetThreadHdl() const {
	return mThreadHdl;
}

HANDLE ThreadBase:: GetDuplicateHdl() const{

	HANDLE hdlDup = nullptr;
	BOOL res = DuplicateHandle(GetCurrentProcess(), mThreadHdl, GetCurrentProcess(), &hdlDup,
		0, FALSE, DUPLICATE_SAME_ACCESS);
		assert(res != 0);
	return hdlDup;
}
