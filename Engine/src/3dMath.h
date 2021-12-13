/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	3dMath.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas i funkcji u�atwiaj�ce 
		prowadzenia r�norakich oblicze� matematycznych.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <glm/vec3.hpp>

#include <CBCore/StringConvert.h>

#include <math.h>
#include "StrEx.h"

constexpr float PIOVER180 = 0.0174532925199432957692369076848861f;
constexpr float POW(float P) { return P * P; }

/*	KLASA P�ASZCZYZN
	P�aszczyzna, to niczym nie ograniczona
	powieszchnia, kt�ra posiada tylko kierunek
	przedni oraz przesuni�cie. Jest BARDZO
	przydatna je�eli chodzi o grafike 3D.
*/
class Planef
{
public:
	glm::vec3 Normal;
	float D = 0.0f;

public:
	Planef();
	Planef(const glm::vec3& normal, const float distance);
	Planef(const glm::vec3& normal, const glm::vec3& point);
	Planef(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

	void	Set(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
	void	Set(const glm::vec3& normal, const glm::vec3& pos);

	const float	Distance( const glm::vec3& pos ) const;
	const bool	Intersects( const glm::vec3& origin, const glm::vec3& dest, glm::vec3& outIntersect ) const;
};

/// <summary>
/// Klasa obiekt�w 
///	Fizyczny obiekt opsiadaj�cy pozycj� 
/// oraz promie� obj�to�ci.
/// </summary>
class CObject
{
protected:
	float		Angle = 0.0f;

public:
	glm::vec3	Pos;
	float		Radius = 0.0f;

public:
	CObject() = default;
	CObject(const float radius) : Radius(radius){}
	virtual ~CObject() = default;

	virtual void		SetAngle(const float angle){ Angle = angle; }
	virtual const float	GetAngle() const { return Angle; }

	virtual const bool	IsCollidable() const { return true; }
};

/// <summary>
/// Klasa jednostek
///	Jest to rodzaj obiekt�w posiadaj�cych dynamik�.
///	Objawia si� ona zwrotem/kierunkiem w kt�ry mog�
/// by� zwr�cone oraz pozycj� w kt�rej chc� si� znale��.
/// </summary>
class CDynamic : 
	public CObject
{
public:
	glm::vec3	NextPos;
	glm::vec3	Vector;

public:
	CDynamic() : CObject(3.0f){};
	CDynamic(const float radius) : CObject(radius){}
	virtual ~CDynamic(){}

	const glm::vec3	GetMoveVector() const { return NextPos - Pos; }

	glm::vec3 GetBlockPos();

	virtual const bool	OnCollision( CObject* pObject ){ return false; };
	virtual const bool	OnCollision( const glm::vec3& point, const Planef& plane ){ return false; }
};

glm::vec3	MakeNormal( const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 );
glm::vec3	MakeVectorXZ( const float degAngle );
float		Distance( const glm::vec3& V1, const glm::vec3& V2 );
float	pythagoreanTheorem( const float A, const float B);
glm::vec3		ClosestPoint( const glm::vec3& V1, const glm::vec3 &V2, const glm::vec3 &Point );
float		GetAngle( const glm::vec3& V1, const glm::vec3 &V2 );
float	SwapAngle( float degAngle );

namespace core {
	template<class _Type>
	bool beetween(_Type value, _Type minValue, _Type maxValue) {
		return minValue < value&& value < maxValue;
	}
}