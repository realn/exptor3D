#include <CBGL/COpenGL.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureText.h"

/*========================
	KLASA CTextRenderer
	Jest odpowiedzialna za rysowanie tekstów na ekranie.
=========================*/
CTextRenderer::CTextRenderer( gfx::TextureRepository& texManager ) :
	base( 0 ),
	Color( 1.0f, 1.0f, 1.0f, 1.0f )
{
	Tex = texManager.Get( "Font.tga" );
	base = glGenLists(256);	// Creating 256 display lists

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
}

CTextRenderer::~CTextRenderer()
{
	if( base != 0 )
	{
		glDeleteLists( base, 256 );
		base = 0;
	}
}

void	CTextRenderer::StartPrint( const float width, const float height )
{
	glPushAttrib( GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
	glPushMatrix();

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glDisable( GL_LIGHTING );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Tex->Activate();

	{
		glMatrixMode(GL_PROJECTION);
		auto mat = glm::ortho(0.0f, width, height, 0.0f);
		glLoadMatrixf(glm::value_ptr(mat));
		glMatrixMode(GL_MODELVIEW);
	}

	glLoadIdentity();

	glListBase( base - 32 );
}

void	CTextRenderer::Print( const float x, const float y, const std::string& text, const float ScaleX, const float ScaleY )
{
	char stext[255];
	strcpy_s( stext, 255 * sizeof(char), text.c_str() );
	glPushMatrix();
	
	glTranslatef( x, y, 0 );
	glScalef( ScaleX, ScaleY, 1.0f );
	glColor4f( Color.x, Color.y, Color.z, Color.w );

	glCallLists( static_cast<GLsizei>(strlen(stext)), GL_UNSIGNED_BYTE, stext );
	glPopMatrix();
}

void	CTextRenderer::Print( const glm::vec2& pos, const std::string& text, const glm::vec2& scale )
{
	Print( pos.x, pos.y, text, scale.x, scale.y );
}

void	CTextRenderer::EndPrint()
{
	glPopMatrix();
	glPopAttrib();

	glColor3f( 1.0f, 1.0f ,1.0f );
}

void	CTextRenderer::SetColor( float R, float G, float B, float Alpha )
{
	Color = glm::vec4( R, G, B, Alpha );
}

void	CTextRenderer::SetColor( const glm::vec3& color )
{
	Color = glm::vec4( color.x, color.y, color.z, 1.0f );
}

void	CTextRenderer::SetColor( const glm::vec4& color )
{
	Color = glm::vec4( color.x, color.y, color.z, color.w );
}

const glm::vec2	CTextRenderer::GetTextSize( const std::string& text ) const
{
	return glm::vec2( (float)text.size() * 14.0f, 16.0f );
}