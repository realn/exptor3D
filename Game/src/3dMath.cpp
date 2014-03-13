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
#include "Log.h"

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
	Dziêki tej funkcji otrzymamy wektor
	prostopad³y do dwóch pozosta³ych
	wektorów.

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
	Hmm trudno mi wyjaœniæ do czego
	s³u¿y ta metoda. Zwraca ona liczbê
	zmiennoprzecinkow¹ i... Sami siê
	zreszt¹ przekonacie :P
*/
const float Vector3f::Dot( const Vector3f &V1 ) const
{
	return X * V1.X + Y * V1.Y + Z * V1.Z;
}

/*	D£UGOŒÆ
	W przypadku wektorów mo¿na ³atwo
	policzyæ ich d³ugoœæ. Poni¿ej podana
	metoda jest po prostu rozwiniêciem
	wzoru Pitagorasa ( dodano 3 wymiar :) )
*/
const float Vector3f::Length() const
{
	return sqrtf( LengthSq() );
}

const float Vector3f::LengthSq() const
{
	return Dot( *this );
}

/*	NORMALIZACJA
	Normalizacja jest skróceniem wektora
	do d³ugoœci równej 1. Najproœciej to
	wykonaæ poprzez dzielenie jego sk³adowych
	przez jego dotychczasow¹ d³ugoœæ.
*/
Vector3f Vector3f::Normalize() const
{
	float len = Length();
	if( len == 0.0f )
		return Vector3f();

	return Vector3f( X / len, Y / len, Z / len );
}

/*	ODWRÓCENIE
	No chyba nie powiecie, ¿e siê nie
	domyœlacie :)
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

Planef::Planef( const Vector3f& normal, const Vector3f& pos ) :
	D( 0.0f )
{
	Set( normal, pos );
}

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

void	Planef::Set( const Vector3f& normal, const Vector3f& pos )
{
	Normal = normal;
	D = -Normal.Dot( pos );
}

const float	Planef::Distance( const Vector3f& pos ) const
{
	return Normal.Dot( pos ) + D;
}

const bool	Planef::Intersects( const Vector3f& origin, const Vector3f& dest, Vector3f& outIntersect ) const
{
	float w = Distance( origin ) * Distance( dest );
	if( w >= 0.0f )
		return false;

	auto vec = dest - origin;

	auto counter = Distance( origin );
	auto denom = -Normal.Dot( vec );

	outIntersect = origin + vec * ( counter / denom );
	return true;
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
		cosf( angle * PIOVER180 )
		);
}

/*	DYSTANS
	Ta funkcja oblicza odleg³oœæ
	miêdzy dwoma punktami.
*/
const float Distance( const Vector3f& V1, const Vector3f& V2 )
{
	Vector3f vect = V2 - Vector3f(V1);
	return vect.Length();
}

const float DistanceSq( const Vector3f& V1, const Vector3f& V2 )
{
	Vector3f vect = V2 - V1;
	return vect.LengthSq();
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
	Vect2 = Vect2.Normalize();

	float d = Distance( V1, V2 );
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
	auto vec1 = V2 - V1;
	Vector3f origin( 0.0f, 0.0f, 1.0f );

	float angRad = atan2f( vec1.Z, vec1.X ) - atan2f( origin.Z, origin.X );

	return angRad / PIOVER180;
}

const float SwapAngle( const float &Angle )
{
	float angle = Angle;

	while( angle > 180.0f )
		angle -= 360.0f;
	while( angle < -180.0f )
		angle += 360.0f;

	return angle;
}

const float	MaxValue( const float A, const float B )
{
	return A > B ? A : B;
}

const float	MinValue( const float A, const float B )
{
	return A < B ? A : B;
}