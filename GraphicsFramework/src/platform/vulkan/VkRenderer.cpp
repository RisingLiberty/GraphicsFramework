#include "stdafx.h"

#include "VkRenderer.h"
#include "VkHelperMethods.h"
#include "VkContext.h"

#include "scenegraph/SceneObject.h"
#include "graphics/Material.h"
#include "graphics/Mesh.h"
#include "graphics/IndexBuffer.h"

VkRenderer::VkRenderer()
{

}

VkRenderer::~VkRenderer()
{

}

void VkRenderer::Draw()
{
	for (SceneObject* object : m_scene_objects)
	{
		Mesh* mesh = object->GetMesh();
		Material* material = object->GetMaterial();

		material->Use();

		VkCommandBuffer command_buffer = GetVkCurrentCommandBuffer();
		GetVkContext()->BindResourcesToPipeline();
		vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(mesh->GetIndices()->GetCount()), 1, 0, 0, 0);
	}

	m_scene_objects.clear();
}

void VkRenderer::ClearAllBuffers()
{
}

void VkRenderer::ClearColorBuffer()
{
}

void VkRenderer::ClearDepthStencilBuffer()
{
}

void VkRenderer::ClearDepthBuffer()
{
}

void VkRenderer::ClearStencilBuffer()
{
}

void VkRenderer::Begin()
{
}

void VkRenderer::End()
{
}

void VkRenderer::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), GetVkCurrentCommandBuffer());
}
