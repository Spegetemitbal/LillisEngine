#include "System_Emscripten.h"

using namespace GPR460;

class System_Emscripten : public System
{
public:
	void Init() override;
	void Shutdown() override;
	void ShowError(const string& message) override;
	void LogToErrorFile(const string& message) override;
private:
	//HANDLE gameErrors = INVALID_HANDLE_VALUE;
};

void System_Emscripten::Init()
{

}

void System_Emscripten::Shutdown()
{

}

void System_Emscripten::LogToErrorFile(const string& message)
{
	EM_ASM({ console.log($0); }, message.c_str());
}

void System_Emscripten::ShowError(const string& message)
{
	EM_ASM({ alert($0); }, message.c_str());
}

System* System::Create()
{
	return new System_Emscripten();
}