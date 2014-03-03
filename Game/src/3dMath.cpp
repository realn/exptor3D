/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	3dMath.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> 3dMath.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "3dMath.h"

/*	KONTRUKTORY */
Vector3f::Vector3f()
{
	/*	Tu ¿adna rewelacja.
		Po prostu zerujemy
		sk³adowe.
	*/
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

Vector3f::Vector3f( const Vector3f& V1 )
{
	/*	Tu przypisujemy
		sk³adowym wartoœci
		sk³adowych wektora V1
	*/
	X = V1.X;
	Y = V1.Y;
	Z = V1.Z;
}

Vector3f::Vector3f( const float &x, const float &y, const float &z )
{
	/*	Niewielka ró¿nica w stosunku
		do poprzedniej metody. Po prostu
		przypisujemy podane wartoœci.
	*/
	Set( x, y, z );
}

/*	DESTRUKTOR	*/
Vector3f::~Vector3f()
{
	// Raczej nie ma czego usuwaæ :P
}

/*	METODA USTAWIAJ¥CA
	Du¿o ona nie robi, po prostu
	przypisuje wartoœci do sk³adowych.
*/
Vector3f Vector3f::Set( const float &x, const float &y, const float &z )
{
	X = x;
	Y = y;
	Z = z;

	// Przy okazji zwracamy ten wektor, by nie mieæ konflików, przy ró¿nych operacjach
	return *this;
}

/*	OPERATORY
	Dzia³enie tych opertorów
	jest na tyle proste, ¿e
	nie bêde ich opisywa³.
	( czyt. nie chce mi siê :P )
*/
Vector3f Vector3f::operator=( const Vector3f &V1 )
{
	Set( V1.X, V1.Y, V1.Z );
	return *this;
}

Vector3f Vector3f::operator=( const float &V1 )
{
	Set( V1, V1, V1 );
	return *this;
}

Vector3f Vector3f::operator+( const Vector3f &V1 ) const
{
	return Vector3f( X + V1.X, Y + V1.Y, Z + V1.Z );
}

Vector3f Vector3f::operator+( const float &V1 ) const
{
	return Vector3f( X + V1, Y + V1, Z + V1 );
}

Vector3f Vector3f::operator-( const Vector3f &V1 ) const
{
	return Vector3f( X - V1.X, Y - V1.Y, Z - V1.Z );
}

Vector3f Vector3f::operator-( const float &V1 ) const
{
	return Vector3f( X - V1, Y - V1, Z - V1 );	
}

Vector3f Vector3f::operator*( const Vector3f &V1 ) const
{
	return Vector3f( X * V1.X, Y * V1.Y, Z * V1.Z );
}

Vector3f Vector3f::operator*( const float &V1 ) const
{
	return Vector3f( X * V1, Y * V1, Z * V1 );
}

Vector3f Vector3f::operator/( const Vector3f &V1 ) const
{
	return Vector3f( X / V1.X, Y / V1.Y, Z / V1.Z );
}

Vector3f Vector3f::operator/( const float &V1 ) const
{
	return Vector3f( X / V1, Y / V1, Z / V1 );	
}

Vector3f Vector3f::operator+=( const Vector3f &V1 )
{
	return *this = *this + V1;
}

Vector3f Vector3f::operator+=( const float &V1 )
{
	return *this = *this + V1;
}

Vector3f Vector3f::operator-=( const Vector3f &V1 )
{
	return *this = *this - V1;
}

Vector3f Vector3f::operator-=( const float &V1 )
{
	return *this = *this - V1;
}

Vector3f Vector3f::operator*=( const Vector3f &V1 )
{
	return *this = *this * V1;
}

Vector3f Vector3f::operator*=( const float &V1 )
{
	return *this = *this * V1;
}

Vector3f Vector3f::operator/=( const Vector3f &V1 )
{
	return *this = *this / V1;
}

Vector3f Vector3f::operator/=( const float &V1 )
{
	return *this = *this / V1;
}

/*	METODY SPECJALNE	*/

/*	CROSS PRODUCT
	Dziêki tej funkcji otrzymamy wektor
	prostopad³y do dwóch pozosta³ych
	wektorów.
*/
Vector3f Vector3f::Cross( const Vector3f &V1 )
{
	Vector3f zwrot;
	zwrot.X = ( Y * V1.Z ) - ( V1.Y * Z );
	zwrot.Y = ( X * V1.Z ) - ( V1.X * Z );
	zwrot.Z = ( X * V1.Y ) - ( V1.X * Y );

	return zwrot;
}

/*	DOT PRODUCT
	Hmm trudno mi wyjaœniæ do czego
	s³u¿y ta metoda. Zwraca ona liczbê
	zmiennoprzecinkow¹ i... Sami siê
	zreszt¹ przekonacie :P
*/
float Vector3f::Dot( const Vector3f &V1 )
{
	return X * V1.X + Y * V1.Y + Z * V1.Z;
}

/*	D£UGOŒÆ
	W przypadku wektorów mo¿na ³atwo
	policzyæ ich d³ugoœæ. Poni¿ej podana
	metoda jest po prostu rozwiniêciem
	wzoru Pitagorasa ( dodano 3 wymiar :) )
*/
float Vector3f::Leangth()
{
	return sqrtf( X * X + Y * Y + Z * Z );
}

float Vector3f::LeangthSq()
{
	return X * X + Y * Y + Z * Z;
}

/*	NORMALIZACJA
	Normalizacja jest skróceniem wektora
	do d³ugoœci równej 1. Najproœciej to
	wykonaæ poprzez dzielenie jego sk³adowych
	przez jego dotychczasow¹ d³ugoœæ.
*/
Vector3f Vector3f::Normalize()
{
	float len = Leangth();
	X /= len;
	Y /= len;
	Z /= len;

	return *this;
}

/*	ODWRÓCENIE
	No chyba nie powiecie, ¿e siê nie
	domyœlacie :)
*/
Vector3f Vector3f::Reverse()
{
	Vector3f zwrot;
	zwrot.Set( -X, -Y, -Z );
	return zwrot;
}

Vector3f CDynamic::GetBlockPos()
{
	Vector3f zwrot;
	zwrot.X = float( (int)Pos.X / 10 );
	zwrot.Z = -float( (int)Pos.Z / 10 );
	return zwrot;
}

/*	DYSTANS
	Ta funkcja oblicza odleg³oœæ
	miêdzy dwoma punktami.
*/
float mathDist( const Vector3f& V1, const Vector3f& V2 )
{
	Vector3f vect = V2 - Vector3f(V1);
	return vect.Leangth();
}

float mathDistSq( const Vector3f& V1, const Vector3f& V2 )
{
	Vector3f vect = V2 - V1;
	return vect.LeangthSq();
}

Vector3f ClosestPoint( const Vector3f &V1, const Vector3f &V2, const Vector3f &Point )
{
	Vector3f Vect1, Vect2;
	Vect1 = Point - V1;
	Vect2 = V2 - V1;
	Vect2.Normalize();
	float d = mathDist( V1, V2 );
	float t = Vect2.Dot( Vect1 );

	if( t <= 0 )
		return V1;

	if( t >= d )
		return V2;

	Vector3f Wynik = V1 + Vect2 * t;

	return Wynik;
}

float GetAngle( const Vector3f &V1, const Vector3f &V2 )
{
	
	float R = mathDist( V1, V2 );

	float sinus = ( V2.X - V1.X ) / R; 
	float cosinus = ( V2.Z - V1.Z ) / R;

	//wyliczamy dwa nowe katy na podstawie arc sin i cos
	float kat = asinf(sinus) / PIOVER180;
	float kat2 = acosf(cosinus) / PIOVER180;

	//porównujemy, je¿eli oba k¹ty s¹ wiêksze 
	//od zera to zwracamy ten oparty na arc cos
	if(kat >= 0 && kat2 >= 0)
	{
		return kat2;
	}
	//jednak je¿eli k¹t na podstawie arc sin jest mniejszy od zera
	//to zwracamy k¹t jaki powstanie gdy od pe³nego okrêgu odejmniemy
	//k¹t na podstawie arc cos (jest dodatni) wtedy uzyskamy prawdziwy
	//k¹t
	else if(kat < 0 && kat2 > 0)
	{
		float rkat = 360.0f - kat2;

		return rkat;
	}
	else return 0.0f;
}

float SwapAngle( const float &Angle )
{
	if( Angle < 0.0f )
		return Angle + 360.0f;
	if( Angle > 360.0f )
		return Angle - 360.0f;

	return Angle;
}