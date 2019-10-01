#pragma once

class YUVFrame
{	
	// представление одного кадра
	typedef struct
	{
		unsigned char* y;
		unsigned char* u;
		unsigned char* v;
	} YUV;
protected:
	typedef struct
	{
		int w;
		int h;
	} Size;

public:
	YUVFrame() = default;
	YUVFrame(const Size& size);
	~YUVFrame();

	int getFrameSizeY();
	int getFrameSizeUV();
	int getFrameSize();
	unsigned char* frame();
	const unsigned char* operator+(const YUVFrame& rv) const;

protected:
	Size			_size;
	YUV				_yuv;
	int				_frame_size_y;
	int				_frame_size_uv;
	int				_frame_size;
	unsigned char*	_frame;
};