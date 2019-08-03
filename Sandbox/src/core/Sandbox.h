#pragma once

#include <platform/win64/Win64Application.h>

class Sandbox : public Win64Application
{
public:
	Sandbox();
	virtual ~Sandbox();

	virtual void Initialize() override;

private:

};