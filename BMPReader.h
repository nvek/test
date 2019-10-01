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
		int			bfSize;         // ������ �����
		int			bfReserved;     // 0
		int			bfOffBits;      // �������� �� ���� ������,
									// ������ 54 = 16 + biSize
		int			biSize;         // ������ �������� � ������:
									// 40(BITMAPINFOHEADER) ��� 108(BITMAPV4HEADER)
									// ��� 124(BITMAPV5HEADER)
		int			biWidth;		// ������ � ������
		int			biHeight;		// ������ � ������
		__int16		biPlanes;       // ������ ������ ���� 1
		__int16		biBitCount;     // 0 | 1 | 4 | 8 | 16 | 24 | 32
		int			biCompression;  // BI_RGB | BI_RLE8 | BI_RLE4 |
									// BI_BITFIELDS | BI_JPEG | BI_PNG
									// ������� ������������ ���� BI_RGB
		int			biSizeImage;    // ���������� ���� � ���� ������
									// ������ ��������������� � 0
		int			biXPelsPerMeter;// �������������� ����������, ����� �� ����
		int			biYPelsPerMeter;// ������������ ����������, ����� �� ����
		int			biClrUsed;      // ���������� ������������ ������
									// (���� ���� ������� ������)
		int			biClrImportant; // ���������� ������������ ������.
									// ����� �������, ������ 0
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
