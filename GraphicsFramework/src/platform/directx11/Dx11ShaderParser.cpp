#include "stdafx.h"

#include "Dx11ShaderParser.h"

namespace
{
	Dx11ShaderStruct FindStruct(const std::vector<Dx11ShaderStruct>& definedStructs, const std::string& name)
	{
		for (const Dx11ShaderStruct& s : definedStructs)
		{
			if (s.name == name)
				return s;
		}
		//TODO: change this
		return Dx11ShaderStruct("", {});
	}

	UniformDataType Dx11UniformDataTypeToCustomDataType(const std::string& type)
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

	Dx11ShaderTexture::Type Dx11ShaderTextureTypeStringToEnum(const std::string& type)
	{
		if (type == "Texture2D")		return Dx11ShaderTexture::Type::TEXTURE2D;
		if (type == "TextureCube")		return Dx11ShaderTexture::Type::TEXTURECUBE;

		return Dx11ShaderTexture::Type::UNSPECIFIED;
	}

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

	Dx11ShaderStruct ParseStruct(const std::string& block)
	{
		std::vector<std::string> tokens = Tokenize(block);
		unsigned int index = 0;
		index++; // struct
		std::string struct_name = tokens[index++];
		index++; // {
		std::vector<Dx11ShaderUniform> uniforms;
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

			UniformDataType data_type = Dx11UniformDataTypeToCustomDataType(type);

			Dx11ShaderUniform field = Dx11ShaderUniform(name, data_type, offset);
			offset += field.size;
			uniforms.push_back(field);
		}
		return Dx11ShaderStruct(struct_name, uniforms);
	}

	Dx11ShaderCBuffer ParseCBuffer(const std::string& block, const std::vector<Dx11ShaderStruct>& definedStructs)
	{
		std::vector<std::string> tokens = Tokenize(block);

		unsigned int index = 1;
		std::string buffer_name = tokens[index++];
		unsigned int reg = 0;
		if (tokens[index++] == ":") // Register specified
			reg = GetRegister(tokens[index++]);

		std::unique_ptr<Dx11ShaderCBuffer> buffer;

		index++; // {
		std::vector<std::unique_ptr<Dx11ShaderUniform>> uniforms;
		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;

			std::string type = tokens[index++];
			std::string name = tokens[index++];

			// Strip ; from name if present
			if (const char* s = strstr(name.c_str(), ";"))
				name = std::string(name.c_str(), s - name.c_str());

			UniformDataType data_type = Dx11UniformDataTypeToCustomDataType(type);

			// is user defined?
			if (data_type == UniformDataType::UNSPECIFIED)
			{
				// only supports user defined structs
				Dx11ShaderStruct s = FindStruct(definedStructs, type);
				assert(!s.uniforms.empty()); // if this asserts, compilation would also fail
				//TODO: fix this
				uniforms.push_back(std::make_unique<Dx11ShaderStruct>(s.name, s.uniforms));
			}
			else
				uniforms.push_back(std::make_unique<Dx11ShaderUniform>(name, data_type));
		}
		return Dx11ShaderCBuffer(buffer_name, reg, uniforms);
	}

	Dx11ShaderTexture ParseTexture(const std::string& statement)
	{
		std::vector<std::string> tokens = Tokenize(statement);
		// Support Texture2D and TextureCube for now
		unsigned int index = 0;

		unsigned int reg = 0;
		std::string type = tokens[index++];
		std::string name = tokens[index++];
		std::string count_str;
		unsigned int count = 1;
		if (name.find("["))
			count = GetCount(name);

		if (tokens[index++] == ":") // Register specified
			reg = GetRegister(tokens[index++]);

		return Dx11ShaderTexture(name, Dx11ShaderTextureTypeStringToEnum(type), count, reg);
	}

	Dx11ShaderSamplerState ParseSamplerState(const std::string& statement)
	{
		std::vector<std::string> tokens = Tokenize(statement);
		// Support Texture2D and TextureCube for now
		unsigned int index = 0;

		unsigned int reg = 0;
		std::string type = tokens[index++];
		std::string name = tokens[index++];
		std::string count_str;
		unsigned int count = 1;
		if (name.find("["))
			count = GetCount(name);

		if (tokens[index++] == ":") // Register specified
			reg = GetRegister(tokens[index++]);

		return Dx11ShaderSamplerState(name, count, reg);
	}

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

Dx11ParsedShader::Dx11ParsedShader() = default;
Dx11ParsedShader::~Dx11ParsedShader() = default;

Dx11ShaderParser::Dx11ShaderParser() = default;
Dx11ShaderParser::~Dx11ShaderParser() = default;

Dx11ParsedShader Dx11ShaderParser::Parse(const std::string& code)
{
	Dx11ParsedShader parsed_shader;

	const char* token;
	const char* str;

	std::string pure_code = RemoveComments(code);

	str = pure_code.c_str();
	while (token = FindToken(str, "struct"))
		parsed_shader.structs.push_back(ParseStruct(GetBlock(token, &str)));

	str = pure_code.c_str();
	while (token = FindToken(str, "cbuffer"))
		parsed_shader.buffers.push_back(ParseCBuffer(GetBlock(token, &str),parsed_shader.structs));

	str = pure_code.c_str();
	while (token = FindToken(str, "Texture2D"))
		parsed_shader.textures.push_back(ParseTexture(GetStatement(token, &str)));

	str = pure_code.c_str();
	while (token = FindToken(str, "TextureCube"))
		parsed_shader.textures.push_back(ParseTexture(GetStatement(token, &str)));

	str = pure_code.c_str();
	while (token = FindToken(str, "SamplerState"))
		parsed_shader.sampler_states.push_back(ParseSamplerState(GetStatement(token, &str)));

	return parsed_shader;
}