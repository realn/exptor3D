#include "stdafx.h"
#include "EditionBlock.h"

CEditionBlock::CEditionBlock() {
}

CEditionBlock::CEditionBlock( const glm::vec3& position, const glm::vec3& size ) :
	m_Position( position )
{
	const glm::vec3 halfSize = size / 2.0f;

	m_Verts[0] = position + halfSize * glm::vec3( -1.0f, -1.0f, 1.0f );
	m_Verts[1] = position + halfSize * glm::vec3( -1.0f, -1.0f, -1.0f );
	m_Verts[2] = position + halfSize * glm::vec3( 1.0f, -1.0f, -1.0f );
	m_Verts[3] = position + halfSize * glm::vec3( 1.0f, -1.0f, 1.0f );

	m_Verts[4] = position + halfSize * glm::vec3( -1.0f, 1.0f, -1.0f );
	m_Verts[5] = position + halfSize * glm::vec3( -1.0f, 1.0f, 1.0f );
	m_Verts[6] = position + halfSize * glm::vec3( 1.0f, 1.0f, 1.0f );
	m_Verts[7] = position + halfSize * glm::vec3( 1.0f, 1.0f, -1.0f );

	this->GetSurfaceMod( BLOCK_SURFACE::SIDE_LEFT	).Set( m_Verts[4], m_Verts[5], m_Verts[0], m_Verts[1] );
	this->GetSurfaceMod( BLOCK_SURFACE::SIDE_FAR	).Set( m_Verts[1], m_Verts[4], m_Verts[7], m_Verts[2] );
	this->GetSurfaceMod( BLOCK_SURFACE::SIDE_RIGHT	).Set( m_Verts[2], m_Verts[7], m_Verts[6], m_Verts[3] );
	this->GetSurfaceMod( BLOCK_SURFACE::SIDE_NEAR	).Set( m_Verts[3], m_Verts[6], m_Verts[5], m_Verts[0] );
	this->GetSurfaceMod( BLOCK_SURFACE::TOP			).Set( m_Verts[4], m_Verts[5], m_Verts[6], m_Verts[7] );
	this->GetSurfaceMod( BLOCK_SURFACE::BOTTOM		).Set( m_Verts[0], m_Verts[1], m_Verts[2], m_Verts[3] );

	for( unsigned i = 0; i < 4; i++ ) {
		this->m_SideBlock[i] = nullptr;
	}
}

const CSurface&	CEditionBlock::GetSurface( const BLOCK_SURFACE surface ) const {
	return this->m_Surface[(unsigned)surface];
}

const CSurface&	CEditionBlock::GetOppositeSurface( const BLOCK_SURFACE surface ) const {
	switch (surface)
	{
	case BLOCK_SURFACE::SIDE_LEFT:	return this->GetSurface( BLOCK_SURFACE::SIDE_RIGHT );
	case BLOCK_SURFACE::SIDE_FAR:	return this->GetSurface( BLOCK_SURFACE::SIDE_NEAR );
	case BLOCK_SURFACE::SIDE_RIGHT:	return this->GetSurface( BLOCK_SURFACE::SIDE_LEFT );
	case BLOCK_SURFACE::SIDE_NEAR:	return this->GetSurface( BLOCK_SURFACE::SIDE_FAR );
	case BLOCK_SURFACE::BOTTOM:		return this->GetSurface( BLOCK_SURFACE::TOP );
	case BLOCK_SURFACE::TOP:		return this->GetSurface( BLOCK_SURFACE::BOTTOM );
	default:
		return this->m_Surface[0];
	}
}

CEditionBlock* const	CEditionBlock::GetSideBlock( const BLOCK_SURFACE surface ) const {
	unsigned side = (unsigned)surface % 4;
	return this->m_SideBlock[side];
}

CEditionBlock* const	CEditionBlock::GetOppositeBlock( const BLOCK_SURFACE surface ) const {
	unsigned side = ((unsigned)surface + 2) % 4;
	return this->m_SideBlock[side];
}

const bool	CEditionBlock::FindSurface( const CSurface& surface, BLOCK_SURFACE& outIndex ) const {
	for( unsigned i = 0; i < 6; i++ ) {
		if( this->m_Surface[i].IsEqual( surface, false ) ) {
			outIndex = (BLOCK_SURFACE)i;
			return true;
		}
	}
	return false;
}

void	CEditionBlock::SetSideBlock( const BLOCK_SURFACE surface, CEditionBlock* const block ) {
	unsigned side = (unsigned)surface % 4;
	this->m_SideBlock[side] = block;
}

void	CEditionBlock::ClearSideBlocks() {
	for( unsigned i = 0; i < 4; i++ ) {
		this->m_SideBlock[i] = nullptr;
	}
}

CSurface&	CEditionBlock::GetSurfaceMod( const BLOCK_SURFACE surface ){
	return this->m_Surface[(unsigned)surface];
}
