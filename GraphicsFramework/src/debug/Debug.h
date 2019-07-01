#pragma once

#define ASSERT(x, message) if(!(x))\
{\
__debugbreak();\
spdlog::error(message);\
}
