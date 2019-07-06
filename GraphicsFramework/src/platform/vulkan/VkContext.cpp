#include "stdafx.h"

#include "VkContext.h"

VkContext::VkContext(Window* window)
{

}

VkContext::~VkContext()
{

}

void VkContext::Initialize()
{
}

void VkContext::Present()
{
}

Context::API VkContext::GetApiType() const
{
	return API::VULKAN;
}
