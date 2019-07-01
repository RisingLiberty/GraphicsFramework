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

// windows headers
#include <Windows.h>
#include <windowsx.h>

// custom headers
#include "exceptions/Exceptions.h"
#include "exceptions/Throws.h"

#include "core/DefaultEnums.h"