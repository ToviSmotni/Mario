#include "Animation.h"
#ifdef _DEBUG
#pragma comment(lib, "opencv_world451d.lib")
#else
#pragma comment(lib, "opencv_world451.lib")
#endif

#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;




Mat extractMask(Mat transp) {
	std::vector<Mat> layers;
	split(transp, layers);
	return layers[3];
}

Frame::Frame(std::string const& path)
{
	image = imread(path, cv::IMREAD_UNCHANGED);
	mask = extractMask(image);
}

bool Frame::isValid() const
{
	return image.empty() == false;
}


void drawFrame(Frame const& src, Mat& dst, cv::Point const& TL)
{
	Rect dstROI(TL, TL + Point(src.image.size()));
	Rect dstBoundery(Point(0, 0), Point(dst.size()));
	Rect dstEffectiveROI = dstROI & dstBoundery;
	Rect srcEffectiveROI(dstEffectiveROI.tl() - TL, dstEffectiveROI.br() - TL);
	if (srcEffectiveROI.empty() || dstEffectiveROI.empty())
		return;

	src.image(srcEffectiveROI).copyTo(dst(dstEffectiveROI), src.mask(srcEffectiveROI));
}

//////////////////////////////////////////////////////////

Animation::Animation(std::string const& folder)
{
	for (const auto& entry : fs::directory_iterator(folder))
	{
		auto path = entry.path().string();
		_frames.push_back(Frame(path));
	}
}

Frame const& Animation::getFrame(size_t idx)
{
	return _frames[idx];
}

size_t Animation::numFrames() const
{
	return _frames.size();
}
