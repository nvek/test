#pragma once
#include <string>

class YUVFrame;
class MainManager
{
public:
	MainManager(int argc, char** argv);
	~MainManager();

private:
	// from command line
	std::string _sourceVideoPath;
	std::string _outputVideoPath;
	std::string _pictureBmpPath;
	int _width;
	int _height;

	void parseCommandLine(int argc, char** argv);
	void bmpConvertToYuv();
	YUVFrame* _pictureFrame;
};

