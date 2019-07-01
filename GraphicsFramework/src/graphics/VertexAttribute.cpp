#include "stdafx.h"

#include "VertexAttribute.h"

#include <GL/glew.h>

VertexAttribute::VertexAttribute(const VertexAttributeType attributeType, VertexAttributeDataType dataType, bool isNormalized, unsigned int count) :
	AttributeType(attributeType),
	DataType(dataType),
	IsNormalized(isNormalized),
	Count(count)
{
}

unsigned int VertexAttribute::GetSizeOfDataType(VertexAttributeDataType type)
{
	switch (type)
	{
	case VertexAttributeDataType::BYTE: return sizeof(char);
	case VertexAttributeDataType::UNSIGNED_BYTE: return sizeof(unsigned char);
	case VertexAttributeDataType::INT: return sizeof(int);
	case VertexAttributeDataType::UNSIGNED_INT: return sizeof(unsigned int);
	case VertexAttributeDataType::FLOAT: return sizeof(float);
		//case VertexAttributeDataType::VEC2: return sizeof(Math::Vec2);
		//case VertexAttributeDataType::VEC3: return sizeof(Math::Vec3);
		//case VertexAttributeDataType::VEC4: return sizeof(Math::Vec4);

	default: ASSERT(false, "type not specified");
	}
	return 0;
}

unsigned int VertexAttribute::GetOpenGLDataType() const
{
	switch (DataType)
	{
	case VertexAttributeDataType::BYTE: return GL_BYTE;
	case VertexAttributeDataType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
	case VertexAttributeDataType::INT: return GL_INT;
	case VertexAttributeDataType::UNSIGNED_INT: return GL_UNSIGNED_INT;
	case VertexAttributeDataType::FLOAT: return GL_FLOAT;
		//case VertexAttributeDataType::VEC2: return GL_FLOAT_VEC2;
		//case VertexAttributeDataType::VEC3: return GL_FLOAT_VEC3;
		//case VertexAttributeDataType::VEC4: return GL_FLOAT_VEC4;

	default: ASSERT(false, "type not specified");
	}
	return 0;
}
