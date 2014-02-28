#pragma once

class CTimer
{
protected:
	bool bWinClock;
	unsigned __int64 uFirstTick;
	unsigned __int64 uLastTick;
	unsigned __int64 uFreq;
	
	unsigned long ulMMFirstTick;
	unsigned long ulMMLastTick;

	float fDT;
	float fFPS;

public:
	CTimer();
	~CTimer();
	
	bool Init();
	void Update();
	void Free();

	float GetFPS(){	return fFPS;	}
	float GetDT(){	return fDT;		}
};

extern CTimer* Timer;
