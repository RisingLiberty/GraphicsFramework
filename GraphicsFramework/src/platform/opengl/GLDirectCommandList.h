#pragma once

#include "GLCommandList.h"

class GLDirectCommandList : public GLCommandList
{
public:
	GLDirectCommandList();
	virtual ~GLDirectCommandList();
	
	virtual void Execute() override;

private:
	bool m_has_executed;
};