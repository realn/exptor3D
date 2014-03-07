#pragma once

class IUpdateable
{
protected:
	bool DeleteThis;

public:
	IUpdateable() : DeleteThis( false ){}
	virtual ~IUpdateable(){}

	virtual void	Update( const float fTD ) = 0;

	virtual const bool	IsActive() const { return true; }
	virtual const bool	CanDelete() const { return DeleteThis; }

	virtual void	OnDelete(){}
};