#include "Effect.h"
//
//CEffectRay::CEffectRay( const glm::vec3& pos, const glm::vec3& dest ) :
//	Alpha( 1.0f ),
//	FromPos( pos ),
//	ToPos( dest )
//{
//}
//
//void CEffectRay::Update( const float fTD )
//{
//	if( Alpha > 0.0f )
//		Alpha -= 0.02f * GUI.GetSpeed() * fTD;
//	else CanDelete = true;
//}
//
//void CEffectRay::Render()
//{
//	glDisable( GL_TEXTURE_2D );
//	glDisable( GL_CULL_FACE );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	glEnable( GL_BLEND );
//
//	glBegin( GL_QUADS );
//
//	glColor4f( 1.0f, 0.7f, 0.0f, Alpha * 0.5f );
//	glVertex3f( FromPos.X, FromPos.Y + 0.1f, FromPos.Z );
//	glVertex3f( FromPos.X, FromPos.Y - 0.1f, FromPos.Z );
//
//	glColor4f( 1.0f, 0.8f, 0.0f, Alpha );
//	glVertex3f( ToPos.X, -0.1f, ToPos.Z );
//	glVertex3f( ToPos.X, 0.1f, ToPos.Z );
//
//	glEnd();
//
//	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
//	glDisable( GL_BLEND );
//	glEnable( GL_CULL_FACE );
//	glEnable( GL_TEXTURE_2D );
//}
//
//const glm::vec3	CEffectRay::GetFromPos() const
//{
//	return FromPos;
//}
//
//const glm::vec3	CEffectRay::GetToPos() const
//{
//	return ToPos;
//}