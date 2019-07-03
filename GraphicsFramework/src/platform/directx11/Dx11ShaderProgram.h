#pragma once

#include "graphics/ShaderProgram.h"

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

	const std::vector<std::unique_ptr<ShaderUniform>>& GetUniforms() const override;
private:

	
};

