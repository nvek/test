#include <vector>
#include <fstream>
#include <iostream>
#include <exception>

#include "BMPReader.h"
#include "YUVFrame.h"
#include "immintrin.h"
#include "getCPUTime.c"


const float RRGB24YUV_00 = 0.299f;
const float RRGB24YUV_01 = 0.587f;
const float RRGB24YUV_02 = 0.114f;
const float RRGB24YUV_10 = -0.147f;
const float RRGB24YUV_11 = -0.289f;
const float RRGB24YUV_12 = 0.436f;
const float RRGB24YUV_20 = 0.615f;
const float RRGB24YUV_21 = -0.515f;
const float RRGB24YUV_22 = -0.100f;

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// read bytes
template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size)
{
	fp.read(reinterpret_cast<char*>(&result), size);
}

unsigned char bitextract(const unsigned int byte, const unsigned int mask)
{
	if (mask == 0)
		return 0;

	// определение количества нулевых бит справа от маски
	unsigned int maskBufer = mask;
	int	maskPadding = 0;

	while (!(maskBufer & 1)) 
	{
		maskBufer >>= 1;
		maskPadding++;
	}

	// применение маски и смещение
	return (byte & mask) >> maskPadding;
}

void BMPReader::bmpToYUVFile(const std::string& fileName, YUVFrame** yuvFrame)
{
	std::ifstream fileStream(fileName, std::ifstream::binary);


	// заголовк изображения
	BITMAPFILEHEADER fileHeader;

	fileStream.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
	if (fileHeader.bfType != 0x4D42) 
	{
		std::string mes = "Error: '" + fileName + "' is not BMP file.";
		throw std::runtime_error(mes.c_str());
	}

	// информация изображения
	BITMAPINFOHEADER fileInfoHeader;
	read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));

	// bmp core
	if (fileInfoHeader.biSize >= 12)
	{
		read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
		read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
		read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
		read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	}

	// получаем информацию о битности
	int colorsCount = fileInfoHeader.biBitCount >> 3;
	if (colorsCount < 3)
	{
		colorsCount = 3;
	}

	int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
	int maskValue = (1 << bitsOnColor) - 1;

	// bmp v1
	if (fileInfoHeader.biSize >= 40)
	{
		read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
		read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
		read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
		read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
		read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
		read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
	}

	// bmp v2
	fileInfoHeader.biRedMask = 0;
	fileInfoHeader.biGreenMask = 0;
	fileInfoHeader.biBlueMask = 0;

	if (fileInfoHeader.biSize >= 52)
	{
		read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
		read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
		read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
	}

	// если маска не задана, то ставим маску по умолчанию
	if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0)
	{
		fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
		fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
		fileInfoHeader.biBlueMask = maskValue;
	}

	// bmp v3
	if (fileInfoHeader.biSize >= 56)
	{
		read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
	}
	else
	{
		fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
	}

	// bmp v4
	if (fileInfoHeader.biSize >= 108)
	{
		read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
		read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
		read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
		read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
		read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
	}

	// bmp v5
	if (fileInfoHeader.biSize >= 124)
	{
		read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
		read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
		read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
		read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
	}

	// проверка на поддерку этой версии формата
	if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
		fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124)
	{
		throw std::runtime_error("Error: Unsupported BMP format.");
	}

	if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) 
	{
		throw std::runtime_error("Error: Unsupported BMP bit count.");
	}

	if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3)
	{
		throw std::runtime_error("Error: Unsupported BMP compression.");
	}

	// определение размера отступа в конце каждой строки
	int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;

	unsigned int bufer;
	
	YUVFrame::Size pictureSize = { static_cast<int>(fileInfoHeader.biWidth) ,  static_cast<int>(fileInfoHeader.biHeight) };
	*yuvFrame = new YUVFrame(pictureSize);
	unsigned char*	frame = (*yuvFrame)->frame();
	
	struct RGBQUAD {
		unsigned char  rgbBlue;
		unsigned char  rgbGreen;
		unsigned char  rgbRed;
		unsigned char  rgbReserved;
	};

	double start, end;

	start = getCPUTime();

	// rgb info
	RGBQUAD **rgbInfo = new RGBQUAD*[fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++)
		rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++)
	{
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++)
		{
			read(fileStream, bufer, fileInfoHeader.biBitCount / 8);

			rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
			rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
			rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
			rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);

		}
		fileStream.seekg(linePadding, std::ios_base::cur);
	}
	fileStream.close();

	// переворачиваем
	for (unsigned int i = 0; i < fileInfoHeader.biHeight / 2; ++i)
	{
		std::swap(rgbInfo[i], rgbInfo[fileInfoHeader.biHeight - i - 1]);
	}

	__m128 val00 = _mm_set1_ps (RRGB24YUV_00);
	__m128 val01 = _mm_set1_ps (RRGB24YUV_01);
	__m128 val02 = _mm_set1_ps (RRGB24YUV_02);
	__m128 val10 = _mm_set1_ps (RRGB24YUV_10);
	__m128 val11 = _mm_set1_ps (RRGB24YUV_11);
	__m128 val12 = _mm_set1_ps (RRGB24YUV_12);
	__m128 val20 = _mm_set1_ps (RRGB24YUV_20);
	__m128 val21 = _mm_set1_ps (RRGB24YUV_21);
	__m128 val22 = _mm_set1_ps (RRGB24YUV_22);

	unsigned int size = fileInfoHeader.biHeight * fileInfoHeader.biWidth;
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++)
	{
		for (size_t j = 0; j < fileInfoHeader.biWidth; j +=4)
		{

			__m128 sseArrR = _mm_setr_ps(static_cast<float>(rgbInfo[i][3].rgbRed),
					static_cast<float>(rgbInfo[i][2].rgbRed),
					static_cast<float>(rgbInfo[i][1].rgbRed),
					static_cast<float>(rgbInfo[i][0].rgbRed));

			__m128 sseArrG = _mm_setr_ps(static_cast<float>(rgbInfo[i][3].rgbGreen),
					static_cast<float>(rgbInfo[i][2].rgbGreen),
					static_cast<float>(rgbInfo[i][1].rgbGreen),
					static_cast<float>(rgbInfo[i][0].rgbGreen));

			__m128 sseArrB = _mm_setr_ps(static_cast<float>(rgbInfo[i][3].rgbBlue),
					static_cast<float>(rgbInfo[i][2].rgbBlue),
					static_cast<float>(rgbInfo[i][1].rgbBlue),
					static_cast<float>(rgbInfo[i][0].rgbBlue));

			__m128 yr = _mm_mul_ps(sseArrR, val00);
			__m128 yg = _mm_mul_ps(sseArrG, val01);
			__m128 yb = _mm_mul_ps(sseArrB, val02);

			__m128 ur = _mm_mul_ps(sseArrR, val10);
			__m128 ug = _mm_mul_ps(sseArrG, val11);
			__m128 ub = _mm_mul_ps(sseArrB, val12);

			__m128 vr = _mm_mul_ps(sseArrR, val20);
			__m128 vg = _mm_mul_ps(sseArrG, val21);
			__m128 vb = _mm_mul_ps(sseArrB, val22);

			__m128 Y = _mm_add_ps(_mm_add_ps(yr, yg), yb);
			__m128 U = _mm_add_ps(_mm_add_ps(ur, ug), ub);
			__m128 V = _mm_add_ps(_mm_add_ps(vr, vg), vb);


			frame[i * fileInfoHeader.biWidth + (j)] = Y[0];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j) / 2) + size] = U[0];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j) / 2) + size + (size / 4)] = V[0];

			frame[i * fileInfoHeader.biWidth + (j + 1)] = Y[1];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j + 1) / 2) + size] = U[1];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j + 1) / 2) + size + (size / 4)] = V[1];

			frame[i * fileInfoHeader.biWidth + (j + 2)] = Y[2];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j + 2) / 2) + size] = U[2];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j + 2) / 2) + size + (size / 4)] = V[2];

			frame[i * fileInfoHeader.biWidth + (j + 3)] = Y[3];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j + 3) / 2) + size] = U[3];
			frame[(i / 2) * (fileInfoHeader.biWidth / 2) + ((j + 3) / 2) + size + (size / 4)] = V[3];

		}
	}

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++)
		delete[] rgbInfo[i];
	delete[] rgbInfo;

	end = getCPUTime();

	std::cout << "итого что мы имеем: " << end - start; //1) 0,031 ~ 0.046
}
