#pragma once
class UserSelections
{
public:
	
	static int getWidth() { return width; }
	static int getHeight() { return height; }
	static int getFps() { return fps; }
	static int getBitrate() { return bitrate; }

	static void setWidth(int number) { width = number; }
	static void setHeight(int number) { height = number; }
	static void setFps(int number) { fps = number; }
	static void setBitrate(int number) { bitrate = number; }

private:
	static int width;
	static int height;
	static int fps;
	static int bitrate;
};