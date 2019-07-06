#pragma once

// spdlog headers
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

// Imgui headers
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/imgui/imgui_impl_dx11.h"
#include "vendor/imgui/imgui_impl_dx12.h"
#include "vendor/imgui/imgui_impl_vulkan.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

// stl headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <functional>
#include <algorithm>
#include <array>
#include <set>

// directx headers
#include <d3d12.h>
#include <dx12/d3dx12.h>
#include <d3d11on12.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

// vulkan header
#include <vulkan/vulkan.h>

// windows headers
#include <Windows.h>
#include <windowsx.h>
#include <wrl.h> // windows run time libraries
using Microsoft::WRL::ComPtr;

// custom headers
#include "exceptions/Exceptions.h"
#include "exceptions/Throws.h"
#include "debug/Debug.h"

#include "core/DefaultEnums.h"