#pragma once

#include "WeaponBullet.h"

// Menager pocisków
class CBulletManager
{
private:
	std::vector<CBullet*> List;

public:
	void AddBullet( CBullet* bullet );
	void DeleteBullet( unsigned int index );
	CBullet* GetBullet( unsigned int index );

	void Update( const float fTD );
	void Render();

	float DoTest( CDynamic* Dum, float Armor = 0.0f );
	void Clear();
};

extern CBulletManager BManager;
