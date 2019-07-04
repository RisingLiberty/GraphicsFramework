#include "stdafx.h"

#include "Dx11HelperMethods.h"
#include "Dx11UILayer.h"
#include "scenegraph/LayerObject.h"

Dx11UILayer::Dx11UILayer(const std::string& title):
	UILayer(title)
{
	ImGui_ImplDX11_Init(GetDx11Device(), GetDx11DeviceContext());
}

Dx11UILayer::~Dx11UILayer()
{
	ImGui_ImplDX11_Shutdown();
}

void Dx11UILayer::Update(float dTime)
{

}

void Dx11UILayer::Draw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(m_name.c_str());
	for (std::unique_ptr<LayerObject>& object : m_objects)
	{
		object->Draw();
	}

	ImGui::End();

}