#pragma once

class Exception
{
public:
	Exception();
	Exception(const std::wstring& errorMessage, const std::wstring& functionName, const std::wstring& fileName, int lineNumber);
	Exception(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber);

	virtual std::wstring ToString() const;

private:
	HRESULT m_error_code;
	std::wstring m_function_name;
	std::wstring m_file_name;
	int m_line_nr;
	std::wstring m_error_message;
};

#define DeclareException(className)\
class className : public Exception\
{\
public:\
	className();\
	className(const std::wstring& errorMessage, const std::wstring& functionName, const std::wstring& fileName, int lineNumber);\
	className(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber);\
\
	virtual std::wstring ToString() const override;\
};\

DeclareException(DefaultException);
DeclareException(WindowException);
DeclareException(DeviceException);
DeclareException(CommandQueueException);
DeclareException(SwapChainException);
DeclareException(CommandListException);
DeclareException(UploadBufferException);
DeclareException(CommandAllocatorException);
DeclareException(ContextException);