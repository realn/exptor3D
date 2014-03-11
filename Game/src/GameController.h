#pragma once

#include <vector>

class IController
{
public:
	virtual ~IController(){};

	virtual void	Update() = 0;
};

class CControllerList
{
private:
	std::vector<IController*>	List;

public:
	CControllerList();
	virtual ~CControllerList();

	void	AddController(IController* pController);
	void	RemoveController(IController* pController);

	void	Clear();

	void	Update();
};