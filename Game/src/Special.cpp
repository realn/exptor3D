/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Special.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Special.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Special.h"

//CSpecialEffectManager SEManager;
//
//CSpecialEffectManager::CSpecialEffectManager() :
//	TexManager( nullptr )
//{
//}
//
//CSpecialEffectManager::~CSpecialEffectManager()
//{
//
//}
//
//void	CSpecialEffectManager::Init( CTexManager& texManager )
//{
//	TexManager = &texManager;
//}
//
//CTexManager&	CSpecialEffectManager::GetTexMng()
//{
//	return *TexManager;
//}
//
//void CSpecialEffectManager::AddEffect( CEffect* effect )
//{
//	if( GUI.GetMaxSpecial() > 0 )
//		if( List.size() >= GUI.GetMaxSpecial() )
//			return;
//
//	List.push_back( effect );
//}
//
//void CSpecialEffectManager::DeleteEffect( unsigned int index )
//{
//	delete List[index];
//	List.erase( List.begin() + index );
//}
//
//CEffect* CSpecialEffectManager::GetEffect( unsigned int index )
//{
//	return List[index];
//}
//
//void CSpecialEffectManager::Update( const float fTD )
//{
//	CEffect* effect;
//	for( int i = List.size()-1; i >= 0; i-- )
//	{
//		effect = List[i];
//		effect->Update( fTD );
//		if( effect->CanDelete )
//		{
//			DeleteEffect( i );
//		}
//	}
//}
//
//void CSpecialEffectManager::Render()
//{
//	unsigned int i;
//	CEffect* effect;
//	for( i = 0; i < List.size(); i++ )
//	{
//		effect = List[i];
//
//		if( !effect->Visible )
//			continue;
//
//		effect->Render();
//	}
//	if( GUI.GetReflection() )
//	{
//		glPushMatrix();
//		glTranslatef( 0.0f, -10.0f, 0.0f );
//		glScalef( 1.0f, -1.0f, 1.0f );
//		glDisable( GL_CLIP_PLANE0 );
//		glEnable( GL_CLIP_PLANE1 );
//		glFrontFace( GL_CW );
//		for( i = 0; i < List.size(); i++ )
//		{
//			effect = List[i];
//
//			if( !effect->Visible )
//				continue;
//
//			effect->Render();
//		}
//		glFrontFace( GL_CCW );
//		glDisable( GL_CLIP_PLANE1 );
//		glEnable( GL_CLIP_PLANE0 );
//		glPopMatrix();
//	}
//}


specMotionBlur::specMotionBlur()
{
	CurTex = 0;
	CanCopy = false;
	inited = false;
	tick = 0;
}

specMotionBlur::~specMotionBlur()
{
	Free();
}

void specMotionBlur::Init()
{
	unsigned int* data; // Stored Data
	data = (unsigned int*)new GLuint[((512 * 512)* 4 * sizeof(unsigned int))];
	std::memset(data, 0,((512 * 512)* 4 * sizeof(unsigned int)));
	glGenTextures(5, &texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	delete [] data;
	
	list = glGenLists(2);
	glNewList( list, GL_COMPILE );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( -1, 1, -1, 1, -1, 1 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEndList();
	glNewList( list+1, GL_COMPILE );
		glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -1.0f, 1.0f, 0.0f );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -1.0f, -1.0f, 0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 1.0f, 0.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, -1.0f, 0.0f );
		glEnd();
	glEndList();
	
}

void specMotionBlur::CopyImage()
{
	if( !CanCopy )
		return;

	CurTex = ( CurTex + 1 ) % 5;
	glBindTexture( GL_TEXTURE_2D, texture[CurTex] );
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 512, 512, 0);
	
	CanCopy = false;
}

void specMotionBlur::Update( const float fTD )
{
	tick++;
	//if( tick >= GUI.GetMBKeyFrames() )
	//{
	//	CanCopy = true;
	//	tick = 0;
	//}
}

void specMotionBlur::Render()
{
	glCallList( list );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	
	
	for( int i = 0; i < 5; i++ )
	{
		int tex = ( CurTex + i ) % 5;
		glBindTexture( GL_TEXTURE_2D, texture[tex] );
		glColor4f( 1.0f, 1.0f, 1.0f, (float)i/10.0f );
		
		glCallList( list+1 );
	}

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );

	glPopMatrix();
	
}

void specMotionBlur::Free()
{
	glDeleteTextures( 5, &texture[0] );
}

specMotionBlur SMBlur;