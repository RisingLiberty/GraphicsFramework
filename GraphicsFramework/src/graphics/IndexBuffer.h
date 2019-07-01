#pragma once

class IndexBuffer
{
public:
	IndexBuffer(size_t count);
	virtual ~IndexBuffer();

	virtual void SetData(void* data) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	size_t GetCount() const;

	static std::unique_ptr<IndexBuffer> Create(size_t count);

protected:
	size_t m_count;
};