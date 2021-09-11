#pragma once
class Utilities
{
public:
	static std::string getTextFromBox(Platform::String^ boxContents);
	static Platform::String^ getPlatformString(std::string input);
};