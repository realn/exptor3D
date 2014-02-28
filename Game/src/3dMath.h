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
#ifndef _3DMATH_H_
#define _3DMATH_H_

#include <math.h>

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
	Vector3f Cross( const Vector3f &V1 );

	// DotProduct
	float Dot( const Vector3f &V1 );

	// D³ugoœæ wektora
	float Leangth();

	// D³ugoœæ wektora, ale niespierwiastkowana
	float LeangthSq();

	// Normalizacja
	Vector3f Normalize();

	// Odwrócenie
	Vector3f Reverse();
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
};

/*	KLASA KUK£ :)
	S³u¿¹ mi do wykrywania kolizji miêdzy
	obiektami ró¿nych klas. Wiêkszoœæ
	obiektów sceny z niej dzidziczy.
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