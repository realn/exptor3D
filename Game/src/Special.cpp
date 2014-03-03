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

specManager SEManager;
ioTexture	Part;

specEffect::specEffect()
{
	Visible = true;
	CanDelete = false;
}

void specEffect::Update( const float fTD )
{

}

void specEffect::Render()
{

}

void specRay::Create( Vector3f Pos, Vector3f Veloc )
{
	Alpha = 1.0f;

	FromPos = Pos;
	ToPos = RayCast( Pos, Veloc, 1.0f, &GLevel );
}

void specRay::Update( const float fTD )
{
	if( Alpha > 0.0f )
		Alpha -= 0.02f * GUI.GetSpeed() * fTD;
	else CanDelete = true;
}

void specRay::Render()
{
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glBegin( GL_QUADS );
	glColor4f( 1.0f, 0.7f, 0.0f, Alpha * 0.5f );
	glVertex3f( FromPos.X, FromPos.Y + 0.1f, FromPos.Z );
	glVertex3f( FromPos.X, FromPos.Y - 0.1f, FromPos.Z );
	glColor4f( 1.0f, 0.8f, 0.0f, Alpha );
	glVertex3f( ToPos.X, -0.1f, ToPos.Z );
	glVertex3f( ToPos.X, 0.1f, ToPos.Z );
	glEnd();
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
}

void specExplode::Create( Vector3f Pos, float Power, float fStep )
{
	Step = fStep;
	toPower = Power;
	FromPos = Pos;
	thisPower = 0.0f;
}

void specExplode::Update( const float fTD )
{
	if( thisPower < toPower )
	{
		thisPower += Step * GUI.GetSpeed() * fTD;

		Alpha = 1.0f - ( thisPower * ( 1.0f / toPower ) );
	}
	else CanDelete = true;
}

void specExplode::Render()
{
	glPushMatrix();
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	glTranslatef( FromPos.X, 0.0f, FromPos.Z );
	GLUquadric* obj = gluNewQuadric();

	glColor4f( 1.0f, 0.0f, 0.0f, Alpha );
	gluSphere( obj, thisPower/2.0f, 16, 16 );

	glColor4f( 1.0f, 1.0f, 0.2f, Alpha );
	gluSphere( obj, thisPower, 16, 16 );

	gluDeleteQuadric( obj );

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	glPopMatrix();
}

void specSprite::Create( Vector3f Pos, float R, float G, float B )
{
	C[0] = R;
	C[1] = G;
	C[2] = B;
	AtPos = Pos;
	Alpha = 1.0f;
	CanDelete = false;
}

void specSprite::Update( const float fTD )
{
	Alpha -= 0.01f * GUI.GetSpeed() * fTD;
	if( Alpha <= 0.0f )
		CanDelete = true;
}

void specSprite::Render()
{
	if( !GUI.GetCanSmoke() )
		return;

	glPushMatrix();
	glDisable( GL_CULL_FACE );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable( GL_BLEND );

	glTranslatef( AtPos.X, AtPos.Y, AtPos.Z );
	glRotatef( -(GUI.PInfo.angle-180.0f), 0.0f, 1.0f, 0.0f );

	Part.Activate( GUI.GetTexDLevel() );
	glBegin( GL_TRIANGLE_STRIP );
	glColor4f( C[0], C[1], C[2], Alpha );
	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( -0.8f, 0.8f, 0.0f );
	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( -0.8f, -0.8f, 0.0f );
	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( 0.8f, 0.8f, 0.0f );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( 0.8f, -0.8f, 0.0f );
	glEnd();

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glPopMatrix();
}

void specManager::AddEffect( specEffect* effect )
{
	if( GUI.GetMaxSpecial() > 0 )
		if( List.size() >= GUI.GetMaxSpecial() )
			return;

	List.push_back( effect );
}

void specManager::DeleteEffect( unsigned int index )
{
	delete List[index];
	List.erase( List.begin() + index );
}

specEffect* specManager::GetEffect( unsigned int index )
{
	return List[index];
}

void specManager::Update( const float fTD )
{
	specEffect* effect;
	for( int i = List.size()-1; i >= 0; i-- )
	{
		effect = List[i];
		effect->Update( fTD );
		if( effect->CanDelete )
		{
			DeleteEffect( i );
		}
	}
}

void specManager::Render()
{
	unsigned int i;
	specEffect* effect;
	for( i = 0; i < List.size(); i++ )
	{
		effect = List[i];

		if( !effect->Visible )
			continue;

		effect->Render();
	}
	if( GUI.GetReflection() )
	{
		glPushMatrix();
		glTranslatef( 0.0f, -10.0f, 0.0f );
		glScalef( 1.0f, -1.0f, 1.0f );
		glDisable( GL_CLIP_PLANE0 );
		glEnable( GL_CLIP_PLANE1 );
		glFrontFace( GL_CW );
		for( i = 0; i < List.size(); i++ )
		{
			effect = List[i];

			if( !effect->Visible )
				continue;

			effect->Render();
		}
		glFrontFace( GL_CCW );
		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}
}


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
	ZeroMemory(data,((512 * 512)* 4 * sizeof(unsigned int)));
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
	if( tick >= GUI.GetMBKeyFrames() )
	{
		CanCopy = true;
		tick = 0;
	}
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