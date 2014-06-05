#pragma once

#include <glm/glm.hpp>
#include <Surface.h>

enum class BLOCK_SURFACE {
	SIDE_LEFT	= 0,
	SIDE_FAR	= 1,
	SIDE_RIGHT	= 2,
	SIDE_NEAR	= 3,
	TOP			= 4,
	BOTTOM		= 5,
};

class CEditionBlock {
private:
	glm::vec3		m_Position;
	glm::vec3		m_Verts[8];
	CSurface		m_Surface[6];
	CEditionBlock*	m_SideBlock[4];

public:
	CEditionBlock();
	CEditionBlock( const glm::vec3& position, const glm::vec3& size );

	const glm::vec3&		GetPosition() const { return this->m_Position; }
	const glm::vec3&		GetVert( const unsigned vert ) const { return this->m_Verts[vert]; }
	const CSurface&			GetSurface( const BLOCK_SURFACE surface ) const;
	const CSurface&			GetOppositeSurface( const BLOCK_SURFACE surface ) const;
	CEditionBlock* const	GetSideBlock( const BLOCK_SURFACE surface ) const;
	CEditionBlock* const	GetOppositeBlock( const BLOCK_SURFACE surface ) const;
	const bool				FindSurface( const CSurface& surface, BLOCK_SURFACE& outIndex ) const;
	 
	void	SetSideBlock( const BLOCK_SURFACE surface, CEditionBlock* const block );
	void	ClearSideBlocks();

private:
	CSurface&	GetSurfaceMod( const BLOCK_SURFACE surface );
};

