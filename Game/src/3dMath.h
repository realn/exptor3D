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

#include <math.h>
#include "StrEx.h"

#define PIOVER180  0.0174532925199432957692369076848861f
#define POW(P) (P*P)

/*	KLASA WEKTOR�W
	Ta klasa bardzo si� przyda przy
	grafice 3D. Jako, �e zale�y nam
	na prostocie, b�dzie jednocze�nie
	s�u�y�a za wierzcho�ek i wektor, oraz
	punkty 2D i 3D.
*/
class Vector3f
{
public:
	//	Sk�adowe
	float X, Y, Z;

	// Konstruktory i Destruktor
	Vector3f();
	Vector3f( const Vector3f& V1 );
	Vector3f( const float &x, const float &y, const float &z );
	~Vector3f();

	// Funkcja ustawiaj�ca wszystkie sk�adowe na raz ( by by�o mniej pisania :P )
	Vector3f Set( const float &x, const float &y, const float &z );

	/*	OPERATORY
		By ta klasa by�a u�yteczna, to musi
		mie� operatory, by m�c operowa� na
		tych obiektach. Mog�em zrobi� oddzielne
		funkcje i operowa� na strukturze, ale
		to przecie� C++ a nie C ;)
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

	// operator mno�enia
	Vector3f operator*( const Vector3f &V1 ) const;
	Vector3f operator*( const float &V1 ) const;

	// operator dzielenia
	Vector3f operator/( const Vector3f &V1 ) const;
	Vector3f operator/( const float &V1 ) const;

	// operatory ��czone:
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
		W grafice 3D wyst�puje kilka
		bardzo przydatnych wzor�w, kt�re
		operuj� na wierzcho�kach i wektorach.
		By korzysta� z tych dobrodziejstw
		napisa�em wbudowane metody.
		( BTW: Kiedy� robi�em to na
		odzielnych funkcjach, ale to
		tylko wprowadza�o zam�t w kodzie :P )
	*/
	// CrossProduct
	const Vector3f Cross( const Vector3f &V1 ) const;

	// DotProduct
	const float Dot( const Vector3f &V1 ) const;

	// D�ugo�� wektora
	const float Length() const;

	// D�ugo�� wektora, ale niespierwiastkowana
	const float LengthSq() const;

	// Normalizacja
	Vector3f Normalize() const;

	// Odwr�cenie
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

/*	KLASA P�ASZCZYZN
	P�aszczyzna, to niczym nie ograniczona
	powieszchnia, kt�ra posiada tylko kierunek
	przedni oraz przesuni�cie. Jest BARDZO
	przydatna je�eli chodzi o grafike 3D.
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
/// Klasa obiekt�w 
///	Fizyczny obiekt opsiadaj�cy pozycj� 
/// oraz promie� obj�to�ci.
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
///	Jest to rodzaj obiekt�w posiadaj�cych dynamik�.
///	Objawia si� ona zwrotem/kierunkiem w kt�ry mog�
/// by� zwr�cone oraz pozycj� w kt�rej chc� si� znale��.
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