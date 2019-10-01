#pragma once

#include "pch.h"

class YUVFrame
{	
	// представление одного кадра
	typedef struct
	{
		unsigned char* y;
		unsigned char* u;
		unsigned char* v;
	} YUV;

	typedef struct
	{
		int w;
		int h;
	} Size;

public:
	YUVFrame(const Size& size);
	~YUVFrame();

	int getFrameSizeY();
	int getFrameSizeUV();
	int getFrameSize();
	unsigned char* frame();
	const unsigned char* operator+(const YUVFrame& rv) const;

private:
	Size			_size;
	YUV				_yuv;
	int				_frame_size_y;
	int				_frame_size_uv;
	int				_frame_size;
	unsigned char*	_frame;
};