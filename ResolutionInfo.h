#pragma once
class ResolutionInfo
{
public:
	
	static int getWidth() { return width; }
	static int getHeight() { return height; }

	static void setWidth(int number) { width = number; }
	static void setHeight(int number) { height = number; }

	ResolutionInfo(int givenWidth, int givenHeight)
	{
		setWidth(givenWidth);
		setHeight(givenHeight);
	}

private:
	static int width;
	static int height;
};

