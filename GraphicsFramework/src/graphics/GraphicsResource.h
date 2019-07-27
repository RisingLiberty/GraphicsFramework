#pragma once

template <typename T>
class GraphicsResource
{
public:
	GraphicsResource()
	{
		++m_resource_id;
	}

	virtual ~GraphicsResource()
	{
		//--m_resource_id;
	}

	unsigned int GetResourceId() const
	{
		return this->GetCurrentResourceId();
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
};

template <typename T>
unsigned int GraphicsResource<T>::m_resource_id = 0;