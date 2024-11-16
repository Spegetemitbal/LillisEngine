#include "../pch.h"
#pragma once

namespace LILLIS
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
