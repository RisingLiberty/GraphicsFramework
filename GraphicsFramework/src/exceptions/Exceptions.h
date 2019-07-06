#pragma once

class Exception
{
public:
	Exception();
	Exception(const std::string& errorMessage, const std::string& functionName, const std::string& fileName, int lineNumber);
	Exception(HRESULT hr, const std::string& functionName, const std::string& fileName, int lineNumber);

	virtual std::wstring ToString() const;

private:
	HRESULT m_error_code;
	std::wstring m_function_name;
	std::wstring m_file_name;
	std::wstring m_error_message;
	int m_line_nr;
};

#define DeclareException(className)\
class className : public Exception\
{\
public:\
	className();\
	className(const std::string& errorMessage, const std::string& functionName, const std::string& fileName, int lineNumber);\
	className(HRESULT hr, const std::string& functionName, const std::string& fileName, int lineNumber);\
\
	virtual std::wstring ToString() const override;\
};

DeclareException(DefaultException);
DeclareException(WindowException);
DeclareException(DeviceException);
DeclareException(CommandQueueException);
DeclareException(SwapChainException);
DeclareException(CommandListException);
DeclareException(UploadBufferException);
DeclareException(CommandAllocatorException);
DeclareException(ContextException);