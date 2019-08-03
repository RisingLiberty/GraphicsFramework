#pragma once

#include "VkCommandList.h"

class VkCommandQueue;

class VkDirectCommandList : public VkCommandList
{
public:
	VkDirectCommandList(const VkCommandQueue* queue);
	~VkDirectCommandList();

private:
	const VkCommandQueue* m_queue;
};