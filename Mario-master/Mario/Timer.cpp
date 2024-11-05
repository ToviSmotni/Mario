#include "Timer.h"
#include "opencv2/opencv.hpp"
#include "Config.h"

Timer::Timer(size_t frequency_ms)
	:_frequency_ms(frequency_ms)
{
}

void Timer::tick()
{
	int key = cv::waitKey(_frequency_ms);

	Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::TIME_TICK });
	if (key != -1)
		Notify(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED , key });
}
