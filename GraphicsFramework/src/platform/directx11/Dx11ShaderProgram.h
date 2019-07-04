#pragma once

#include "graphics/ShaderProgram.h"

#include "Dx11ShaderStruct.h"
#include "Dx11ShaderCBuffer.h"
#include "Dx11ShaderTexture.h"
#include "Dx11ShaderSamplerState.h"


struct Dx11ShaderCBuffer;

struct ParsedShader
{
	ParsedShader();
	~ParsedShader();

	std::vector<Dx11ShaderStruct> structs;
	std::vector<Dx11ShaderCBuffer> buffers;
	std::vector<Dx11ShaderTexture> textures;
	std::vector<Dx11ShaderSamplerState> sampler_states;
};

class Dx11ShaderProgram : public ShaderProgram
{
public:
	Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~Dx11ShaderProgram();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetBoolUniform(const std::string& name, bool uniform) override;
	virtual void SetUShortUniform(const std::string& name, unsigned short& uniform) override;
	virtual void SetShortUniform(const std::string& name, short uniform) override;
	virtual void SetIntUniform(const std::string& name, int uniform) override;
	virtual void SetUIntUniform(const std::string& name, unsigned int& uniform) override;
	virtual void SetFloatUniform(const std::string& name, float uniform) override;
	virtual void SetDoubleUniform(const std::string& name, double uniform) override;
	virtual void SetVec2Uniform(const std::string& name, float x, float y) override;
	virtual void SetVec2Uniform(const std::string& name, float* values) override;
	virtual void SetVec3Uniform(const std::string& name, float x, float y, float z) override;
	virtual void SetVec3Uniform(const std::string& name, float* values) override;
	virtual void SetVec4Uniform(const std::string& name, float x, float y, float z, float w) override;
	virtual void SetVec4Uniform(const std::string& name, float* values) override;
	virtual void SetMat3Uniform(const std::string& name, float* values) override;
	virtual void SetMat4Uniform(const std::string& name, float* values) override;

private:
	class Dx11VertexShader* GetDxVertexShader() const;
	class Dx11FragmentShader* GetDxFragmentShader() const;

private:

	std::unique_ptr<Dx11ShaderCBuffer> m_vs_constant_buffer;
	std::unique_ptr<Dx11ShaderCBuffer> m_fs_constant_buffer;

	//// Allow only 1 per shader vs and ps per pass buffer
	//std::unique_ptr<Dx11ShaderCBuffer> m_vs_per_pass_buffer;
	//std::unique_ptr<Dx11ShaderCBuffer> m_fs_per_pass_buffer;
	//
	//// Allow only 1 per shader vs and ps per object buffer
	//std::unique_ptr<Dx11ShaderCBuffer> m_vs_per_object_buffer;
	//std::unique_ptr<Dx11ShaderCBuffer> m_fs_per_object_buffer;

	//ParsedShader m_vs_parsed;
	//ParsedShader m_fs_parsed;

};

