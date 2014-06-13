#pragma once

#include <Surface.h>
#include <Dictionary.h>

class CEditionBlock;

class CSurfaceContext {
private:
	std::vector<CEditionBlock*>	m_Blocks;

public:
	CSurfaceContext();
	~CSurfaceContext();

	void	SetEdition( const std::vector<glm::vec3>& blocks );
	void	Clear();
	const std::vector<glm::vec3>	GetLocations() const;
	const CDictionary<CSurface, CEditionBlock*>	GetOuterSurfaces() const;

private:
	void	AddBlock( const glm::vec3& position );
	void	ConnectBlocks();
	void	ConnectBlock( CEditionBlock* const pBlock );
};