#pragma once

#include "Dx11CommandList.h"

class Dx11DirectCommandList : public Dx11CommandList
{
public:
	Dx11DirectCommandList();
	virtual ~Dx11DirectCommandList();

	void Execute() override;

private:
	bool m_has_executed;
};