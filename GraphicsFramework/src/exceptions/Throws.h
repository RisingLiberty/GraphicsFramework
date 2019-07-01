#pragma once

inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}

#ifndef ThrowIfFailed
#define ThrowIfFailed(x, exception)\
{\
    HRESULT _hr_ = (x);\
    if(FAILED(_hr_)) { throw exception(_hr_, L#x, AnsiToWString(__FILE__), __LINE__); } \
}

#define ThrowException(x)\
{\
	throw Exception(x, AnsiToWString(__FUNCTION__), AnsiToWString(__FILE__), __LINE__);\
}

#define ThrowContextException(x)\
{\
	throw ContextException(x, AnsiToWString(__FUNCTION__), AnsiToWString(__FILE__), __LINE__);\
}

#define ThrowIfFailedDefault(x)\
{\
	ThrowIfFailed(x, Exception);\
}

#define ThrowIfFailedWindow(x)\
{\
    ThrowIfFailed(x, WindowException);\
}

#define ThrowIfFailedDevice(x)\
{\
	ThrowIfFailed(x, DeviceException);\
}

#define ThrowIfFailedCommandQueue(x)\
{\
	ThrowIfFailed(x, CommandQueueException);\
}

#define ThrowIfFailedSwapChain(x)\
{\
	ThrowIfFailed(x, SwapChainException);\
}

#define ThrowIfFailedCommandList(x)\
{\
	ThrowIfFailed(x, CommandListException);\
}

#define ThrowIfFailedUploadBuffer(x)\
{\
	ThrowIfFailed(x, UploadBufferException); \
}

#define ThrowIfFailedCommandAllocator(x)\
{\
	ThrowIfFailed(x, CommandAllocatorException);\
}


#endif
