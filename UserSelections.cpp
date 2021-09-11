#include "pch.h"
#include "UserSelections.h"
#include "Utilities.h"
#include <ppltasks.h>
#include <string>
#include <iostream>

using namespace moonlight_xbox_dx;

using namespace Windows::Storage;
using namespace Windows::System;

std::string UserSelections::ipAddress;
int UserSelections::width;
int UserSelections::height;
int UserSelections::fps;
int UserSelections::bitrate;

std::string getNextUserVar(std::string& fileContents);
std::string initializeUserFields();
static std::string getTextFromFile(Windows::Storage::StorageFile^ givenFile);
static Windows::Storage::StorageFile^ getFile(std::string filename);
bool doesFileExist(std::string filename);

std::string getNextUserVar(std::string &fileContents) {
	int endOfFirst = fileContents.find_first_of(" ");
	std::string firstVar = fileContents.substr(0, endOfFirst);
	fileContents = fileContents.substr(endOfFirst + 1, fileContents.length() - 1);
	return(firstVar);
}

void UserSelections::initalizeUserFields() {
	if (doesFileExist("settings.txt")) {
		Windows::Storage::StorageFile^ settingsFile = getFile("settings.txt");
		std::string settings = getTextFromFile(settingsFile);

		UserSelections::setIpAddress(getNextUserVar(settings));
		UserSelections::setWidth(stoi(getNextUserVar(settings)));
		UserSelections::setHeight(stoi(getNextUserVar(settings)));
		UserSelections::setFps(stoi(getNextUserVar(settings)));
		UserSelections::setBitrate(stoi(getNextUserVar(settings)));
	}
	else {
		UserSelections::setWidth(1280);
		UserSelections::setHeight(720);
		UserSelections::setFps(30);
		UserSelections::setBitrate(60000);
	}
}

static std::string getTextFromFile(Windows::Storage::StorageFile^ givenFile) {
	auto getTextTask = concurrency::create_task(Windows::Storage::FileIO::ReadTextAsync(givenFile))
		.then([](Platform::String^ returnedText) {
		return(Utilities::getTextFromBox(returnedText));
			});

	getTextTask.wait();
	return(getTextTask.get());
}

static Windows::Storage::StorageFile^ getFile(std::string filename) {
	Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto getFileTask = concurrency::create_task(
		localFolder->GetFileAsync("settings.txt"))
		.then([](StorageFile^ returnedFile) {
		return(returnedFile);
			});

	getFileTask.wait();
	return(getFileTask.get());
}

// Working
static bool doesFileExist(std::string filename) {
	Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto checkFileTask = concurrency::create_task(
		localFolder->TryGetItemAsync(Utilities::getPlatformString(filename)))
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

	checkFileTask.wait();
	return(checkFileTask.get());
}