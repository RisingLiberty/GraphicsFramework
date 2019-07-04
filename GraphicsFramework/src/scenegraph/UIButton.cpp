#include "stdafx.h"

#include "UIButton.h"

UIButton::UIButton(const std::string& text, unsigned int width, unsigned int height):
	UIObject(text),
	width(width),
	height(height)
{

}

UIButton::~UIButton()
{

}

void UIButton::Update(float dTime)
{

}

void UIButton::Draw()
{
	ImGui::Button("Button", ImVec2((float)width, (float)height));
}
