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
	/*	Tu �adna rewelacja.
		Po prostu zerujemy
		sk�adowe.
	*/
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

Vector3f::Vector3f( const Vector3f& V1 )
{
	/*	Tu przypisujemy
		sk�adowym warto�ci
		sk�adowych wektora V1
	*/
	X = V1.X;
	Y = V1.Y;
	Z = V1.Z;
}

Vector3f::Vector3f( const float &x, const float &y, const float &z )
{
	/*	Niewielka r�nica w stosunku
		do poprzedniej metody. Po prostu
		przypisujemy podane warto�ci.
	*/
	Set( x, y, z );
}

/*	DESTRUKTOR	*/
Vector3f::~Vector3f()
{
	// Raczej nie ma czego usuwa� :P
}

/*	METODA USTAWIAJ�CA
	Du�o ona nie robi, po prostu
	przypisuje warto�ci do sk�adowych.
*/
Vector3f Vector3f::Set( const float &x, const float &y, const float &z )
{
	X = x;
	Y = y;
	Z = z;

	// Przy okazji zwracamy ten wektor, by nie mie� konflik�w, przy r�nych operacjach
	return *this;
}

/*	OPERATORY
	Dzia�enie tych opertor�w
	jest na tyle proste, �e
	nie b�de ich opisywa�.
	( czyt. nie chce mi si� :P )
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

Vector3f Vector3f::operator-() const
{
	return Vector3f( -X, -Y, -Z );
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
	Dzi�ki tej funkcji otrzymamy wektor
	prostopad�y do dw�ch pozosta�ych
	wektor�w.

			|  x  y  z |
	A x B = | Ax Ay Az |
			| Bx By Bz |

	A x B = | Ay Az | * x - | Ax Az | * y + | Ax Ay | * z
			| By Bz |		| Bx Bz |		| Bx By |

	A x B = ( Ay * Bz - Az * By ) * x - ( Ax * Bz - Az * Bx ) * y + ( Ax * By - Ay * Bx ) * z

	x =  ( Ay * Bz - Az * By )
	y = -( Ax * Bz - Az * Bx )
	z =  ( Ax * By - Ay * Bx )
*/
const Vector3f Vector3f::Cross( const Vector3f &V1 ) const
{
	return Vector3f(
		 ( Y * V1.Z - Z * V1.Y ),
		-( X * V1.Z - Z * V1.X ),
		 ( X * V1.Y - Y * V1.X ) 
		);
}

/*	DOT PRODUCT
	Hmm trudno mi wyja�ni� do czego
	s�u�y ta metoda. Zwraca ona liczb�
	zmiennoprzecinkow� i... Sami si�
	zreszt� przekonacie :P
*/
const float Vector3f::Dot( const Vector3f &V1 ) const
{
	return X * V1.X + Y * V1.Y + Z * V1.Z;
}

/*	D�UGO��
	W przypadku wektor�w mo�na �atwo
	policzy� ich d�ugo��. Poni�ej podana
	metoda jest po prostu rozwini�ciem
	wzoru Pitagorasa ( dodano 3 wymiar :) )
*/
const float Vector3f::Leangth() const
{
	return sqrtf( LeangthSq() );
}

const float Vector3f::LeangthSq() const
{
	return Dot( *this );
}

/*	NORMALIZACJA
	Normalizacja jest skr�ceniem wektora
	do d�ugo�ci r�wnej 1. Najpro�ciej to
	wykona� poprzez dzielenie jego sk�adowych
	przez jego dotychczasow� d�ugo��.
*/
Vector3f Vector3f::Normalize() const
{
	float len = Leangth();
	if( len == 0.0f )
		return Vector3f();

	return Vector3f( X / len, Y / len, Z / len );
}

/*	ODWR�CENIE
	No chyba nie powiecie, �e si� nie
	domy�lacie :)
*/
Vector3f Vector3f::Reverse() const
{
	return Vector3f( -X, -Y, -Z );
}

Vector3f CDynamic::GetBlockPos()
{
	Vector3f zwrot;
	zwrot.X = float( (int)Pos.X / 10 );
	zwrot.Z = -float( (int)Pos.Z / 10 );
	return zwrot;
}



Planef::Planef() :
	D( 0.0f )
{}

Planef::Planef( const Vector3f& normal, const float distance ) :
	Normal( normal ),
	D( distance )
{}

Planef::Planef( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3 ) :
	D( 0.0f )
{
	Set( v1, v2, v3 );
}

void	Planef::Set( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3 )
{
	Normal = MakeNormal( v1, v2, v3 );
	D = -Normal.Dot( v1 );
}



const Vector3f	MakeNormal( const Vector3f& v1, const Vector3f& v2, const Vector3f& v3 )
{
	Vector3f vec1 = v2 - v1;
	Vector3f vec2 = v3 - v1;

	return vec1.Cross(vec2).Normalize();
}

const Vector3f	MakeVectorXZ( const float angle )
{
	return Vector3f(
		sinf( angle * PIOVER180 ),
		0.0f,
		-cosf( angle * PIOVER180 )
		);
}

/*	DYSTANS
	Ta funkcja oblicza odleg�o��
	mi�dzy dwoma punktami.
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

const float	TriangleSide( const float a, const float b )
{
	return sqrtf( POW(a) + POW(b) );
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

	//por�wnujemy, je�eli oba k�ty s� wi�ksze 
	//od zera to zwracamy ten oparty na arc cos
	if(kat >= 0 && kat2 >= 0)
	{
		return kat2;
	}
	//jednak je�eli k�t na podstawie arc sin jest mniejszy od zera
	//to zwracamy k�t jaki powstanie gdy od pe�nego okr�gu odejmniemy
	//k�t na podstawie arc cos (jest dodatni) wtedy uzyskamy prawdziwy
	//k�t
	else if(kat < 0 && kat2 > 0)
	{
		float rkat = 360.0f - kat2;

		return rkat;
	}
	else return 0.0f;
}

const float SwapAngle( const float &Angle )
{
	if( Angle < 0.0f )
		return Angle + 360.0f;
	if( Angle > 360.0f )
		return Angle - 360.0f;

	return Angle;
}