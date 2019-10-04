#include "YUVFrame.h"

YUVFrame::YUVFrame(const Size& size) :
_size(size)
{
	_frame_size_y = size.w * size.h;
	_frame_size_uv = (size.w >> 1) * (size.h >> 1);
	_frame_size = _frame_size_y + 2 * _frame_size_uv;
	
	_frame = new unsigned char[_frame_size];
	_yuv.y = &_frame[0];
	_yuv.u = &_frame[_frame_size_y];
	_yuv.v = &_frame[_frame_size_y + _frame_size_uv];
}

int YUVFrame::getFrameSize()
{
	return _frame_size;
}

unsigned char* YUVFrame::frame()
{
	return _frame;
}

// плюсуется картинка
const unsigned char* YUVFrame::operator+(const YUVFrame & rv) const
{
	auto correct = [](unsigned char byte, unsigned char byte2) -> unsigned char
	{
		// не уверен что так надо делать(корректировать цвет)
		if ((byte + byte2) > 255)
			return 255;
		
		return byte + byte2;
	};

	for (int i = 0; i < rv._size.h; i++)
		for (int j = 0; j < rv._size.w; j++)
		{
			_yuv.y[i*_size.w + j] += correct(rv._yuv.y[i* rv._size.w + j], _yuv.y[i*_size.w + j]);
		}

	for (int i = 0; i < rv._size.h / 2; i++)
		for (int j = 0; j < rv._size.w / 2; j++)
		{
			_yuv.v[i*_size.w / 2 + j] = correct (rv._yuv.v[i* rv._size.w / 2 + j] , _yuv.v[i*_size.w / 2 + j]);
			_yuv.u[i*_size.w / 2 + j] = correct (rv._yuv.u[i* rv._size.w / 2 + j] , _yuv.v[i*_size.w / 2 + j]);
		}

	return _frame;
}

YUVFrame::~YUVFrame()
{
	delete[] _frame;
}

int YUVFrame::getFrameSizeY()
{
	return _frame_size_y;
}

int YUVFrame::getFrameSizeUV()
{
	return _frame_size_uv;
}
