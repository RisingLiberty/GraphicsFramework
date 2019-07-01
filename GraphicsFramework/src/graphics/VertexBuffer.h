#pragma once

class VertexBuffer
{
public:
	enum class BufferUsage
	{
		DYNAMIC,
		STATIC
	};

	VertexBuffer(size_t size, BufferUsage usage);
	virtual ~VertexBuffer();

	virtual void SetData(void* data) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	static std::unique_ptr<VertexBuffer> Create(size_t size, BufferUsage usage = BufferUsage::STATIC);

protected:
	size_t m_size;
	BufferUsage m_usage;

};