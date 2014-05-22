#include "stdafx.h"
#include "Timer.h"
#include <Windows.h>

CTimer::CTimer() :
	bWinClock(false),
	uFreq(0),
	uFirstTick(0),
	uLastTick(0),
	ulMMFirstTick(0),
	ulMMLastTick(0),
	fDT(0.0f),
	fFPS(0.0f)
{
	if(!QueryPerformanceFrequency( (LARGE_INTEGER*)&this->uFreq ) )
	{
		this->bWinClock = true;
		this->ulMMFirstTick = this->ulMMLastTick = GetTickCount();
		this->uFreq = 1000;
	}
	else
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&this->uFirstTick);
		this->uLastTick = this->uFirstTick;
		this->bWinClock = false;
	}
}

CTimer::~CTimer()
{

}

void CTimer::Update()
{
	if( bWinClock )
	{
		unsigned long MMThisTick = GetTickCount();
		fDT = float(((double)MMThisTick-(double)this->ulMMLastTick) / double(uFreq) );
		ulMMLastTick = MMThisTick;
	}
	else
	{
		unsigned __int64 ThisTick = 0;
		if(QueryPerformanceCounter((LARGE_INTEGER *)&ThisTick))
		{
			fDT = float( ( (double)ThisTick - (double)this->uLastTick ) / double(uFreq) );
			uLastTick = ThisTick;
		}
	}
	if( fDT != 0.0f )
		fFPS = 1.0f / fDT;
}