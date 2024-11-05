#include "Animation.h"
#include <opencv2/opencv.hpp>
#include "Timer.h"
#include "HeroEntity.h"
#include "SlimeEntity.h"

using namespace cv;
using namespace std;
//---------- duck function-------------------
void duck()
{
	Animation animation(R"(../Animations/Hero/duckDown)");
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	bool isToExit = false;
	for (int i = 0; i < animation.numFrames(); i++)
	{
		Frame const& frame = animation.getFrame(i);

		Mat canvas = background.clone();

		Point topLeft(canvas.size().width / 2, canvas.size().height * 3 / 4);//3/4
		drawFrame(frame, canvas, topLeft);

		imshow("test", canvas);
	}//end for

	while (isToExit == false)
	{
		Animation animation(R"(../Animations/Hero/duckDown)");

		int key = waitKey(100);

		if (key == 'w')
		{
			Animation animation(R"(../Animations/Hero/standAfterDuck)");// added on friday
			for (int i = 0; i < animation.numFrames(); i++)
			{
				Frame const& frame = animation.getFrame(i);

				Mat canvas = background.clone();

				Point topLeft(canvas.size().width / 2, canvas.size().height * 3 / 4);//3/4
				drawFrame(frame, canvas, topLeft);

				imshow("test", canvas);
			}//end for
			isToExit = true;
		}

	}

}

void walkRight()
{

	Animation animation(R"(../Animations/Hero/runRight)");

	// and it's defined inside namespace cv. So the full name is Mat.
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);

	//// Draw
	bool isToExit = false;
	Mat canvas = background.clone();//added
	int maxToGO = 1;
	while (false == isToExit)
	{
		for (int i = 0; i < animation.numFrames(); i++)
		{
			Frame const& frame = animation.getFrame(i);

			Mat canvas = background.clone();

			Point topLeft(canvas.size().width / 2 + (canvas.size().width / 100) * maxToGO, canvas.size().height * 3 / 4);
			drawFrame(frame, canvas, topLeft);
			// show what we've got (use BOTH of the following two commands):
			imshow("test", canvas);

			int key = waitKey(100);
			/*if (frame.image.size().width < canvas.size().width)
			{

			}*/
			if (key == 'a' || key == 's' || key == 'w')
			{
				isToExit = true;
			}

			if (maxToGO < 40)
			{
				maxToGO++;
			}
			else
			{
				maxToGO = 1;
				Point topLeft(canvas.size().width / 2 + (canvas.size().width / 100) * maxToGO, canvas.size().height * 2 / 3);
				drawFrame(frame, canvas, topLeft);
			}
		}//end for



	}
}
///-----------------------jumpRight function----------------------------
void jumpRight()
{
	Animation animation(R"(../Animations/Hero/jump)");
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	bool isToExit = false;
	int i = 0;
	for (i = 0; i < animation.numFrames(); i++)
	{
		Frame const& frame = animation.getFrame(i);

		Mat canvas = background.clone();

		Point topLeft(canvas.size().width / 2 +i*10, canvas.size().height * 2/4);
		drawFrame(frame, canvas, topLeft);

		imshow("test", canvas);
	}//end for
	//while (false == isToExit)
	{

		//Animation animation1(R"(../Animations/Hero/idle)");
		for (int j = i; j > animation.numFrames(); j--)
		{
			Frame const& frame = animation.getFrame(j);

			Mat canvas = background.clone();

			Point topLeft(canvas.size().width / 2 + i * 10, canvas.size().height * 1/4);
			drawFrame(frame, canvas, topLeft);

			imshow("test", canvas);

		}
		int key = waitKey(100);
		if (key == 'd'||key=='a')
		{
			isToExit = true;
		}

	}

}
/////---------stop function--------------------------
void stop()
{
	Animation animation(R"(../Animations/Hero/idle)");
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	bool isToExit = false;

	for (int j = 0; j < animation.numFrames(); j++)
	{
		Frame const& frame = animation.getFrame(j);
		Mat canvas = background.clone();
		Point topLeft(canvas.size().width / 2 + j * 10, canvas.size().height * 3 / 4);
		drawFrame(frame, canvas, topLeft);
		imshow("test", canvas);
		int key = waitKey(100);
	}
	while (false == isToExit)
	{

		int key = waitKey(100);

		if (key == 'd')
		{
			isToExit = true;
		}

	}
}

int main()
{
	// OpenCV is a popular image processing and computer vision library.
	// it is not part of the C++ language, but it's written in C++.
	// in this project we integrated it for you.
	// The class that represents an image in OpenCV is Mat (i.e. Matrix),
	// and it's defined inside namespace cv. So the full name is Mat.
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);

	auto slimeState = CreateSlimeEnemy(R"(../Animations/SlimeOrange)");
	slimeState->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));
	
	auto slimeEnemy = createSlime(R"(../Animations/SlimeOrange)");
	slimeEnemy->reset(Point(background.size().width * 1/2, background.size().height * 3/5));

	EntityPtr hero = createHero(R"(../Animations/Hero)");
	hero->reset(Point(background.size().width / 2, background.size().height * 1/2));

	//EntityPtr slime(new Entity(slimeEnemy));
	EntityPtr slime(new Entity(slimeState));
	Timer timer(/*freq. ms=*/100);
	timer.Register(slime);
	timer.Register(hero);

	bool isToExit = false;
	while (false == isToExit)
	{
		Mat canvas = background.clone();

 		timer.tick();
		slimeEnemy->draw(canvas);

		slime->draw(canvas);
		hero->draw(canvas);

		imshow("test", canvas);
	}

	return 0;
}









