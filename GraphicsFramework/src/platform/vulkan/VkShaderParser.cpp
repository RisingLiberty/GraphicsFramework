#include "stdafx.h"

#include "VkShaderParser.h"

namespace
{
	VkShaderStruct FindStruct(const std::vector<VkShaderStruct>& definedStructs, const std::string& name)
	{
		for (const VkShaderStruct& s : definedStructs)
		{
			if (s.name == name)
				return s;
		}
		//TODO: change this
		return VkShaderStruct("", {});
	}

	UniformDataType VkUniformDataTypeToCustomDataType(const std::string& type)
	{
		if (type == "int")			return UniformDataType::INT;
		if (type == "float")		return UniformDataType::FLOAT;
		if (type == "float2")		return UniformDataType::VEC2;
		if (type == "float3")		return UniformDataType::VEC3;
		if (type == "float4")		return UniformDataType::VEC4;
		if (type == "float3x3")		return UniformDataType::MAT3;
		if (type == "float4x4")		return UniformDataType::MAT4;

		return UniformDataType::UNSPECIFIED;
	}

	//VkShaderTexture::Type VkShaderTextureTypeStringToEnum(const std::string& type)
	//{
	//	if (type == "Texture2D")		return VkShaderTexture::Type::TEXTURE2D;
	//	if (type == "TextureCube")		return VkShaderTexture::Type::TEXTURECUBE;

	//	return VkShaderTexture::Type::UNSPECIFIED;
	//}

	bool StartsWith(const std::string& string, const std::string& start)
	{
		return string.find(start) == 0;
	}

	int NextInt(const std::string& string)
	{
		const char* str = string.c_str();
		for (size_t i = 0; i < string.size(); i++)
		{
			if (isdigit(string[i]))
				return atoi(&string[i]);
		}
		return -1;
	}

	int GetCount(const std::string& string)
	{
		std::string count_str = string.substr(string.find("["));
		count_str.pop_back(); // pop ']'
		return std::stoi(count_str);
	}

	int GetRegister(const std::string& string)
	{
		std::string tex_register = string;
		return NextInt(tex_register);
	}

	std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
	{
		size_t start = 0;
		size_t end = string.find_first_of(delimiters);

		std::vector<std::string> result;

		while (end <= std::string::npos)
		{
			std::string token = string.substr(start, end - start);
			if (!token.empty())
				result.push_back(token);

			if (end == std::string::npos)
				break;

			start = end + 1;
			end = string.find_first_of(delimiters, start);
		}

		return result;
	}

	std::vector<std::string> SplitString(const std::string& string, const char delimiter)
	{
		return SplitString(string, std::string(1, delimiter));
	}

	std::vector<std::string> Tokenize(const std::string& string)
	{
		return SplitString(string, " \t\n");
	}

	std::vector<std::string> GetLines(const std::string& string)
	{
		return SplitString(string, "\n");
	}

	std::string GetBlock(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, "}");
		if (!end)
			return std::string(str);

		if (outPosition)
			*outPosition = end;
		size_t length = end - str + 1;
		return std::string(str, length);
	}

	std::string GetBlock(const std::string& string, unsigned int offset)
	{
		const char* str = string.c_str() + offset;
		return GetBlock(str, nullptr);
	}

	std::string GetStatement(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, ";");
		if (!end)
			return std::string(str);

		if (outPosition)
			*outPosition = end;
		size_t length = end - str + 1;
		return std::string(str, length);
	}

	VkShaderStruct ParseStruct(const std::string& block)
	{
		std::vector<std::string> tokens = Tokenize(block);
		unsigned int index = 0;
		index++; // struct
		std::string struct_name = tokens[index++];
		index++; // {
		std::vector<ShaderUniform> uniforms;
		unsigned int offset = 0;
		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;

			std::string type = tokens[index++];
			std::string name = tokens[index++];

			if (type == ":") // TODO: Temp
				continue;

			// Strip ; from name if present
			if (const char* s = strstr(name.c_str(), ";"))
				name = std::string(name.c_str(), s - name.c_str());

			UniformDataType data_type = VkUniformDataTypeToCustomDataType(type);

			ShaderUniform field = ShaderUniform(name, data_type, offset);
			offset += field.size;
			uniforms.push_back(field);
		}
		return VkShaderStruct(struct_name, uniforms);
	}

	VkShaderCBuffer ParseCBuffer(const std::string& block, const std::vector<VkShaderStruct>& definedStructs)
	{
		std::vector<std::string> tokens = Tokenize(block);

		unsigned int index = 1;
		std::string buffer_name = tokens[index++];
		unsigned int reg = 0;
		if (tokens[index++] == ":") // Register specified
			reg = GetRegister(tokens[index++]);

		std::unique_ptr<VkShaderCBuffer> buffer;

		index++; // {
		std::vector<std::unique_ptr<ShaderUniform>> uniforms;
		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;

			std::string type = tokens[index++];
			std::string name = tokens[index++];

			// Strip ; from name if present
			if (const char* s = strstr(name.c_str(), ";"))
				name = std::string(name.c_str(), s - name.c_str());

			UniformDataType data_type = VkUniformDataTypeToCustomDataType(type);

			// is user defined?
			if (data_type == UniformDataType::UNSPECIFIED)
			{
				// only supports user defined structs
				VkShaderStruct s = FindStruct(definedStructs, type);
				assert(!s.uniforms.empty()); // if this asserts, compilation would also fail
				//TODO: fix this
				uniforms.push_back(std::make_unique<VkShaderStruct>(s.name, s.uniforms));
			}
			else
				uniforms.push_back(std::make_unique<ShaderUniform>(name, data_type));
		}
		return VkShaderCBuffer(buffer_name, reg, uniforms);
	}

	//VkShaderTexture ParseTexture(const std::string& statement)
	//{
	//	std::vector<std::string> tokens = Tokenize(statement);
	//	// Support Texture2D and TextureCube for now
	//	unsigned int index = 0;

	//	unsigned int reg = 0;
	//	std::string type = tokens[index++];
	//	std::string name = tokens[index++];
	//	std::string count_str;
	//	unsigned int count = 1;
	//	if (name.find("["))
	//		count = GetCount(name);

	//	if (tokens[index++] == ":") // Register specified
	//		reg = GetRegister(tokens[index++]);

	//	return VkShaderTexture(name, VkShaderTextureTypeStringToEnum(type), count, reg);
	//}

	//VkShaderSamplerState ParseSamplerState(const std::string& statement)
	//{
	//	std::vector<std::string> tokens = Tokenize(statement);
	//	// Support Texture2D and TextureCube for now
	//	unsigned int index = 0;

	//	unsigned int reg = 0;
	//	std::string type = tokens[index++];
	//	std::string name = tokens[index++];
	//	std::string count_str;
	//	unsigned int count = 1;
	//	if (name.find("["))
	//		count = GetCount(name);

	//	if (tokens[index++] == ":") // Register specified
	//		reg = GetRegister(tokens[index++]);

	//	return VkShaderSamplerState(name, count, reg);
	//}

	std::string RemoveRange(const std::string& string, size_t start, size_t length)
	{
		std::string result = string;
		return result.erase(start, length);
	}

	size_t FindPosition(const std::string& string, const std::string& search, size_t offset)
	{
		return string.find(search, offset);

		//const char* str = string.c_str() + offset;
		//const char* found = strstr(str, search.c_str());
		//if (found == nullptr)
		//	return -1;
		//return (int)(found - str) + offset;
	}

	std::string RemoveComments(const std::string& source)
	{
		std::string result = source;
		size_t start_pos;
		while ((start_pos = FindPosition(result, "/*", 0)) != -1)
		{
			size_t end_pos = FindPosition(result, "*/", 0);
			assert(end_pos != -1);
			result = RemoveRange(result, start_pos, end_pos - start_pos + 2);
		}

		while ((start_pos = FindPosition(result, "//", 0)) != -1)
		{
			size_t end_pos = FindPosition(result, "\n", start_pos);
			assert(end_pos != -1);
			result = RemoveRange(result, start_pos, end_pos - start_pos + 1);
		}

		return result;
	}

	const char* FindToken(const char* str, const std::string& token)
	{
		const char* t = str;
		while (t = strstr(t, token.c_str()))
		{
			bool left = str == t || isspace(t[-1]); // no letters to the left
			bool right = !t[token.size()] || isspace(t[token.size()]); // no letters to the right
			if (left && right)
				return t;

			t += token.size();
		}
		return nullptr;
	}
}


VkParsedShader::VkParsedShader()
{

}

VkParsedShader::~VkParsedShader()
{

}

VkShaderParser::VkShaderParser()
{

}

VkShaderParser::~VkShaderParser()
{

}

