#pragma once
class Utilities
{
public:
	static std::string getTextFromBox(Platform::String^ boxContents);
	static void setTextboxText(std::string input, Platform::String^ textBox);
	static Platform::String^ getPlatformString(std::string input);
};