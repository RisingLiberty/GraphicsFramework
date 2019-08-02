#pragma once

template <typename T>
class GraphicsResource
{
public:
	GraphicsResource()
	{
		++m_resource_id;
		++m_count;
	}

	virtual ~GraphicsResource()
	{
		//--m_resource_id;
		--m_count;
	}

	unsigned int GetResourceId() const
	{
		return this->GetCurrentResourceId();
	}

	unsigned int GetCount() const
	{
		return m_count;
	}

	static unsigned int GetCurrentResourceId()
	{
		return m_resource_id;
	}

	static unsigned int GetNextResourceId()
	{
		return m_resource_id;
	}

protected:
	static unsigned int m_resource_id;
	static unsigned int m_count;
};

template <typename T>
unsigned int GraphicsResource<T>::m_resource_id = 0;

template <typename T>
unsigned int GraphicsResource<T>::m_count = 0;