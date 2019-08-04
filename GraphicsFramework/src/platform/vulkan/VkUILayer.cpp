#include "stdafx.h"

#include "VkUILayer.h"
#include "scenegraph/LayerObject.h"

VkUILayer::VkUILayer(const std::string& title) :
	UILayer(title)
{
}

VkUILayer::~VkUILayer()
{

}

void VkUILayer::Update(float dTime)
{
	UILayer::Update(dTime);
}

void VkUILayer::Draw()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(m_name.c_str()/*, nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize*/);
	for (std::unique_ptr<LayerObject>& object : m_objects)
	{
		object->Draw();
	}
	ImGui::End();

}