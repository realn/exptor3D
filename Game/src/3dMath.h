/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	3dMath.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas i funkcji u³atwiaj¹ce 
		prowadzenia ró¿norakich obliczeñ matematycznych.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <math.h>
#include "StrEx.h"

#define PIOVER180  0.0174532925199432957692369076848861f
#define POW(P) (P*P)

/*	KLASA WEKTORÓW
	Ta klasa bardzo siê przyda przy
	grafice 3D. Jako, ¿e zale¿y nam
	na prostocie, bêdzie jednoczeœnie
	s³u¿y³a za wierzcho³ek i wektor, oraz
	punkty 2D i 3D.
*/
class Vector3f
{
public:
	//	Sk³adowe
	float X, Y, Z;

	// Konstruktory i Destruktor
	Vector3f();
	Vector3f( const Vector3f& V1 );
	Vector3f( const float &x, const float &y, const float &z );
	~Vector3f();

	// Funkcja ustawiaj¹ca wszystkie sk³adowe na raz ( by by³o mniej pisania :P )
	Vector3f Set( const float &x, const float &y, const float &z );

	/*	OPERATORY
		By ta klasa by³a u¿yteczna, to musi
		mieæ operatory, by móc operowaæ na
		tych obiektach. Mog³em zrobiæ oddzielne
		funkcje i operowaæ na strukturze, ale
		to przecie¿ C++ a nie C ;)
	*/
	//	operator przypisania
	Vector3f operator=( const Vector3f &V1 );
	Vector3f operator=( const float &V1 );

	// operator dodawania
	Vector3f operator+( const Vector3f &V1 ) const;
	Vector3f operator+( const float &V1 ) const;

	// operator odejmowania
	Vector3f operator-( const Vector3f &V1 ) const;
	Vector3f operator-( const float &V1 ) const;
	Vector3f operator-() const;

	// operator mno¿enia
	Vector3f operator*( const Vector3f &V1 ) const;
	Vector3f operator*( const float &V1 ) const;

	// operator dzielenia
	Vector3f operator/( const Vector3f &V1 ) const;
	Vector3f operator/( const float &V1 ) const;

	// operatory ³¹czone:
	Vector3f operator+=( const Vector3f &V1 );
	Vector3f operator+=( const float &V1 );
	Vector3f operator-=( const Vector3f &V1 );
	Vector3f operator-=( const float &V1 );
	Vector3f operator*=( const Vector3f &V1 );
	Vector3f operator*=( const float &V1 );
	Vector3f operator/=( const Vector3f &V1 );
	Vector3f operator/=( const float &V1 );	

	const bool	operator==( const Vector3f& vert ) const
	{
		return X == vert.X && Y == vert.Y && Z == vert.Z;
	}
	const bool	operator!=( const Vector3f& vert ) const
	{
		return !(*this == vert);
	}

	/*	METODY SPECJALNE
		W grafice 3D wystêpuje kilka
		bardzo przydatnych wzorów, które
		operuj¹ na wierzcho³kach i wektorach.
		By korzystaæ z tych dobrodziejstw
		napisa³em wbudowane metody.
		( BTW: Kiedyœ robi³em to na
		odzielnych funkcjach, ale to
		tylko wprowadza³o zamêt w kodzie :P )
	*/
	// CrossProduct
	const Vector3f Cross( const Vector3f &V1 ) const;

	// DotProduct
	const float Dot( const Vector3f &V1 ) const;

	// D³ugoœæ wektora
	const float Length() const;

	// D³ugoœæ wektora, ale niespierwiastkowana
	const float LengthSq() const;

	// Normalizacja
	Vector3f Normalize() const;

	// Odwrócenie
	Vector3f Reverse() const;

	const std::string	ToString() const{
		return "VEC3F( " + FloatToStr( X ) + ", " + FloatToStr( Y ) + ", " + FloatToStr( Z ) + " )";
	}
};

class Vector2f
{
public:
	float	X;
	float	Y;

	Vector2f() : X( 0.0f ), Y( 0.0f ){}
	Vector2f( const float x, const float y ) : X( x ), Y( y ){}
	Vector2f( const Vector2f& vert ) : X(vert.X), Y(vert.Y){}

	const Vector2f	operator+( const Vector2f& vert ) const
	{
		return Vector2f( X + vert.X, Y + vert.Y );
	}
	const Vector2f	operator*( const Vector2f& vert ) const
	{
		return Vector2f( X * vert.X, Y * vert.Y );
	}

	void	operator=( const Vector2f& vert )
	{
		X = vert.X;
		Y = vert.Y;
	}

	void	operator+=( const Vector2f& vert )
	{
		*this = *this + vert;
	}

	const bool	operator==( const Vector2f& vert ) const
	{
		return X == vert.X && Y == vert.Y;
	}
	const bool	operator!=( const Vector2f& vert ) const
	{
		return !(*this == vert);
	}
};

/*	KLASA P£ASZCZYZN
	P³aszczyzna, to niczym nie ograniczona
	powieszchnia, która posiada tylko kierunek
	przedni oraz przesuniêcie. Jest BARDZO
	przydatna je¿eli chodzi o grafike 3D.
*/
class Planef
{
public:
	Vector3f Normal;
	float D;

public:
	Planef();
	Planef(const Vector3f& normal, const float distance);
	Planef(const Vector3f& normal, const Vector3f& point);
	Planef(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);

	void	Set(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);
	void	Set(const Vector3f& normal, const Vector3f& pos);

	const float	Distance( const Vector3f& pos ) const;
	const bool	Intersects( const Vector3f& origin, const Vector3f& dest, Vector3f& outIntersect ) const;

	const std::string	ToString() const
	{
		return "PLANEF( " + Normal.ToString() + ", " + FloatToStr( D ) + " )";
	}
};

/// <summary>
/// Klasa obiektów 
///	Fizyczny obiekt opsiadaj¹cy pozycjê 
/// oraz promieñ objêtoœci.
/// </summary>
class CObject
{
protected:
	float		Angle;

public:
	Vector3f	Pos;
	float		Radius;

public:
	CObject() : Radius(1.0f), Angle(0.0f){}
	CObject(const float radius) : Radius(radius){}
	virtual ~CObject(){}

	virtual void		SetAngle(const float angle){ Angle = angle; }
	virtual const float	GetAngle() const { return Angle; }

	virtual const bool	IsCollidable() const { return true; }
};

/// <summary>
/// Klasa jednostek
///	Jest to rodzaj obiektów posiadaj¹cych dynamikê.
///	Objawia siê ona zwrotem/kierunkiem w który mog¹
/// byæ zwrócone oraz pozycj¹ w której chc¹ siê znaleŸæ.
/// </summary>
class CDynamic : 
	public CObject
{
public:
	Vector3f	NextPos;
	Vector3f	Vector;

public:
	CDynamic() : CObject(3.0f){};
	CDynamic(const float radius) : CObject(radius){}
	virtual ~CDynamic(){}

	const Vector3f	GetMoveVector() const { return NextPos - Pos; }

	Vector3f GetBlockPos();

	virtual const bool	OnCollision( CObject* pObject ){ return false; };
	virtual const bool	OnCollision( const Vector3f& point, const Planef& plane ){ return false; }
};

extern const Vector3f	MakeNormal( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3 );
extern const Vector3f	MakeVectorXZ( const float angle );
extern const float		Distance( const Vector3f& V1, const Vector3f& V2 );
extern const float		DistanceSq( const Vector3f& V1, const Vector3f& V2 );
extern const float	TriangleSide( const float A, const float B);
extern Vector3f		ClosestPoint( const Vector3f &V1, const Vector3f &V2, const Vector3f &Point );
extern float		GetAngle( const Vector3f &V1, const Vector3f &V2 );
extern const float	SwapAngle( const float &Angle );
extern const float	MaxValue( const float A, const float B );
extern const float	MinValue( const float A, const float B );