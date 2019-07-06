#include "stdafx.h"
#include "Exceptions.h"

#include <comdef.h>

namespace
{
	inline std::wstring AnsiToWideString(const std::string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
	}
}

Exception::Exception() :
	m_error_code(S_FALSE),
	m_function_name(L""),
	m_file_name(L""),
	m_line_nr(-1),
	m_error_message()
{
	// Get the string description of the error code
	_com_error err(m_error_code);
	std::wstring msg = err.ErrorMessage();
	std::wstringstream ss;
	std::wstring wide_function_name(m_function_name.begin(), m_function_name.end());
	std::wstring wide_file_name(m_file_name.begin(), m_file_name.end());

	ss << wide_function_name << L" failed in " << wide_file_name << L" on line " << m_line_nr << L"\n";
	ss << "error: " << msg << "\n";
	m_error_message = ss.str();
}

Exception::Exception(const std::string& errorMessage, const std::string& functionName, const std::string& fileName, int lineNumber) :
	m_error_code(S_FALSE),
	m_function_name(AnsiToWideString(functionName)),
	m_file_name(AnsiToWideString(fileName)),
	m_line_nr(lineNumber),
	m_error_message(AnsiToWideString(errorMessage))
{

}

Exception::Exception(HRESULT hr, const std::string& functionName, const std::string& fileName, int lineNumber) :
	m_error_code(hr),
	m_function_name(AnsiToWideString(functionName)),
	m_file_name(AnsiToWideString(fileName)),
	m_line_nr(lineNumber)
{

}

std::wstring Exception::ToString() const
{
	return m_error_message;
}


#define CreateException(className, message)\
className::className() :\
Exception()\
{\
\
}\
\
className::className(const std::string& errorMessage, const std::string& functionName, const std::string& fileName, int lineNumber) :\
	Exception(errorMessage, functionName, fileName, lineNumber)\
{\
\
}\
\
className::className(HRESULT hr, const std::string& functionName, const std::string& fileName, int lineNumber) :\
	Exception(hr, functionName, fileName, lineNumber)\
{\
\
}\
\
std::wstring className::ToString() const\
{\
	std::wstringstream ss;\
\
	ss << L"---" << message << L"---\n" << Exception::ToString();\
\
	return ss.str();\
}

CreateException(DefaultException, "Default exception");
CreateException(WindowException, "Window exception");
CreateException(DeviceException, "Device exception");
CreateException(CommandQueueException, "Command Queue exception");
CreateException(SwapChainException, "Swapchain exception");
CreateException(CommandListException, "Command List Exception");
CreateException(UploadBufferException, "Upload Buffer Exception");
CreateException(CommandAllocatorException, "Command Allocator Exception");
CreateException(ContextException, "Context Exception");