#pragma once

#include "graphics/CommandList.h"
#include "graphics/RasterizerSettings.h"

class ApiBufferWrapper;
class ShaderProgram;
class VertexArray;
class IndexBuffer;
struct Dx11ShaderCBuffer;

class Dx11CommandList : public CommandList
{
public:
	Dx11CommandList();
	virtual ~Dx11CommandList();
};