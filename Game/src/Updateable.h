#pragma once

class IUpdateable
{
public:
	virtual ~IUpdateable(){}

	virtual void	Update( const float fTD ) = 0;

	virtual const bool	IsActive() const { return true; }
};