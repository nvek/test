#include "pch.h"

#include "BMPReader.h"
#include <vector>
#include <fstream>

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

void BMPReader::open(const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::binary);

	file.read((char*)&_header, sizeof(_header));
		
	_rgb = new RgbQuad*[_header.biHeight];
	for (int i = 0; i < _header.biHeight; i++)
	{
		_rgb[i] = new RgbQuad[_header.biWidth];
	}

	for (int i = 0; i < _header.biHeight; i++)
	{
		for (int j = 0; j < _header.biWidth; j++)
		{
			file.read((char*)&_rgb[i][j].rgbBlue, 1);
			file.read((char*)&_rgb[i][j].rgbGreen, 1);
			file.read((char*)&_rgb[i][j].rgbRed, 1);
		}

		// пропускаем последний байт в строке
				char t;
			file.read(&t, 1);
	
	}
	file.close();
}

YUVFrame BMPReader::toYUV()
{
	YUVFrame* yuvFrame = new YUVFrame({ _header.biWidth, _header.biHeight });
	unsigned char* frame = yuvFrame->frame();

	int yIndex = 0;
	int uIndex = _header.biWidth * _header.biHeight;
	int vIndex = yuvFrame->getFrameSizeY() + yuvFrame->getFrameSizeUV();

	for (int i = 0; i < _header.biHeight; i++)
	{
		for (int j = 0; j < _header.biWidth; j++)
		{
			unsigned char R = _rgb[i][j].rgbRed;
			unsigned char G = _rgb[i][j].rgbGreen;
			unsigned char B = _rgb[i][j].rgbBlue;

			// well known RGB to YUV algorithm
			int y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
			int u = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
			int v = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

			frame[yIndex++] = y;
			if (j % 2 == 0 && i % 2 == 0) 
			{
				frame[uIndex ++] = u;
				frame[vIndex ++] = v;
			}
		}
	}
	return *yuvFrame;
}
