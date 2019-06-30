#pragma once

class Window
{
public:
	Window(unsigned int width, unsigned int height);
	virtual ~Window();

private:
	unsigned int m_width;
	unsigned int m_height;
};