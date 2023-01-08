#pragma once
#include <string>

#ifdef _DEBUG
#ifdef _WIN32
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#endif
#else
#define DBG_NEW new
#endif

namespace GPR460
{
#ifdef _WIN32
	typedef std::wstring string;
#else
	typedef std::string string;
#endif
	//string is automatically UTF8 (unicode 8) on other platforms, on windows wstring is automatically UTF16
	//Normal strings in windows are just ASCII.

	class System
	{
	public:
		virtual ~System() {}

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void ShowError(const string& message) = 0;
		virtual void LogToErrorFile(const string& message) = 0;

		static System* Create();
		//Polymorphism like this is slightly slower than just doing preprocessors to handle platform specific variables.
	private:

	};
}
