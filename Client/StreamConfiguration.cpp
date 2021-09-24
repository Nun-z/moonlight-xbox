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

Platform::String^ StreamConfiguration::getNextSettingsString(Platform::String^ settings) {
	int endOfFirst = Utils::getTextFromBox(settings).find_first_of(" ");
	std::string firstVar = Utils::getTextFromBox(settings).substr(0, endOfFirst);
	Platform::String^ newSettings = Utils::getPlatformString(
		Utils::getTextFromBox(settings).substr(endOfFirst + 1, Utils::getTextFromBox(settings).length() - 1));
	return(newSettings);
}

void StreamConfiguration::setDefaults() {
    setWidth(1280);
    setHeight(720);
    setFps(30);
    setBitrate(8000);
}

// Really needs refactoring
void StreamConfiguration::initalizeUserFields() {
	if (doesFileExist("settings.txt")) {
		Windows::Storage::StorageFile^ settingsFile = getFile("settings.txt");
		try {
			Platform::String^ settings = getTextFromFile(settingsFile);

			setHostname(getNextUserVar(settings));
			settings = getNextSettingsString(settings);
			setWidth(stoi(Utils::getTextFromBox(getNextUserVar(settings))));
			settings = getNextSettingsString(settings);
			setHeight(stoi(Utils::getTextFromBox(getNextUserVar(settings))));
			settings = getNextSettingsString(settings);
			setFps(stoi(Utils::getTextFromBox(getNextUserVar(settings))));
			settings = getNextSettingsString(settings);
			setBitrate(stoi(Utils::getTextFromBox(getNextUserVar(settings))));
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
