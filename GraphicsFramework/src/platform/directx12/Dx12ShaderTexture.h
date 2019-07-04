#pragma once

struct Dx12ShaderTexture
{
public:
	enum class Type
	{
		UNSPECIFIED,

		TEXTURE2D,
		TEXTURECUBE
	};

	Dx12ShaderTexture(const std::string& name, Type type, unsigned int count, unsigned int reg);
	~Dx12ShaderTexture();

public:
	std::string name;
	Type type;
	unsigned int count;
	unsigned int reg;
	
};