#pragma once
#include <string>

ref class StreamConfiguration {

public:

	int getAppID()
	{ return appID; }
	int getWidth()
	{ return width; }
	int getHeight()
	{ return height;}
	int getBitrate()
	{ return bitrate; }
	int getFps()
	{ return fps; }
	Platform::String^ getHostname()
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
	void setDefaults();
	void initalizeUserFields();
	Platform::String^ getTextFromFile(Windows::Storage::StorageFile^ givenFile);
	Windows::Storage::StorageFile^ getFile(Platform::String^ filename);
	bool doesFileExist(Platform::String^ filename);

private:
	property Platform::String^ hostname;
	property int appID;
	property int width;
	property int height;
	property int bitrate;
	property int fps;
};
