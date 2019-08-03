#include "stdafx.h"

#include "VkRenderPassWrapper.h"
#include "VkHelperMethods.h"

VkRenderPassWrapper::VkRenderPassWrapper(Format format)
{
	// For render target
	VkAttachmentDescription color_attachment = {};
	color_attachment.format = format.ToVulkan();
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT; // needs to be this for imgui! aka no multi sampling
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// For multi sampling
	//VkAttachmentDescription color_attachment_resolve = {};
	//color_attachment_resolve.format = m_swapchain_image_format;
	//color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
	//color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// For depth and stencil data
	//VkAttachmentDescription depth_attachment = {};
	//depth_attachment.format = FindDepthFormat();
	//depth_attachment.samples = m_msaa_samples;
	//depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	//depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

	VkAttachmentReference color_attachment_ref = {};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//VkAttachmentReference color_attachment_resolve_ref = {};
	//color_attachment_resolve_ref.attachment = 2;
	//color_attachment_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//VkAttachmentReference depth_attachment_ref = {};
	//depth_attachment_ref.attachment = 1;
	//depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;
	//subpass.pDepthStencilAttachment = &depth_attachment_ref;
	//subpass.pResolveAttachments = &color_attachment_resolve_ref;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 1> attachments = { color_attachment };// , depth_attachment, color_attachment_resolve

	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	render_pass_info.pAttachments = attachments.data();
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

	VKCALL(vkCreateRenderPass(GetVkDevice(), &render_pass_info, nullptr, &m_render_pass));
}

VkRenderPassWrapper::~VkRenderPassWrapper()
{
	vkDestroyRenderPass(GetVkDevice(), m_render_pass, nullptr);
}

VkRenderPass VkRenderPassWrapper::GetApiRenderPass() const
{
	return m_render_pass;
}