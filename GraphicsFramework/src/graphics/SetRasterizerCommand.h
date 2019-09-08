#pragma once

#include "Command.h"
#include "RasterizerSettings.h"

class SetRasterizerCommand : public Command
{
public:
	SetRasterizerCommand(const RasterizerSettings& settings);
	virtual ~SetRasterizerCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const;

protected:
	const RasterizerSettings m_settings;
};