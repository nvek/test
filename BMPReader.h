#pragma once
#include <string>
#include "YUVFrame.h"

class BMPReader
{

	typedef struct
	{
		unsigned char rgbBlue;
		unsigned char rgbGreen;
		unsigned char rgbRed;
		unsigned char rgbReserved;
	} RgbQuad;

#pragma pack(push, 1)
	typedef struct 
	{
		__int16		bfType;         // 0x4d42 | 0x4349 | 0x5450
		int			bfSize;         // размер файла
		int			bfReserved;     // 0
		int			bfOffBits;      // смещение до поля данных,
									// обычно 54 = 16 + biSize
		int			biSize;         // размер струкуры в байтах:
									// 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER)
									// или 124(BITMAPV5HEADER)
		int			biWidth;		// ширина в точках
		int			biHeight;		// высота в точках
		__int16		biPlanes;       // всегда должно быть 1
		__int16		biBitCount;     // 0 | 1 | 4 | 8 | 16 | 24 | 32
		int			biCompression;  // BI_RGB | BI_RLE8 | BI_RLE4 |
									// BI_BITFIELDS | BI_JPEG | BI_PNG
									// реально используется лишь BI_RGB
		int			biSizeImage;    // Количество байт в поле данных
									// Обычно устанавливается в 0
		int			biXPelsPerMeter;// горизонтальное разрешение, точек на дюйм
		int			biYPelsPerMeter;// вертикальное разрешение, точек на дюйм
		int			biClrUsed;      // Количество используемых цветов
									// (если есть таблица цветов)
		int			biClrImportant; // Количество существенных цветов.
									// Можно считать, просто 0
	} BMPheader;
#pragma pack(pop)


public:
	BMPReader() = default;
	void open(const std::string& fileName);
	YUVFrame toYUV();
	void* toYUVWithSIMD();

private:
	int** _buffer;
	RgbQuad **	_rgb;
	std::string _fileName;
	BMPheader _header;
};
