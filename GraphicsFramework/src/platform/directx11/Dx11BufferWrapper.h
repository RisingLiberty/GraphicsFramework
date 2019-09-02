#pragma once

#include "graphics/ApiBufferWrapper.h"

class Dx11BufferWrapper : public ApiBufferWrapper
{
public:
	Dx11BufferWrapper(unsigned int size, BufferUsage usage, BufferAccess access, BufferType type, const void* data);
	virtual ~Dx11BufferWrapper();

	ID3D11Buffer* GetBuffer() const;

protected:
	void SetDataInternal(const void* data, unsigned int size) override;

	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer(unsigned int size) const;

private:
	ComPtr<ID3D11Buffer> m_buffer;
};