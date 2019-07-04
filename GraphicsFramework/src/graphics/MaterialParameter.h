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
public:
	MaterialParameter(const std::string& name, MaterialParameterDataType dataType, void* data = nullptr);
	MaterialParameter(const MaterialParameter& other);
	MaterialParameter(const MaterialParameter&& other);
	MaterialParameter& operator=(const MaterialParameter& other);
	MaterialParameter& operator=(const MaterialParameter&& other);

	~MaterialParameter();

	bool operator==(const MaterialParameter& other) const;

	unsigned int GetSize() const;
	void SetData(void* data);

public:
	std::string name;
	MaterialParameterDataType data_type;
	char* data;
};