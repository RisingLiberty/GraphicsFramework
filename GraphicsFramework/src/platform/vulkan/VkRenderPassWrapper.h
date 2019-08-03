#pragma once

#include "graphics/BufferHelpers.h"

class VkRenderPassWrapper
{
public:
	VkRenderPassWrapper(Format format);
	~VkRenderPassWrapper();

	VkRenderPass GetApiRenderPass() const;

private:
	VkRenderPass m_render_pass;
};