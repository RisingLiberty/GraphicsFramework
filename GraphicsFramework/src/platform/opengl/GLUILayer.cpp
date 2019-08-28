#include "stdafx.h"

#include "GLUILayer.h"
#include "scenegraph/LayerObject.h"

GLUILayer::GLUILayer(const std::string& title):
	UILayer(title)
{
}

GLUILayer::~GLUILayer()
{
}

void GLUILayer::Update(float dTime)
{
	UILayer::Update(dTime);
}

void GLUILayer::Draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(m_name.c_str()/*, nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize*/);

	for (std::unique_ptr<LayerObject>& object : m_objects)
	{
		object->Draw();
	}

	ImGui::End();
}