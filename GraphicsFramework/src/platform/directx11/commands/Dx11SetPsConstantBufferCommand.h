#pragma once

#include "graphics/commands/Command.h"

class Dx11SetPsConstantBufferCommand : public Command
{
public:
	Dx11SetPsConstantBufferCommand(const unsigned int reg, ID3D11Buffer* const buffer);
	virtual ~Dx11SetPsConstantBufferCommand();

	std::string ToString() const override;
	void Execute() override;

private:
	const unsigned int m_register;
	ID3D11Buffer* const m_buffer;
};