#include "pch.h"
#include "StreamConfiguration.h"
#include <Utils.hpp>
#include <ppltasks.h>

using namespace moonlight_xbox_dx;
using namespace Windows::Storage;
using namespace Windows::System;

//TODO
Platform::String^ StreamConfiguration::getNextUserVar(Platform::String^ fileContents) {
	std::string givenFileContents = Utils::getTextFromBox(fileContents);
    int endOfFirst = givenFileContents.find_first_of(" ");
    std::string firstVar = givenFileContents.substr(0, endOfFirst);
    fileContents = Utils::getPlatformString(givenFileContents.substr(endOfFirst + 1, givenFileContents.length() - 1));
    return(Utils::getPlatformString(firstVar));
}

void StreamConfiguration::setDefaults() {
    setWidth(1280);
    setHeight(720);
    setFps(30);
    setBitrate(60000);
}

// Really needs refactoring
void StreamConfiguration::initalizeUserFields() {
	if (doesFileExist("settings.txt")) {
		Windows::Storage::StorageFile^ settingsFile = getFile("settings.txt");
		try {
			Platform::String^ settings = getTextFromFile(settingsFile);

			std::string givenFileContents0 = Utils::getTextFromBox(settings);
			int endOfFirst0 = givenFileContents0.find_first_of(" ");
			std::string firstVar0 = givenFileContents0.substr(0, endOfFirst0);
			settings = Utils::getPlatformString(givenFileContents0.substr(endOfFirst0 + 1, givenFileContents0.length() - 1));
			setHostname(Utils::getPlatformString(firstVar0));

			std::string givenFileContents1 = Utils::getTextFromBox(settings);
			int endOfFirst1 = givenFileContents1.find_first_of(" ");
			std::string firstVar1 = givenFileContents1.substr(0, endOfFirst1);
			settings = Utils::getPlatformString(givenFileContents1.substr(endOfFirst1 + 1, givenFileContents1.length() - 1));
			setWidth(stoi(firstVar1));

			std::string givenFileContents2 = Utils::getTextFromBox(settings);
			int endOfFirst2 = givenFileContents2.find_first_of(" ");
			std::string firstVar2 = givenFileContents2.substr(0, endOfFirst2);
			settings = Utils::getPlatformString(givenFileContents2.substr(endOfFirst2 + 1, givenFileContents2.length() - 1));
			setHeight(stoi(firstVar2));

			std::string givenFileContents3 = Utils::getTextFromBox(settings);
			int endOfFirst3 = givenFileContents3.find_first_of(" ");
			std::string firstVar3 = givenFileContents3.substr(0, endOfFirst3);
			settings = Utils::getPlatformString(givenFileContents3.substr(endOfFirst3 + 1, givenFileContents3.length() - 1));
			setFps(stoi(firstVar3));

			std::string givenFileContents4 = Utils::getTextFromBox(settings);
			int endOfFirst4 = givenFileContents4.find_first_of(" ");
			std::string firstVar4 = givenFileContents4.substr(0, endOfFirst4);
			settings = Utils::getPlatformString(givenFileContents4.substr(endOfFirst4 + 1, givenFileContents4.length() - 1));
			setBitrate(stoi(firstVar4));
		}
		catch (std::exception e) {
			OutputDebugString(L"Formatting error(s) detected.  Aborting load.");
			setDefaults();
		}
	}
	else {
		setDefaults();
	}
}

Platform::String^ StreamConfiguration::getTextFromFile(Windows::Storage::StorageFile^ givenFile) {
	auto getTextTask = concurrency::create_task(Windows::Storage::FileIO::ReadTextAsync(givenFile))
		.then([](Platform::String^ returnedText) {
		return(Utils::getTextFromBox(returnedText));
			});

	return(Utils::getPlatformString(getTextTask.get()));
}

Windows::Storage::StorageFile^ StreamConfiguration::getFile(Platform::String^ filename) {
	Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto getFileTask = concurrency::create_task(
		localFolder->GetFileAsync("settings.txt"))
		.then([](StorageFile^ returnedFile) {
		return(returnedFile);
			});

	return(getFileTask.get());
}

bool StreamConfiguration::doesFileExist(Platform::String^ filename) {
	Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto checkFileTask = concurrency::create_task(
		localFolder->TryGetItemAsync(filename))
		.then([](IStorageItem^ test) {
		if (test)
		{
			return(true);
		}
		else
		{
			return(false);
		}
			});

	return(checkFileTask.get());
}
