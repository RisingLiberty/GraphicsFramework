#pragma once

class OpenGLCommandList;

class OpenGLCommandQueue
{
public:
	OpenGLCommandQueue();
	~OpenGLCommandQueue();

	OpenGLCommandList* GetCommandList() const;

private:
	std::vector<std::unique_ptr<OpenGLCommandList>> m_command_lists;
};