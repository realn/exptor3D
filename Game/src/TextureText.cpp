#include "TextureText.h"

/*========================
	KLASA guiTextureText
	Jest odpowiedzialna za rysowanie tekstów na ekranie.
=========================*/
guiTextureText::guiTextureText()
{
	loaded = false;
	Tex = NULL;
	Width = 800;
	Height = 600;
	C[0] = 1.0f;
	C[1] = 1.0f;
	C[2] = 1.0f;
}

guiTextureText::~guiTextureText()
{
	Free();
}

void guiTextureText::Init( CTexture* font )
{
	if( !font )
		return;
	
	Tex = font;

	base = glGenLists(256);	// Creating 256 display lists
	
	Tex->Activate();

	for (int loop1=0; loop1 < 256; loop1++)			// Loop through all 256 lists
	{
		float cx = float( loop1%16 ) / 16.0f;		// X position of current character
		float cy = float( loop1/16 ) / 16.0f;		// Y position of current character

		glNewList(base+loop1,GL_COMPILE);	// Start building a list
			glBegin(GL_QUADS);					// Use a quad for each character
				glTexCoord2f( cx, 1.0f - cy - 0.0625f );		// Texture coord (Bottom left)
				glVertex2d( 0, 16 );				// Vertex coord (Bottom left)
				glTexCoord2f( cx + 0.0625f, 1.0f - cy - 0.0625f );	// Texture coord (Bottom right)
				glVertex2i( 16, 16);				// Vertex coord (Bottom right)
				glTexCoord2f( cx + 0.0625f, 1.0f - cy - 0.001f );	// Texture coord (Top right)
				glVertex2i( 16, 0 );				// Vertex coord (Top right)
				glTexCoord2f( cx, 1.0f - cy - 0.001f );		// Texture coord (Top left)
				glVertex2i( 0, 0 );				// Vertex coord (Top left)
			glEnd();			// Done building our quad (Character)
			glTranslated( 14, 0, 0);		// Move to the right of the character
		glEndList();				// Done building the display list
	}	

	loaded = true;
}

void guiTextureText::Free()
{
	if( !loaded )
		return;

	glDeleteLists( base, 256 );
	glDeleteLists( list, 2 );
	loaded = false;
}

void guiTextureText::StartPrint()
{
	glLoadIdentity();

	glListBase( base - 32 );

	glPushAttrib( GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
	glPushMatrix();

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glDisable( GL_LIGHTING );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Tex->Activate();

	glMatrixMode(GL_PROJECTION);				
	glLoadIdentity();		
	glOrtho( 0, (double)Width, (double)Height, 0, -1.0, 1.0 );
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}

void guiTextureText::Print( float x, float y, std::string text, float ScaleX, float ScaleY )
{
	char stext[255];
	strcpy_s( stext, 255 * sizeof(char), text.c_str() );
	glPushMatrix();
	glTranslatef( x, y, 0 );
	glScalef( ScaleX, ScaleY, 1.0f );
	glColor4f( C[0], C[1], C[2], C[3] );
	glPushMatrix();
	glCallLists( strlen(stext), GL_UNSIGNED_BYTE, stext );
	glPopMatrix();
	glCallLists( strlen(stext), GL_UNSIGNED_BYTE, stext );
	glPopMatrix();
}

void guiTextureText::EndPrint()
{
	glPopMatrix();
	glPopAttrib();

	glColor3f( 1.0f, 1.0f ,1.0f );
}

void guiTextureText::SetSize( unsigned int wid, unsigned int hei )
{
	Width = wid;
	Height = hei;
}

void guiTextureText::SetColor( float R, float G, float B, float Alpha )
{
	C[0] = R;
	C[1] = G;
	C[2] = B;
	C[3] = Alpha;
}
