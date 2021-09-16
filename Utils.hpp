#pragma once
#include "pch.h"

namespace moonlight_xbox_dx {
	namespace Utils {
		extern std::vector<std::wstring> logLines;
		extern bool showLogs;
		Platform::String^ StringPrintf(const char* fmt, ...);

		void Log(const char* fmt);

		std::vector<std::wstring> GetLogLines();

		Platform::String^ getPlatformString(std::string input);

		std::string getTextFromBox(Platform::String^ boxContents);
	}
}