#include "stdafx.h"

#include "Dx12UILayer.h"
#include "scenegraph/LayerObject.h"

Dx12UILayer::Dx12UILayer(const std::string& title) :
	UILayer(title)
{

}

Dx12UILayer::~Dx12UILayer()
{
}

void Dx12UILayer::Update(float dTime)
{
	UILayer::Update(dTime);
}

void Dx12UILayer::Draw()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(m_name.c_str()/*, nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize*/);
	for (std::unique_ptr<LayerObject>& object : m_objects)
	{
		object->Draw();
	}
	ImGui::End();
}
