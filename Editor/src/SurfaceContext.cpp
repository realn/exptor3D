#include "stdafx.h"
#include "SurfaceContext.h"
#include "EditionBlock.h"

CSurfaceContext::CSurfaceContext() {
}

CSurfaceContext::~CSurfaceContext() {
	this->Clear();
}

void	CSurfaceContext::SetEdition( const std::vector<glm::vec3>& blocks ) {
	this->Clear();

	for( auto it = blocks.cbegin(); it != blocks.cend(); it++ ) {
		this->AddBlock( *it );
	}

	this->ConnectBlocks();
}

void	CSurfaceContext::Clear() {
	for( auto it = m_Blocks.begin(); it != m_Blocks.end(); it++ ) {
		delete (*it);
	}
	m_Blocks.clear();
}

void	CSurfaceContext::AddBlock( const glm::vec3& position ) {
	CEditionBlock* pBlock = new CEditionBlock( position, glm::vec3( 1.0f ) );

	this->m_Blocks.push_back( pBlock );
}

void	CSurfaceContext::ConnectBlocks() {
	for( auto it = m_Blocks.begin(); it != m_Blocks.end(); it++ ) {
		(*it)->ClearSideBlocks();
	}

	for( auto it = m_Blocks.begin(); it != m_Blocks.end(); it++ ) {
		this->ConnectBlock( (*it) );
	}
}

void	CSurfaceContext::ConnectBlock( CEditionBlock* const pBlock ) {
	for( unsigned i = 0; i < 4; i++ ) {
		const BLOCK_SURFACE surfaceId = (BLOCK_SURFACE)i;
		if( pBlock->GetSideBlock( surfaceId ) != nullptr )
			continue;

		const CSurface& surface = pBlock->GetSurface( surfaceId );

		for( auto it = m_Blocks.begin(); it != m_Blocks.end(); it++ ) {
			if( *it == pBlock )
				continue;

			BLOCK_SURFACE outSurfaceId;
			if( (*it)->FindSurface( surface, outSurfaceId ) ) {
				pBlock->SetSideBlock( surfaceId, pBlock );
				(*it)->SetSideBlock( outSurfaceId, pBlock );
			}
		}
	}
}