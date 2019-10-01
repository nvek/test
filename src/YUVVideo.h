#include "YUVFrame.h"

#include <fstream>
class YUVVIdeo : public YUVFrame
{
public:
	YUVVIdeo(const std::string& videoFile, Size size);
	int getCountFrame();
	unsigned char* nextFrame();
private:
	std::ifstream _video;
	int _count;
};