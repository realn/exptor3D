#include "WeaponBulletRay.h"

#include "Special.h"

/*====================
	KLASA CBullRay
	promieñ phazer'a
====================*/
CBullRay::CBullRay( CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector ) :
	CBullet( owner, damage, pos, vector, 1.0f )
{
	Radius = 0.1f;
	Type = BULLET_TYPE_RAY;
	CanDelete = false;

	CEffectRay* Effect = new CEffectRay( GLevel, pos, vector );

	Pos = Effect->GetFromPos();
	NextPos = Effect->GetToPos();

	CEffectSprite* Spr = new CEffectSprite( SEManager.GetTexMng(), Effect->GetToPos() + ( Vector.Reverse() * 0.2f ), 1.0f, 0.8f, 0.0f );

	SEManager.AddEffect( Effect );
	SEManager.AddEffect( Spr );
}

float CBullRay::DoTest( CDynamic* Dum, float Armor )
{
	float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
	if( Dum != NULL && Dum != Owner )
	{
		if( TestCollDum( Dum, this ) )
		{
			SEManager.AddEffect( new CEffectSprite( SEManager.GetTexMng(), Dum->NextPos, 1.0f, 2.0f, 0.0f ) );
			return Damage * ArmorMod;
		}
	}
	return 0.0f;
}

void CBullRay::Update( const float fTD )
{
	CanDelete = true;
}

void CBullRay::Render()
{
	// Nic...
}

