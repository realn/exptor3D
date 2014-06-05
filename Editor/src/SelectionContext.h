#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "WireCube.h"

class CSelectionContext {
private:
	CWireCube	m_Cube;
	std::vector<glm::vec3>	m_Selection;

public:
	CSelectionContext();
	
	void	AddSelection( const std::vector<glm::vec3>& selection );
	void	SetSelection( const std::vector<glm::vec3>& selection );
	const std::vector<glm::vec3>&	GetSelection() const;
	void	Clear();

	void	Render( const glm::mat4& proj, const glm::mat4& modelview );
};