#include "YUVVideo.h"

#include <string>
#include <fstream>

// суть этого класса заключается в том, чтобы он возвращал из потока массив байт по ширине фрейма
YUVVIdeo::YUVVIdeo(const std::string& videoFile, Size size) :
YUVFrame(size),
_video(videoFile, std::ios::ate | std::ios::binary | std::ios::in)
{
	if (!_video.is_open())
		return;
	// получаем размер файла
	size_t filesize = _video.tellg();
	// начало файла
	_video.seekg(0, std::ios::beg);

	_count = (int)(filesize / getFrameSize());
}

YUVVIdeo::~YUVVIdeo()
{
	_video.close();
}

int YUVVIdeo::getCountFrame()
{
	return _count;
}

unsigned char* YUVVIdeo::nextFrame()
{
	_video.read((char*)_frame, _frame_size);
	return _frame;
}

const unsigned char* YUVVIdeo::operator+(const YUVFrame & rv)
{
	nextFrame();
	return YUVFrame::operator+(rv);
}
