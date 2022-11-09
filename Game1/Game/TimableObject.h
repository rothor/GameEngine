#pragma once
#include "TimableObjectEvent.h"
#include <string>
#include <map>

class TimableObject
{
public:
	TimableObject();
	int addTimer(float periodInSeconds, std::wstring eventName);
	virtual void timeElapsed(long long microseconds);
	/* Note for doEvent:
	 * Do not call destroyTimer(timerId) inside doEvent. To destroy a timer,
	 * call timer.setToDestroy().
	 */
	virtual void doEvent(std::wstring event);
	virtual void onDestroy(std::wstring event);
	void destroyTimerAfterNumTimes(int timerId, int numberOfTimes);
	void destroyTimerAfterAmountTime(int timerId, float amountTimeInSeconds);
	void clearTimers();
	void destroyTimer(int timerId);
	bool timerExists(int timerId);
	void setTimerToDestroy(int timerId);

private:
	int getNewEventId();

private:
	std::map<int, TimableObjectEvent> _events;
	int _eventIdCounter{ 0 };
};