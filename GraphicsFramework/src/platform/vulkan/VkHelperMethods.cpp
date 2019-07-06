#include "stdafx.h"

#include "VkHelperMethods.h"

#include "VkContext.h"

VkContext* GetVkContext()
{
	return dynamic_cast<VkContext*>(Context::GetCurrent());
}