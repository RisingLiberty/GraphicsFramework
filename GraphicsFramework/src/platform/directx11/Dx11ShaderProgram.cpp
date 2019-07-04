#include "stdafx.h"

#include "Dx11ShaderProgram.h"
#include "Dx11VertexShader.h"
#include "Dx11FragmentShader.h"
#include "Dx11Context.h"
#include "Dx11ShaderUniform.h"
#include "Dx11ShaderStruct.h"
#include "Dx11ShaderCBuffer.h"
#include "Dx11ShaderTexture.h"
#include "Dx11ShaderSamplerState.h"
#include "Dx11HelperMethods.h"


ParsedShader::ParsedShader()
{

}

ParsedShader::~ParsedShader()
{

}

namespace
{
	static std::vector<Dx11ShaderStruct> s_defined_structs;

	Dx11ShaderStruct FindStruct(const std::string& name)
	{
		for (const Dx11ShaderStruct& s : s_defined_structs)
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
		size_t  length = end - str + 1;
		return std::string(str, length);
	}

	Dx11ShaderStruct ParseStruct(const std::string& block)
	{
		std::vector<std::string> tokens = Tokenize(block);
		unsigned int index = 0;
		index++; // struct
		std::string structName = tokens[index++];
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
		return Dx11ShaderStruct(structName, uniforms);
	}

	Dx11ShaderCBuffer ParseCBuffer(const std::string& block)
	{
		std::vector<std::string> tokens = Tokenize(block);

		unsigned int index = 1;
		std::string bufferName = tokens[index++];
		unsigned int reg = 0;
		if (tokens[index++] == ":") // Register specified
		{
			std::string cbRegister = tokens[index++];
			reg = NextInt(cbRegister);
		}

		std::unique_ptr<Dx11ShaderCBuffer> buffer;

		Shader::Type shaderType;
		if (StartsWith(bufferName, "VS"))
			shaderType = Shader::Type::VERTEX;
		else if (StartsWith(bufferName, "PS"))
			shaderType = Shader::Type::FRAGMENT;
		else
			assert(false);

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

			// TODO: Debug mode validation: make sure every single uniform in a system uniform buffer starts with sys_!
			//if (buffer == nullptr)
			//{
			//	if (StartsWith(name, "sys_"))
			//	{
			//		switch (shaderType)
			//		{
			//		case 0:
			//			m_VSUniformBuffers.push_back(buffer);
			//			break;
			//		case 1:
			//			m_PSUniformBuffers.push_back(buffer);
			//			break;
			//		}
			//	}
			//	else
			//	{
			//		switch (shaderType)
			//		{
			//		case 0:
			//			assert(m_VSUserUniformBuffer == nullptr);
			//			m_VSUserUniformBuffer = buffer;
			//			break;
			//		case 1:
			//			assert(m_PSUserUniformBuffer == nullptr);
			//			m_PSUserUniformBuffer = buffer;
			//			break;
			//		}
			//	}
			//}
			UniformDataType t = Dx11UniformDataTypeToCustomDataType(type);

			// is user defined?
			if (t == UniformDataType::UNSPECIFIED)
			{
				// only supports user defined structs
				Dx11ShaderStruct s = FindStruct(type);
				assert(!s.uniforms.empty()); // if this asserts, compilation would also fail
				//TODO: fix this
				uniforms.push_back(std::make_unique<Dx11ShaderStruct>(s.name, s.uniforms));
			}
			else
				uniforms.push_back(std::make_unique<Dx11ShaderUniform>(name, t));
		}
		return Dx11ShaderCBuffer(bufferName, reg, shaderType, uniforms);
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
		{
			count_str = name.substr(name.find("["));
			count_str.pop_back();
			count = std::stoi(count_str);
		}

		if (tokens[index++] == ":") // Register specified
		{
			std::string texRegister = tokens[index++];
			reg = NextInt(texRegister);
		}

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
		{
			count_str = name.substr(name.find("["));
			count_str.pop_back();
			count = std::stoi(count_str);
		}
		
		if (tokens[index++] == ":") // Register specified
		{
			std::string texRegister = tokens[index++];
			reg = NextInt(texRegister);
		}

		return Dx11ShaderSamplerState(name, count, reg);
	}

	std::string RemoveRange(const std::string& string, unsigned int start, unsigned int length)
	{
		std::string result = string;
		return result.erase(start, length);
	}

	int FindPosition(const std::string& string, const std::string& search, unsigned int  offset)
	{
		const char* str = string.c_str() + offset;
		const char* found = strstr(str, search.c_str());
		if (found == nullptr)
			return -1;
		return (int)(found - str) + offset;
	}

	std::string RemoveComments(const std::string& source)
	{
		const char* str = source.c_str();

		std::string result = source;
		int startPos;
		while ((startPos = FindPosition(result, "/*", 0)) != -1)
		{
			int endPos = FindPosition(result, "*/", 0);
			assert(endPos != -1);
			result = RemoveRange(result, startPos, endPos - startPos + 2);
		}

		while ((startPos = FindPosition(result, "//", 0)) != -1)
		{
			int endPos = FindPosition(result, "\n", startPos);
			assert(endPos != -1);
			result = RemoveRange(result, startPos, endPos - startPos + 1);
		}

		return result;
	}

	const char* FindToken(const char* str, const std::string& token)
	{
		const char* t = str;
		while (t = strstr(t, token.c_str()))
		{
			bool left = str == t || isspace(t[-1]);
			bool right = !t[token.size()] || isspace(t[token.size()]);
			if (left && right)
				return t;

			t += token.size();
		}
		return nullptr;
	}

	ParsedShader ParseShader(const std::string& code)
	{
		const char* token;
		const char* str;

		std::string pure_code = RemoveComments(code);

		ParsedShader parsed_shader = ParsedShader();

		str = pure_code.c_str();
		while (token = FindToken(str, "struct"))
			parsed_shader.structs.push_back(ParseStruct(GetBlock(token, &str)));

		str = pure_code.c_str();
		while (token = FindToken(str, "cbuffer"))
			parsed_shader.buffers.push_back(ParseCBuffer(GetBlock(token, &str)));

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
}

Dx11ShaderProgram::Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
	Dx11VertexShader* dx_vs = dynamic_cast<Dx11VertexShader*>(vs);
	Dx11FragmentShader* dx_fs = dynamic_cast<Dx11FragmentShader*>(fs);

	ParsedShader vs_parsed = std::move(ParseShader(dx_vs->GetCode()));
	ParsedShader fs_parsed = std::move(ParseShader(dx_fs->GetCode()));

	if (!vs_parsed.buffers.empty())
	{
		m_vs_constant_buffer = std::make_unique<Dx11ShaderCBuffer>(vs_parsed.buffers[0]);
	}

	if (!fs_parsed.buffers.empty())
	{
		m_fs_constant_buffer = std::make_unique<Dx11ShaderCBuffer>(fs_parsed.buffers[0]);
		for (const std::unique_ptr<Dx11ShaderUniform>& uniform : m_fs_constant_buffer->uniforms)
			m_uniforms.emplace_back(std::make_unique<ShaderUniform>(uniform->name, uniform->type, uniform->size));
	}

	this->Bind();

	//for (const Dx11ShaderCBuffer& buffer : m_vs_parsed.buffers)
	//{
	//	for (const std::unique_ptr<Dx11ShaderUniform>& uniform : buffer.uniforms)
	//		m_uniforms.emplace_back(std::make_unique<ShaderUniform>(uniform->name, uniform->type, uniform->size));
	//}

	//for (const Dx11ShaderCBuffer& buffer : m_fs_parsed.buffers)
	//{
	//	for (const std::unique_ptr<Dx11ShaderUniform>& uniform : buffer.uniforms)
	//	{
	//		m_uniforms.emplace_back(std::make_unique<ShaderUniform>(uniform->name, uniform->type, uniform->size));
	//		std::vector<std::unique_ptr<Dx11ShaderUniform>> x;
	//		x.push_back(std::make_unique<Dx11ShaderUniform>(uniform->name, uniform->type, uniform->offset, uniform->offset));
	//		m_fs_per_object_buffer = std::make_unique<Dx11ShaderCBuffer>(buffer.name, buffer.reg, buffer.shader_type, x);
	//	}
	//}
}


Dx11ShaderProgram::~Dx11ShaderProgram()
{
}

void Dx11ShaderProgram::Bind() const
{
	Dx11VertexShader* dx_vs = dynamic_cast<Dx11VertexShader*>(m_vertex_shader);
	Dx11FragmentShader* dx_fs = dynamic_cast<Dx11FragmentShader*>(m_fragment_shader);

	GetDx11Context()->BindVertexShader(dx_vs);
	GetDx11Context()->BindFragmentShader(dx_fs);

	if (m_vs_constant_buffer)
		GetDx11DeviceContext()->VSSetConstantBuffers(0, 1, m_vs_constant_buffer->buffer.GetAddressOf());

	if (m_fs_constant_buffer)
	GetDx11DeviceContext()->PSSetConstantBuffers(0, 1, m_fs_constant_buffer->buffer.GetAddressOf());
}

void Dx11ShaderProgram::Unbind() const
{

}

void Dx11ShaderProgram::SetBoolUniform(const std::string& name, bool uniform)
{
}

void Dx11ShaderProgram::SetUShortUniform(const std::string& name, unsigned short & uniform)
{
}

void Dx11ShaderProgram::SetShortUniform(const std::string& name, short uniform)
{
}

void Dx11ShaderProgram::SetIntUniform(const std::string& name, int uniform)
{
}

void Dx11ShaderProgram::SetUIntUniform(const std::string& name, unsigned int & uniform)
{
}

void Dx11ShaderProgram::SetFloatUniform(const std::string& name, float uniform)
{
}

void Dx11ShaderProgram::SetDoubleUniform(const std::string& name, double uniform)
{
}

void Dx11ShaderProgram::SetVec2Uniform(const std::string& name, float x, float y)
{
}

void Dx11ShaderProgram::SetVec2Uniform(const std::string& name, float * values)
{
}

void Dx11ShaderProgram::SetVec3Uniform(const std::string& name, float x, float y, float z)
{
}

void Dx11ShaderProgram::SetVec3Uniform(const std::string& name, float * values)
{
}

void Dx11ShaderProgram::SetVec4Uniform(const std::string& name, float x, float y, float z, float w)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	ZeroMemory(&msr, sizeof(msr));

	float values[] = { x,y,z,w };
	unsigned int size = sizeof(values);

	GetDx11DeviceContext()->Map(m_fs_constant_buffer->buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
	memcpy(msr.pData, values, size);
	GetDx11DeviceContext()->Unmap(m_fs_constant_buffer->buffer.Get(), NULL);

	if (m_fs_constant_buffer)
		GetDx11DeviceContext()->PSSetConstantBuffers(0, 1, m_fs_constant_buffer->buffer.GetAddressOf());

}

void Dx11ShaderProgram::SetVec4Uniform(const std::string& name, float * values)
{
	this->SetVec4Uniform(name, values[0], values[1], values[2], values[3]);
}

void Dx11ShaderProgram::SetMat3Uniform(const std::string& name, float * values)
{
}

void Dx11ShaderProgram::SetMat4Uniform(const std::string& name, float * values)
{
}

Dx11VertexShader* Dx11ShaderProgram::GetDxVertexShader() const
{
	return dynamic_cast<Dx11VertexShader*>(m_vertex_shader);
}

Dx11FragmentShader* Dx11ShaderProgram::GetDxFragmentShader() const
{
	return dynamic_cast<Dx11FragmentShader*>(m_fragment_shader);
}