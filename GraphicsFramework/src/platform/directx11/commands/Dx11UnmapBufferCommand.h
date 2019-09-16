#pragma once

#include "graphics/commands/UnmapBufferCommand.h"

struct Dx11ShaderCBuffer;
class Dx11BufferWrapper;

class Dx11UnmapBufferCommand : public UnmapBufferCommand
{
public:
	Dx11UnmapBufferCommand(const ApiBufferWrapper* buffer, const UINT subResource = 0);
	Dx11UnmapBufferCommand(const Dx11ShaderCBuffer* buffer, const UINT subresource = 0);
	virtual ~Dx11UnmapBufferCommand();

	void Execute() override;

private:
	const Dx11ShaderCBuffer* m_constant_buffer;
	const UINT m_subresource;
};