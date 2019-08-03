#include "stdafx.h"

#include "VkDirectCommandList.h"
#include "VkCommandQueue.h"

VkDirectCommandList::VkDirectCommandList(const VkCommandQueue* queue):
	m_queue(queue)
{
	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	this->Begin(begin_info);
}

VkDirectCommandList::~VkDirectCommandList()
{
	this->End();
	m_queue->DirectSubmit(this->GetDirectSubmitInfo());
}