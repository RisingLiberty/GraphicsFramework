#include "stdafx.h"

#include "UILayer.h"
#include "graphics/Context.h"

#include "platform/opengl/GLUILayer.h"
#include "platform/directx11/Dx11UILayer.h"
#include "platform/directx12/Dx12UILayer.h"
#include "platform/vulkan/VkUILayer.h"

UILayer::UILayer(const std::string& title):
	Layer2D(title)
{

}

UILayer::~UILayer()
{

}

std::unique_ptr<Layer> UILayer::Create(const std::string& title)
{
	switch (Context::GetApi())
	{
	case API::OPENGL: return std::make_unique<GLUILayer>(title);
	case API::DIRECTX11: return std::make_unique<Dx11UILayer>(title);
	case API::DIRECTX12: return std::make_unique<Dx12UILayer>(title);
	case API::VULKAN: return std::make_unique<VkUILayer>(title);
	}

	return nullptr;
}

void UILayer::Update(float dTime)
{
	Layer2D::Update(dTime);
}

void UILayer::Draw()
{
	Layer2D::Draw();
}