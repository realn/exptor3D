#pragma once

#include <vector>

class CRenderViewConfig {
private:
	mutable std::vector<int>	m_Attribs;

public:
	CRenderViewConfig();
	~CRenderViewConfig();

	unsigned	ColorBits;
	unsigned	DepthBits;
	unsigned	StencilBits;

	int*	GetAttribs() const;

private:
	void	AddAttrib(int attrib) const;
	void	AddAttrib(int attrib, int value) const;
};