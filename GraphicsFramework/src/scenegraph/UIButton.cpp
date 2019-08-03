#include "stdafx.h"

#include "UIButton.h"

UIButton::UIButton(const std::string& text, unsigned int width, unsigned int height, std::function<void()> callbackFunction):
	UIObject(text),
	width(width),
	height(height),
	m_callback_function(callbackFunction),
	m_is_pressed(false)
{

}

UIButton::~UIButton()
{

}

void UIButton::Update(float dTime)
{
	if (m_is_pressed)
		m_callback_function();

	m_is_pressed = false;
}

void UIButton::Draw()
{
	m_is_pressed = ImGui::Button(m_name.c_str(), ImVec2((float)width, (float)height));
}

bool UIButton::IsPressed() const
{
	return m_is_pressed;
}