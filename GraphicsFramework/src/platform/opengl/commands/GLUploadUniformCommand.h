#pragma once

#include "graphics/commands/Command.h"
#include "graphics/ShaderUniform.h"

#include <variant>

class GLUploadUniformCommand : public Command
{
public:
	GLUploadUniformCommand(int location, ShaderUniform* uniform);
	virtual ~GLUploadUniformCommand();
	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	int m_location;
	ShaderUniform* m_uniform;
	//UniformDataType m_uniform_data_type;
	//std::variant<int, unsigned int, float, float*> m_data;
};