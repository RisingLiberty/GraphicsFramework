#include "stdafx.h"
#include "Exceptions.h"

#include <comdef.h>

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

	ss << m_function_name << L" failed in " << m_file_name << L" on line " << m_line_nr << L"\n";
	ss << L"error: " << msg << L"\n";

	m_error_message = ss.str();
}

Exception::Exception(const std::wstring& errorMessage, const std::wstring& functionName, const std::wstring& fileName, int lineNumber):
	m_error_code(S_FALSE),
	m_function_name(functionName),
	m_file_name(fileName),
	m_line_nr(lineNumber),
	m_error_message(errorMessage)
{

}

Exception::Exception(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber) :
	m_error_code(hr),
	m_function_name(functionName),
	m_file_name(fileName),
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
className::className(const std::wstring& errorMessage, const std::wstring& functionName, const std::wstring& fileName, int lineNumber) :\
	Exception(errorMessage, functionName, fileName, lineNumber)\
{\
\
}\
\
className::className(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber) :\
	Exception(hr, functionName, fileName, lineNumber)\
{\
\
}\
\
std::wstring className::ToString() const\
{\
	std::wstringstream ss;\
\
	ss << "---" << message << "---\n" << Exception::ToString();\
\
	return ss.str();\
}\


CreateException(DefaultException, "Default exception");
CreateException(WindowException, "Window exception");
CreateException(DeviceException, "Device exception");
CreateException(CommandQueueException, "Command Queue exception");
CreateException(SwapChainException, "Swapchain exception");
CreateException(CommandListException, "Command List Exception");
CreateException(UploadBufferException, "Upload Buffer Exception");
CreateException(CommandAllocatorException, "Command Allocator Exception");
CreateException(ContextException, "Context Exception");