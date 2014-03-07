#pragma once

class ISceneEntity
{
protected:
	bool DeleteThis;

public:
	ISceneEntity() : DeleteThis( false ){}
	virtual ~ISceneEntity(){}

	virtual const bool	CanDelete() const { return DeleteThis; }

	virtual void	OnDelete(){}
};