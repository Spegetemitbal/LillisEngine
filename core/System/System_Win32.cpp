#include "pch.h"
#include "System_Win32.h"

using namespace LILLIS;

class System_Win32 : public System
{
public:
	void Init() override;
	void Shutdown() override;
	void ShowError(const string& message) override;
	void LogToErrorFile(const string& message) override;
private:
	HANDLE gameErrors = INVALID_HANDLE_VALUE;
};

void System_Win32::Init()
{

	if (!AllocConsole())
	{
		//ShowError(L"Failed to Alloc Console");
		// ExitProcess(1);
	}

	//do something with this later.
	//freopen("CONOUT$", "w", stdout);

}

void System_Win32::Shutdown()
{
	if (gameErrors != INVALID_HANDLE_VALUE)
	{
		CloseHandle(gameErrors);
	}
	//_CrtDumpMemoryLeaks();
	//Something is going goofy with this...
}

void System_Win32::LogToErrorFile(const string& message)
{
	if (gameErrors == INVALID_HANDLE_VALUE)
	{
		gameErrors = CreateFileW(L"GameErrors.txt", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	//value-type is crossplatform
	//Windows uses UNICODE 16. Linux uses UNICODE 8.
	//The sizeof is specifically to get the size of the string, entirely independent of what string type (ASCII or Unicode)
	WriteFile(gameErrors, message.c_str(), message.size() * sizeof(string::value_type), 0, NULL);
}

void System_Win32::ShowError(const string& message)
{
	//MessageBox(NULL, message.c_str(), L"Error", MB_OK | MB_ICONERROR);
}

System* System::Create()
{
	return DBG_NEW System_Win32();
}

