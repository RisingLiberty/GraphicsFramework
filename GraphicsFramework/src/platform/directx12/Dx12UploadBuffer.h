#pragma once

#include "Dx12ShaderCBuffer.h"

class Dx12UploadBuffer
{
public:
	Dx12UploadBuffer(const Dx12ShaderCBuffer& constantBuffer);
	~Dx12UploadBuffer();

	void CopyData(const void* data);
	
	ID3D12Resource* GetResource() const;
	unsigned int GetSize() const;

private:
	ComPtr<ID3D12Resource> m_upload_buffer;
	char* m_mapped_data;
	unsigned int m_size;
};