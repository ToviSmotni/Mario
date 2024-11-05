#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>

class Frame
{
public:
	cv::Mat image; // rgb
	cv::Mat mask; // alpha
public:
	Frame(std::string const& path);
	bool isValid() const;
};

void drawFrame(Frame const& src, cv::Mat& dst, cv::Point const& TL);


class Animation
{
private:
	std::vector<Frame> _frames;

public:
	Animation(std::string const& folder);
	Frame const& getFrame(size_t idx);
	size_t numFrames() const;
};

typedef std::shared_ptr<Animation> AnimationPtr;