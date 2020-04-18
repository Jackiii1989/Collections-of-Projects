#ifndef ThreadBase_INCLUDED
#define ThreadBase_INCLUDED

// A simple encapsulation for threads
// use this as a baser class 
// derived classe should provide an Init() method and must overrride the
// pure virtual Run() method

#include<Windows.h>

class ThreadBase {

public:
	ThreadBase();
	virtual ~ThreadBase();

	
	
	
	// control behaviour
	virtual void Start() const;
	virtual void Stop() const;

	// state check:
	// true if the thread has been created sucessfully
	// false otherwise
	virtual bool IsThreadCreated() const;


	// returns a duplicate handle of the thread
	// this handle is even valid after destroying this thread object
	// and can be used on wair function
	// note : call CloseHandle() afterwards
	virtual HANDLE GetDuplicateHdl() const;

protected:
	// returns the internal thread handle for derived classes only
	virtual HANDLE GetThreadHdl() const;
	ThreadBase(ThreadBase const &) = delete;  // do not allow copying
	ThreadBase & operator = (ThreadBase const &) = delete;		// do not allow assigment

private:
	// the thread function
	static unsigned long WINAPI ThreadFunc(void * param);
	
	// the thread Run funktion (pure virtual)
	virtual int Run() = 0;

	HANDLE mThreadHdl;
	unsigned long mThreadId;



};


#endif
