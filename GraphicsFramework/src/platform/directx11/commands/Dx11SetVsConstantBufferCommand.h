#pragma once

#include "graphics/commands/Command.h"

class Dx11SetVsConstantBufferCommand : public Command
{
public:
	Dx11SetVsConstantBufferCommand(unsigned int reg, ID3D11Buffer* const buffer);
	virtual ~Dx11SetVsConstantBufferCommand();

	std::string ToString() const override;
	void Execute() override;

private:
	const unsigned int m_register;
	ID3D11Buffer* const m_buffer;
};