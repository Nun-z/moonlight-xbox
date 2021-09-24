#pragma once
#include <string>

ref class StreamConfiguration {

public:
	static int getAppID()
	{ return appID; }
	static int getWidth()
	{ return width; }
	static int getHeight()
	{ return height;}
	static int getBitrate()
	{ return bitrate; }
	static int getFps()
	{ return fps; }
	static Platform::String^ getHostname()
	{ return hostname; }

	void setAppID(int givenAppID)
	{ appID = givenAppID; }
	void setWidth(int givenWidth)
	{ width = givenWidth; }
	void setHeight(int givenHeight)
	{ height = givenHeight; }
	void setBitrate(int givenBitrate)
	{ bitrate = givenBitrate; }
	void setHostname(Platform::String^ givenHostname)
	{ hostname = givenHostname; }
	void setFps(int givenFps)
	{ fps = givenFps; }

	Platform::String^ getNextUserVar(Platform::String^ fileContents);
	Platform::String^ getNextSettingsString(Platform::String^ settings);
	void setDefaults();
	void initalizeUserFields();
	Platform::String^ getTextFromFile(Windows::Storage::StorageFile^ givenFile);
	Windows::Storage::StorageFile^ getFile(Platform::String^ filename);
	bool doesFileExist(Platform::String^ filename);

private:
	property static Platform::String^ hostname;
	property static int appID;
	property static int width;
	property static int height;
	property static int bitrate;
	property static int fps;
};
