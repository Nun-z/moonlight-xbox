#pragma once
class UserSelections
{
public:
	
	static int getWidth() { return width; }
	static int getHeight() { return height; }
	static int getFps() { return fps; }
	static int getBitrate() { return bitrate; }
	static std::string getIpAddress() { return ipAddress; }

	static void setWidth(int number) { width = number; }
	static void setHeight(int number) { height = number; }
	static void setFps(int number) { fps = number; }
	static void setBitrate(int number) { bitrate = number; }
	static void setIpAddress(std::string ip) { ipAddress = ip; }

	static void initalizeUserFields();

private:
	static int width;
	static int height;
	static int fps;
	static int bitrate;
	static std::string ipAddress;
};