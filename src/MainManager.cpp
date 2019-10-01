#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include "YUVFrame.h"
#include "MainManager.h"
#include "BMPReader.h"

// pathToVideoFile width heigth pathToBmpFIle outputFile
//"C:/Users/Fila4/Desktop/картинки/video352-288.yuv 352 288 C:/Users/Fila4/Desktop/картинки/aptem.yuv C:/Users/Fila4/Desktop/картинки/output.yuv
MainManager::MainManager(int argc, char** argv)
{

	//if (argc != 6)
	//{
	//	std::cout << "invalid arguments";
	//	return;
	//}
	
	//std::string videoFile = argv[1];
	//int width = std::stoi(argv[2]);
	//int height = std::stoi(argv[3]);
	//std::string pictureFile = argv[4];
	//std::string outputFIle = argv[5];

	std::string videoFile = "C:/Users/Fila4/Desktop/картинки/video352-288.yuv";
	int width = 352;
	int height = 288;
	std::string pictureFile = "C:/Users/Fila4/Desktop/картинки/aptem.yuv";
	std::string outputFIle = "C:/Users/Fila4/Desktop/картинки/output12.yuv";


	///////////////// открытие бмп //////////////////////////////
	std::string bmpfile = "C:\\Users\\Fila4\\Desktop\\картинки\\aptem.bmp";
	
	// обернуть в шаред...
	YUVFrame* pictureFrame;
	
	std::thread thr(BMPReader::bmpToYUVFile, bmpfile, &pictureFrame);
	thr.join();

	/////////////////////////// video ////////////////////////////////
	std::ifstream video(videoFile, std::ios::ate, std::ios::binary);
	if (!video.is_open())
		return;
	// получаем размер файла
	size_t size = video.tellg();
	// начало файла
	video.seekg(0, std::ios::beg);

	YUVFrame videoFrame({ width, height });
	int count = (int)(size / videoFrame.getFrameSize());
	//////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////


	std::ofstream outupFile(outputFIle, std::ofstream::binary);
	if (!outupFile.is_open())
		return;

	for (int i = 0; i < count; i++)
	{
		video.read((char*)videoFrame.frame(), videoFrame.getFrameSize());
		outupFile.write((char*)videoFrame.frame(), videoFrame.getFrameSize());
		//outupFile.write((char*)(videoFrame.frame()/* + *pictureFrame*/), videoFrame.getFrameSize());
	}

	outupFile.close();
	video.close();
}


