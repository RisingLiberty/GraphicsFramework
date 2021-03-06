#include "stdafx.h"

#include "Dx11UILayer.h"
#include "scenegraph/LayerObject.h"

Dx11UILayer::Dx11UILayer(const std::string& title):
	UILayer(title)
{
}

Dx11UILayer::~Dx11UILayer()
{
}

void Dx11UILayer::Update(float dTime)
{
	UILayer::Update(dTime);
}

void Dx11UILayer::Draw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(m_name.c_str()/*, nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize*/);
	for (std::unique_ptr<LayerObject>& object : m_objects)
	{
		object->Draw();
	}

	ImGui::End();

}