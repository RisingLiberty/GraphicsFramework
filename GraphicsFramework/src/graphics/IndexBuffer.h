#pragma once

#include "BufferUsage.h"

class IndexBuffer
{
public:
	IndexBuffer(size_t count, BufferUsage usage);
	virtual ~IndexBuffer();

	virtual void SetData(const void* data) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	size_t GetCount() const;

	static std::unique_ptr<IndexBuffer> Create(size_t count, BufferUsage usage = BufferUsage::STATIC, void* data = nullptr);

protected:
	size_t m_count;
	BufferUsage m_usage;
};