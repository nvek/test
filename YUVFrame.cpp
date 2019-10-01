#include "pch.h"
#include "YUVFrame.h"

YUVFrame::YUVFrame(const Size& size) :
_size(size)
{
	_frame_size_y = size.w * size.h;
	_frame_size_uv = ((size.w + 1) >> 1) * ((size.h + 1) >> 1);
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

const unsigned char* YUVFrame::operator+(const YUVFrame & rv) const
{
	int pictureW = rv._size.w;
	for (int v = 0, p = 0, w = 0; p < rv._frame_size_y; v++, p++, w++)
	{
		// по ширине самого маленького (картинки, по условию она не может быть больше видео)
		if (w >= pictureW)
		{
			v += (_size.w - pictureW);
			w = 0;
		}

		if ((_yuv.y[v] + rv._yuv.y[p]) < 255)
			_yuv.y[v] += rv._yuv.y[p];
	}

	for (int v = 0, p = 0, w = 0; p < rv._frame_size_uv; v++, p++, w++)
	{
		// тут по половинке w
		if (w >= pictureW / 2)
		{
			v += _size.w / 2 - pictureW / 2;
			w = 0;
		}

		if ((_yuv.u[v] + rv._yuv.u[p]) < 255)
			_yuv.u[v] += rv._yuv.u[p];

		if ((_yuv.v[v] + rv._yuv.v[p]) < 255)
			_yuv.v[v] += rv._yuv.v[p];
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
