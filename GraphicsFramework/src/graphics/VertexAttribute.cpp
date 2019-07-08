#include "stdafx.h"

#include "VertexAttribute.h"

#include <GL/glew.h>

VertexAttribute::VertexAttribute(const VertexAttributeType attributeType, VertexAttributeDataType data_type, bool isNormalized, unsigned int count) :
	attribute_type(attributeType),
	data_type(data_type),
	count(count),
	is_normalized(isNormalized)
{
}

unsigned int VertexAttribute::GetSize() const
{
	switch (data_type)
	{
	case VertexAttributeDataType::BYTE: return sizeof(char);
	case VertexAttributeDataType::UNSIGNED_BYTE: return sizeof(unsigned char);
	case VertexAttributeDataType::INT: return sizeof(int);
	case VertexAttributeDataType::UNSIGNED_INT: return sizeof(unsigned int);
	case VertexAttributeDataType::FLOAT: return sizeof(float);
		//case VertexAttributedata_type::VEC2: return sizeof(Math::Vec2);
		//case VertexAttributedata_type::VEC3: return sizeof(Math::Vec3);
		//case VertexAttributedata_type::VEC4: return sizeof(Math::Vec4);

	default: ASSERT(false, "type not specified");
	}
	return 0;
}

unsigned int VertexAttribute::GetOpenGLDataType() const
{
	switch (data_type)
	{
	case VertexAttributeDataType::BYTE: return GL_BYTE;
	case VertexAttributeDataType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
	case VertexAttributeDataType::INT: return GL_INT;
	case VertexAttributeDataType::UNSIGNED_INT: return GL_UNSIGNED_INT;
	case VertexAttributeDataType::FLOAT: return GL_FLOAT;
		//case VertexAttributedata_type::VEC2: return GL_FLOAT_VEC2;
		//case VertexAttributedata_type::VEC3: return GL_FLOAT_VEC3;
		//case VertexAttributedata_type::VEC4: return GL_FLOAT_VEC4;

	default: ASSERT(false, "type not specified");
	}
	return 0;
}

VkFormat VertexAttribute::GetVulkanDataType() const
{
	switch (data_type)
	{
	case VertexAttributeDataType::BYTE: return VK_FORMAT_R8_SINT;
	case VertexAttributeDataType::UNSIGNED_BYTE: return VK_FORMAT_R8_UINT;
	case VertexAttributeDataType::INT: return VK_FORMAT_R32_SINT;
	case VertexAttributeDataType::UNSIGNED_INT: return VK_FORMAT_R32_UINT;
	case VertexAttributeDataType::FLOAT: 
		if (count == 1)
			return VK_FORMAT_R32_SFLOAT;
		else if (count == 2)
			return VK_FORMAT_R32G32_SFLOAT;
		else if (count == 3)
			return VK_FORMAT_R32G32B32_SFLOAT;
		else if (count == 4)
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		//case VertexAttributedata_type::VEC2: return GL_FLOAT_VEC2;
		//case VertexAttributedata_type::VEC3: return GL_FLOAT_VEC3;
		//case VertexAttributedata_type::VEC4: return GL_FLOAT_VEC4;

	default: ASSERT(false, "type not specified");
	}
	return VK_FORMAT_UNDEFINED;
}
