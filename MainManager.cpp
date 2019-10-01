#include "pch.h"

#include <fstream>
#include <iostream>
#include <string>
#include "YUVFrame.h"
#include "MainManager.h"
#include "BMPReader.h"

// pathToVideoFile width heigth pathToBmpFIle outputFile
//"C:/Users/Fila4/Desktop/картинки/video352-288.yuv 352 288 C:/Users/Fila4/Desktop/картинки/aptem.yuv C:/Users/Fila4/Desktop/картинки/output.yuv
MainManager::MainManager(int argc, char** argv)
{

	if (argc != 6)
	{
		std::cout << "invalid arguments";
		return;
	}

	std::string videoFile = argv[1];
	int width = std::stoi(argv[2]);
	int height = std::stoi(argv[3]);
	std::string pictureFile = argv[4];
	std::string outputFIle = argv[5];

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

	///////////////// открытие бмп //////////////////////////////
	BMPReader bmpFile;
	std::string bmpfile = "C:\\Users\\Fila4\\Desktop\\картинки\\aptem.bmp";
	bmpFile.open(bmpfile);
	YUVFrame pictureFrame( bmpFile.toYUV() );
	int a = 0;
	a++;
	std::ofstream outupFileP("C:\\Users\\Fila4\\Desktop\\картинки\\aptem1111.yuv", std::ofstream::binary);
	outupFileP.write((char*)pictureFrame.frame(), pictureFrame.getFrameSize());

	//////////////////////////////////////////////////////////


	std::ofstream outupFile(outputFIle, std::ofstream::binary);
	if (!outupFile.is_open())
		return;

	for (int i = 0; i < count; i++)
	{
		video.read((char*)videoFrame.frame(), videoFrame.getFrameSize());
		outupFile.write((char*)(videoFrame + pictureFrame), videoFrame.getFrameSize());
	}

	outupFile.close();
	//bmpFile.close();
	video.close();
}


