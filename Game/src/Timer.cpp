#include "Timer.h"

CTimer* Timer = NULL;

bool CTimer::Init()
{
	if(!QueryPerformanceFrequency( (LARGE_INTEGER*)&this->iFraq ) )
	{
		this->bWinClock = true;
		this->ulMMFirstTick = this->ulMMLastTick = GetTickCount();
		this->iFraq = 1000;
	}
	else
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&this->iFirstTick);
		this->iLastTick = this->iFirstTick;
		this->bWinClock = false;
	}
	return true;
}

void CTimer::Update()
{
	unsigned long MMThisTick = 0;
	__int64 ThisTick = 0;

	if( bWinClock )
	{
		MMThisTick = GetTickCount();
		fDT = float(((double)MMThisTick-(double)this->ulMMLastTick) / double(iFraq) );
		ulMMLastTick = MMThisTick;
	}
	else
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&ThisTick);
		fDT = float( ( (double)ThisTick - (double)this->iLastTick ) / double(iFraq) );
		iLastTick = ThisTick;
	}
	if( fDT != 0.0f )
		fFPS = 1.0f / fDT;
}

void CTimer::Free()
{
}