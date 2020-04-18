#ifndef  LOCK_INCLUDED
#define LOCK_INCLUDED


#include<Windows.h>

// initialize of the Critical Section
class CritSec {
	friend class CSLock;

public:
	CritSec() {
		InitializeCriticalSection(&mCS);
	}

	~CritSec() {
		DeleteCriticalSection(&mCS);
	}

private:
	void Acquire() {
		EnterCriticalSection(&mCS);
	}

	void Release() {
		LeaveCriticalSection(&mCS);
	}

	CritSec(CritSec const &) = delete; // do not allow copying
	CritSec & operator = (CritSec const &) = delete; // do not allow assigment;

	CRITICAL_SECTION mCS; // the critical section variable

};


// apply and release the lock
class CSLock {
public:

	//lock for exclusive access
	CSLock(CritSec & CSObj) : mCsAccess(CSObj) {
		mCsAccess.Acquire();
	}

	// release the lock
	~CSLock() {
		mCsAccess.Release();
	}

private:
	CSLock();			// do not allow default CTor
	CSLock(CSLock const &) = delete;	// do not allow copying
	CSLock & operator = (CSLock const &) = delete; // do not allow assigment

	CritSec &mCsAccess;
};

#endif // ! LOCK_INCLUDED

