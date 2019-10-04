#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "YUVFrame.h"
#include "MainManager.h"
#include "BMPReader.h"
#include "YUVVideo.h"

// pathToVideoFile width heigth pathToBmpFIle outputFile
//"C:/Users/Fila4/Desktop/картинки/video352-288.yuv 352 288 C:/Users/Fila4/Desktop/картинки/aptem.yuv C:/Users/Fila4/Desktop/картинки/output.yuv
MainManager::MainManager(int argc, char** argv)
{
	// парсим аргументы командной строки
	parseCommandLine(argc, argv);

	// конвертим бмп
	bmpConvertToYuv();

	// создаем фрейм для видео
	YUVVIdeo inputVideo(_sourceVideoPath, { _width, _height });

	std::ofstream outupFile(_outputVideoPath, std::ios::binary | std::ios::out);
	if (!outupFile.is_open())
		return;

	for (int i = 0; i < inputVideo.getCountFrame(); i++)
	{
		outupFile.write((char*)(inputVideo + *_pictureFrame), inputVideo.getFrameSize());
	}

	outupFile.close();
}

MainManager::~MainManager()
{
	delete _pictureFrame;
}

void MainManager::parseCommandLine(int argc, char** argv)
{
	if (argc != 6)
	{
		std::cout << "invalid arguments";
		return;
	}

	std::string _sourceVideoPath = argv[1];
	_width = std::stoi(argv[2]);
	_height = std::stoi(argv[3]);
	std::string _pictureBmpPath = argv[4];
	std::string _outputVideoPath = argv[5];

//	_sourceVideoPath = "C:/Users/Fila4/Desktop/картинки/video.yuv";
//	_outputVideoPath = "C:/Users/Fila4/Desktop/картинки/output.yuv";
//	_pictureBmpPath = "C:/Users/Fila4/Desktop/картинки/aptem.bmp";
//	_width = 352;
//	_height = 288;
}

void MainManager::bmpConvertToYuv()
{
	std::thread thr(BMPReader::bmpToYUVFile, _pictureBmpPath, &_pictureFrame);
	thr.join();
}
