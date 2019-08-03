#pragma once

#include "UIObject.h"

class UIButton : public UIObject
{
public: 
	UIButton(const std::string& text, unsigned int width, unsigned int height, std::function<void()> callbackFunction = nullptr);
	virtual ~UIButton();

	virtual void Update(float dTime) override;
	virtual void Draw() override;

	bool IsPressed() const;

private:
	std::string m_text;
	unsigned int width;
	unsigned int height;

	std::function<void()> m_callback_function;
	bool m_is_pressed;
};