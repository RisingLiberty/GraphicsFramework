#pragma once

#include "UIObject.h"

class UIButton : public UIObject
{
public: 
	UIButton(const std::string& text, unsigned int width, unsigned int height);
	virtual ~UIButton();

	virtual void Update(float dTime) override;
	virtual void Draw() override;

private:
	std::string m_text;
	unsigned int width;
	unsigned int height;
};