#include "YUVFrame.h"
#include <fstream>

class YUVVIdeo : public YUVFrame
{
public:
	YUVVIdeo(const std::string& videoFile, Size size);
	~YUVVIdeo();
	int getCountFrame();
	unsigned char* nextFrame();
	const unsigned char* operator+(const YUVFrame& rv);

private:
	std::ifstream	_video;
	int				_count;
};