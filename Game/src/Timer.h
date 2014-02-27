#ifndef _FLEX_TIMER_H_
#define _FLEX_TIMER_H_

#include <windows.h>

class CTimer
{
protected:
	bool bWinClock;
	__int64 iFirstTick;
	__int64 iLastTick;
	__int64 iFraq;
	
	unsigned long ulMMFirstTick;
	unsigned long ulMMLastTick;

	float fDT;
	float fFPS;

public:
	CTimer()
	{ bWinClock = false;	};
	virtual ~CTimer()
	{	Free();	};
	
	virtual bool Init();
	virtual void Update();
	virtual void Free();

	virtual float GetFPS()
	{	return fFPS;	};
	virtual float GetDT()
	{	return fDT;	};
};

extern CTimer* Timer;

#endif