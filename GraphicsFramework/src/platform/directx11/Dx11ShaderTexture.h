#pragma once

struct Dx11ShaderTexture
{
public:
	enum class Type
	{
		UNSPECIFIED,

		TEXTURE2D,
		TEXTURECUBE
	};

	Dx11ShaderTexture(const std::string& name, Type type, unsigned int count, unsigned int reg);
	~Dx11ShaderTexture();

public:
	std::string name;
	Type type;
	unsigned int count;
	unsigned int reg;
	
};