#pragma once
#include <opencv2/opencv.hpp>
#include "Animation.h"

// Interface for various drawing behaviors.
class IGraphicsComponent
{
public:
	// Get mask of current frame (or just an empty cv::Mat() if you don't have)
	virtual cv::Mat getCollisionMask() = 0;
	// Draw current frame on cavnas:
	virtual void draw(cv::Mat& canvas, cv::Point const& topLeft) = 0;
	// Reset the graphics (a generic funtion that has different meaning for different graphic types)
	virtual void reset(int code = 0) = 0;

	// return value is true if the graphics would like to be replaced.
	// (graphics states that "it has fulfilled its duty and has no reason to be here anymore").
	virtual bool update() = 0;
};

typedef std::shared_ptr<IGraphicsComponent> IGraphicsComponentPtr;

// Example of implementation of Graphics of an animation:
class SingleAnimationGraphics : public IGraphicsComponent
{
protected:
	AnimationPtr _animation;
	int _currFrameIdx;
	bool _isCyclic;

public:
	SingleAnimationGraphics(AnimationPtr animPtr, bool isCyclic);

public:
	virtual cv::Mat getCollisionMask();
	virtual void draw(cv::Mat& canvas, cv::Point const& topLeft);
	virtual void reset(int code = 0) override;
	virtual bool update();
};



// In our game, the Hero has several "lives". Each time it hits an enemy - it reduces lives by 1.
// if lives reach zero - game is over.
// we visualizes amount of lives left by drawing them on the top right corner of the screen. 
// for example if Hero has 4 lives left, we will draw 4 hearts in a raw.
class LivesGraphics : public IGraphicsComponent
{
protected:
	Frame _singleLife; // visualization of one life.
	int _livesCount; // how much lives we have. gets updated at reset(..).

public:
	// @2: Write a c'tor
	LivesGraphics(Frame _singleLifeptr);
public:
	virtual cv::Mat getCollisionMask();
	virtual void draw(cv::Mat& canvas, cv::Point const& topLeft);
	// @2:reset: set number of lives:
	virtual void reset(int code = 0) override;
	virtual bool update();
};

// score graphics is writing a score on the screen, as plain text.
// it should use cv::putText function.
// See the following function in the cpp file:
void exampleOfPutText();
// for documentation and usage example. now fill in the class ! :)
class ScoresGraphics : public IGraphicsComponent
{
private:
	// parameters of putText that should be accepted in the c'tor and kept fixed.
	float _fontScale;
	int _score;
	int _fontFace;

public:
	ScoresGraphics( );
	// @2: Write a c'tor

public:
	virtual cv::Mat getCollisionMask();
	virtual void draw(cv::Mat& canvas, cv::Point const& topLeft);
	// @2:reset: should set current score:
	virtual void reset(int code = 0) override;
	virtual bool update();
};