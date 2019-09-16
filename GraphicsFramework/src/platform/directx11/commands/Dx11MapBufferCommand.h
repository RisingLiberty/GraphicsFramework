#pragma once

#include "graphics/commands/MapBufferCommand.h"

struct Dx11ShaderCBuffer;

class Dx11MapBufferCommand : public MapBufferCommand
{
public:
	Dx11MapBufferCommand(const ApiBufferWrapper* buffer, D3D11_MAP mapType, D3D11_MAPPED_SUBRESOURCE* mappedSubresource, const UINT mapFlags = 0, const UINT subResource = 0);
	Dx11MapBufferCommand(const Dx11ShaderCBuffer* buffer, D3D11_MAP mapType, D3D11_MAPPED_SUBRESOURCE* mappedSubresource, const UINT mapFlags = 0, const UINT subResource = 0);
	virtual ~Dx11MapBufferCommand();

	void Execute() override;

private:
	const Dx11ShaderCBuffer* m_constant_buffer;
	D3D11_MAP m_map_type;
	D3D11_MAPPED_SUBRESOURCE* m_mapped_subresource;
	const UINT m_map_flags;
	const UINT m_subresource;
};
