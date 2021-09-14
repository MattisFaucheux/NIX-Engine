#include "EditorApp.h"
#include "Log/Logger.h"
#include "Windowing/Window.h"

#include <time.h>


#include "EditorConsoleLogger.h"
#include "FileLogger.h"
#include "Logger/ConsoleLogger.h"


std::string GetLogFileName()
{
	time_t rawTime = time(nullptr);
	struct tm timeInfo;

	char buffer[80];

	time(&rawTime);
	localtime_s(&timeInfo, &rawTime);
	strftime(buffer, 80, "_%Y%m%d_%H-%M-%S", &timeInfo);
	
	std::string logFileName = "Log\\Log";
	logFileName += buffer;
	logFileName += ".log";
	
	return logFileName;
}

int main()
{
	using namespace NIX;
	
	Core::Logger logger;

#ifndef _SHIPPING
	Core::Logger::GetLogger()->AddClient(new Editor::ConsoleLogger());
	Core::Logger::GetLogger()->AddClient(new Editor::EditorConsoleLogger());
#endif
	
	Core::Logger::GetLogger()->AddClient(new Editor::FileLogger(GetLogFileName().c_str()));

	
	Rendering::Window::InitGlfw();
	Editor::EditorApp App;

	App.Run();
}

