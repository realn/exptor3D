#pragma once

class IEntity
{
public:
	virtual ~IEntity(){};

	virtual void	Update( const float fTD ) = 0;
	virtual void	Render() = 0;
};