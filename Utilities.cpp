#include "pch.h"
#include "Utilities.h"

std::string Utilities::getTextFromBox(Platform::String^ boxContents) {
	std::wstring wideString(boxContents->Data());
	std::string rawString(wideString.begin(), wideString.end());
	return(rawString);
}

Platform::String^ Utilities::getPlatformString(std::string input) {
	std::wstring wideInput = std::wstring(input.begin(), input.end());
	const wchar_t* charInput = wideInput.c_str();
	return(ref new Platform::String(charInput));
}