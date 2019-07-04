#include "stdafx.h"

#include "OpenGLUILayer.h"
#include "scenegraph/LayerObject.h"

OpenGLUILayer::OpenGLUILayer(const std::string& title):
	UILayer(title)
{
	ImGui_ImplOpenGL3_Init("#version 330");
}

OpenGLUILayer::~OpenGLUILayer()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void OpenGLUILayer::Update(float dTime)
{

}

void OpenGLUILayer::Draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(m_name.c_str());

	for (std::unique_ptr<LayerObject>& object : m_objects)
	{
		object->Draw();
	}

	ImGui::End();
}