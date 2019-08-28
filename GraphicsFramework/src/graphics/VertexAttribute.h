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

	unsigned int GetSize() const;
	unsigned int GetGLDataType() const;
	VkFormat GetVulkanDataType() const;
public:

	VertexAttributeType attribute_type; //Attribute type for debug reasons
	VertexAttributeDataType data_type; //What datatype is this? (float, int,...)
	unsigned int count; //How many datatypes are there?
	bool is_normalized;
};
