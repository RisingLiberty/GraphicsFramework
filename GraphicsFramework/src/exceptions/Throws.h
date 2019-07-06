#pragma once


#ifndef ThrowIfFailed
#define ThrowIfFailed(x, exception)\
{\
    HRESULT _hr_ = (x);\
    if(FAILED(_hr_)) { throw exception(_hr_, #x, __FILE__, __LINE__); } \
}
#define ThrowException(x)\
{\
	throw Exception(x, __FUNCTION__, __FILE__, __LINE__);\
}

#define ThrowContextException(x)\
{\
	throw ContextException(x, __FUNCTION__, __FILE__, __LINE__);\
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
