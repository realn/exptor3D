#include "GameStaticObject.h"
#include "Log.h"

/*======================
	KLASA gameStatObj

======================*/
//std::string gameStatObj::GetStr( FILE* fp )
//{
//	char buf[512];
//	fgets( buf, 512, fp );
//	if( buf[strlen( buf )-1] == '\n' ) 
//		buf[strlen( buf )-1] = 0;
//	return buf;
//}
//
//void gameStatObj::Update( const float fTD )
//{
//	//TestCollBlock( this, GLevel.GetBlock( this->GetBlockPos() ), true );
//}
//
//void gameStatObj::Render()
//{
//	if( !Model )
//		return;
//
//	glPushMatrix();
//	glTranslatef( Pos.X, Pos.Y, Pos.Z );
//	glRotatef( Angle, 0.0f, 1.0f, 0.0f );
//
//	for( unsigned int i = 0; i < Model->GetObjCount(); i++ )
//		Model->RenderObject( i );
//
//	glPopMatrix();
//}
//
//bool gameStatObj::LoadObj( gfx::ModelManager& modelManager, const std::string& filename )
//{
//	if( filename == "" )
//	{
//		Log.Error( "STATOBJ( " + file + " ): Pusty ci¹g znaków!" );
//		return false;
//	}
//
//	FILE* fp = 0;
//	fopen_s( &fp, filename.c_str(), "rt" );
//	std::string str;
//
//	if( !fp )
//	{
//		Log.Error( "STATOBJ( " + file + " ): Nieprawid³owa œcie¿ka, lub plik nie istnieje: " + filename );
//		return false;
//	}
//
//	str = GetStr( fp );
//	Model = modelManager.Get( str );
//	if( !Model )
//	{
//		Log.Error( "STATOBJ( " + file + " ): Nieudane wczytanie modelu: " + str );
//		return false;
//	}
//
//	str = GetStr( fp );
//	Radius = (float)atof( GetStr( fp ).c_str() );
//
//	fclose( fp );
//	return true;
//}
