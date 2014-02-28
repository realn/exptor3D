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
#ifndef _3DMATH_H_
#define _3DMATH_H_

#include <math.h>

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
	Vector3f Cross( const Vector3f &V1 );

	// DotProduct
	float Dot( const Vector3f &V1 );

	// D�ugo�� wektora
	float Leangth();

	// D�ugo�� wektora, ale niespierwiastkowana
	float LeangthSq();

	// Normalizacja
	Vector3f Normalize();

	// Odwr�cenie
	Vector3f Reverse();
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
};

/*	KLASA KUK� :)
	S�u�� mi do wykrywania kolizji mi�dzy
	obiektami r�nych klas. Wi�kszo��
	obiekt�w sceny z niej dzidziczy.
*/
class Dummy
{
public:
	Vector3f	Pos;
	Vector3f	NextPos;
	Vector3f	Vector;
	float R;

public:
	Dummy() : R(3.0f){};
	virtual ~Dummy(){}

	Vector3f GetBlockPos();
};

extern float	mathDist( const Vector3f& V1, const Vector3f& V2 );
extern float	mathDistSq( const Vector3f& V1, const Vector3f& V2 );
extern Vector3f	ClosestPoint( const Vector3f &V1, const Vector3f &V2, const Vector3f &Point );
extern float	GetAngle( const Vector3f &V1, const Vector3f &V2 );
extern float	SwapAngle( const float &Angle );

#endif