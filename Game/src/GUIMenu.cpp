#include "GUIMenu.h"

/*======================
	KLASA guiMenu
	Klasa reprezentuj¹ca pojedyñcze menu.
	Zarz¹dza jednoczeœnie swoimi przyciskami
======================*/
guiMenu::guiMenu()
{
	
}

guiMenu::~guiMenu()
{
	Free();
}

void guiMenu::Update( float cX, float cY, bool click )
{
	x=cX;
	y=cY;
	guiMenuItem* Item;
	for( int i = 0; i < List.size(); i++ )
	{
		Item = List[i];

		float itemL = Item->X;
		float itemR = Item->X + (float)Item->Caption.length() * ( 16.0f * 1.5f );
		float itemT = Item->Y;
		float itemB = Item->Y + 16.0f * 1.5f;

		Item->DontHighLight();
		if( cX >= itemL && cX <= itemR )
		{
			if( cY >= itemT && cY <= itemB )
			{
				Item->HighLight();
				if( click )
				{
					if( Item->Action != "" )
						Item->DoAction();

					if( Item->GoTo != -1 )
						Item->DoGoTo();
				}
			}
		}
		else Item->DontHighLight();

		Item->Update();
	}
}

void guiMenu::Render( CTextRenderer *TText, CTexture* cursor )
{
	TText->StartPrint();
	cursor->Activate();
	glDisable( GL_CULL_FACE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBegin( GL_TRIANGLE_STRIP );
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( x, y+50.0f, -0.1f );
	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( x, y, -0.1f );
	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( 50.0f+x, y+50.0f, -0.1f );
	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( 50.0f+x, y, -0.1f );
	glEnd();

	TText->EndPrint();
	TText->StartPrint();
	glTranslatef( slide, 0.0f, 0.0f );
	TText->SetColor( 1.0f, 1.0f, 1.0f );
	TText->Print( 10.0f, 20.0f, this->MainCaption, 2.5f, 2.5f );
	guiMenuItem* Item;
	for( int i = 0; i < List.size(); i++ )
	{
		Item = List[i];
		Item->Render( TText );
	}
	TText->EndPrint();
}

void guiMenu::AddMenuItem( guiMenuItem* item )
{
	List.push_back( item );
}

void guiMenu::DeleteMenuItem( unsigned int index )
{
	delete List[index];
	List.erase( List.begin() + index );
}

void guiMenu::Free()
{
	for( int i = List.size()-1; i >= 0; i-- )
	{
		DeleteMenuItem( i );
	}
}
