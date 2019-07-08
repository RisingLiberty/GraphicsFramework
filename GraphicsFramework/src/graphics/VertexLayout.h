#pragma once

#include "VertexAttribute.h"

class VertexLayout
{
public:
	VertexLayout();
	virtual ~VertexLayout();

	template <typename T>
	void Push(VertexAttributeType type, unsigned int count = 1)
	{
		ASSERT(false, "vertex attribute not recognised");
	}

	template <>
	void Push<float>(VertexAttributeType type, unsigned int count)
	{
		m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::FLOAT, false, count));
		m_size += count * m_attributes.back().GetSize();
	}

	template <>
	void Push<unsigned int>(VertexAttributeType type, unsigned int count)
	{
		m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::UNSIGNED_INT, false, count));
		m_size += count * m_attributes.back().GetSize();
	}

	template <>
	void Push<int>(VertexAttributeType type, unsigned int count)
	{
		m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::INT, false, count));
		m_size += count * m_attributes.back().GetSize();
	}

	template <>
	void Push<unsigned char>(VertexAttributeType type, unsigned int count)
	{
		m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::UNSIGNED_BYTE, true, count));
		m_size += count * m_attributes.back().GetSize();
	}

	template <>
	void Push<char>(VertexAttributeType type, unsigned int count)
	{
		m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::BYTE, false, count));
		m_size += count * m_attributes.back().GetSize();
	}

	//template <>
	//void Push<Math::Vec2>(VertexAttributeType type, unsigned int count)
	//{
	//	m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::FLOAT, false, count * 2));
	//	m_size += count * VertexAttribute::GetSizeOfDataType(VertexAttributeDataType::FLOAT) * 2;
	//}

	//template <>
	//void Push<Math::Vec3>(VertexAttributeType type, unsigned int count)
	//{
	//	m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::FLOAT, false, count * 3));
	//	m_size += count * VertexAttribute::GetSizeOfDataType(VertexAttributeDataType::FLOAT) * 3;
	//}

	//template <>
	//void Push<Math::Vec4>(VertexAttributeType type, unsigned int count)
	//{
	//	m_attributes.push_back(VertexAttribute(type, VertexAttributeDataType::FLOAT, false, count * 4));
	//	m_size += count * VertexAttribute::GetSizeOfDataType(VertexAttributeDataType::FLOAT) * 4;
	//}

	const std::vector<VertexAttribute>& GetAttributes() const { return m_attributes; }
	unsigned int GetSize() const { return m_size; }

	void Clear();

	static std::unique_ptr<VertexLayout> Create();

protected:


protected:
	std::vector<VertexAttribute> m_attributes;
	unsigned int m_size;

};