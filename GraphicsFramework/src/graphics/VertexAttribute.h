#pragma once

enum class VertexAttributeType
{
	POSITION,
	NORMAL,
	TEXCOORD
};

enum class VertexAttributeDataType
{
	BYTE,
	UNSIGNED_BYTE,
	INT,
	UNSIGNED_INT,
	FLOAT,
	//VEC2,
	//VEC3,
	//VEC4
};

struct VertexAttribute
{
public:
	VertexAttribute(const VertexAttributeType attributeType, VertexAttributeDataType dataType, bool isNormalized, unsigned int count = 1);

	static unsigned int GetSizeOfDataType(VertexAttributeDataType type);
	unsigned int GetOpenGLDataType() const;
public:

	VertexAttributeType AttributeType; //Attribute type for debug reasons
	VertexAttributeDataType DataType; //What datatype is this? (float, int,...)
	unsigned int Count; //How many datatypes are there?
	bool IsNormalized;
};
