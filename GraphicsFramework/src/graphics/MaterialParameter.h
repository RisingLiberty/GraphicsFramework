#pragma once

enum class MaterialParameterDataType
{
	UNSPECIFIED,

	BOOL,
	SHORT,
	UNSIGNED_SHORT,
	INT,
	UNSIGNED_INT,
	FLOAT,
	DOUBLE,
	VEC2,
	VEC3,
	VEC4,
	MAT3,
	MAT4
};

struct MaterialParameter
{
	MaterialParameter(const std::string& name, MaterialParameterDataType dataType, void* data = nullptr);
	MaterialParameter(const MaterialParameter& other);
	MaterialParameter(const MaterialParameter&& other);
	MaterialParameter& operator=(const MaterialParameter& other);
	MaterialParameter& operator=(const MaterialParameter&& other);

	~MaterialParameter();

	std::string name;
	MaterialParameterDataType data_type;
	void* data;

	size_t GetSize() const;
	void SetData(void* data);

};