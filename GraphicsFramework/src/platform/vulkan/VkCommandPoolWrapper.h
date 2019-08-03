#pragma once

class VkCommandPoolWrapper
{
public:
	VkCommandPoolWrapper(unsigned int familyIndex);
	~VkCommandPoolWrapper();

	VkCommandPool GetApiCommandPool() const;

private:
	VkCommandPool m_pool;
};