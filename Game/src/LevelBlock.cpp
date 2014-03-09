#include "Level.h"

CLvlBlock::CLvlBlock() :
	walls( 0 ),
	CornerCount( 0 ),
	Row( 0 ),
	Col( 0 )
{

}

void	CLvlBlock::Set( const unsigned row, const unsigned col, const float width, const float height, const float depth )
{
	Row = row;
	Col = col;
	Origin.X = (float)col * width + ( width / 2.0f );
	Origin.Y = 0.0f;//height / 2.0f;
	Origin.Z = (float)row * depth + ( depth / 2.0f );
}

void CLvlBlock::LoadWalls( const std::string& str )
{
	walls = (unsigned)LEV_SURFACE::CEILING | (unsigned)LEV_SURFACE::FLOOR;

	if( ContainsString( str, "L" ) )
		walls |= (unsigned)LEV_SURFACE::WALL_LEFT;
	if( ContainsString( str, "R" ) )
		walls |= (unsigned)LEV_SURFACE::WALL_RIGHT;
	if( ContainsString( str, "F" ) )
		walls |= (unsigned)LEV_SURFACE::WALL_FAR;
	if( ContainsString( str, "N" ) )
		walls |= (unsigned)LEV_SURFACE::WALL_NEAR;
}
