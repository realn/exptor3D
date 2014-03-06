#include "Level.h"

CLvlBlock::CLvlBlock() :
	walls( 0 ),
	CornerCount( 0 )
{

}

void CLvlBlock::LoadWalls( const std::string& str )
{
	walls = 0;
	if( ContainsString( str, "L" ) )
		walls |= LEV_WALL_LEFT;
	if( ContainsString( str, "R" ) )
		walls |= LEV_WALL_RIGHT;
	if( ContainsString( str, "F" ) )
		walls |= LEV_WALL_TOP;
	if( ContainsString( str, "N" ) )
		walls |= LEV_WALL_BOTTOM;
}