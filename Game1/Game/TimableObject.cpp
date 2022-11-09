#include "TimableObject.h"
#include <math.h>

TimableObject::TimableObject()
{
	// don't use this
}

void TimableObject::timeElapsed(long long microseconds)
{
	for (auto it = _events.begin(); it != _events.end(); ) {
		int numTimesToFireEvent = (*it).second.timeElapsed(microseconds);
		while (numTimesToFireEvent > 0) {
			doEvent((*it).second.getEvent());
			numTimesToFireEvent--;
			if ((*it).second.shouldDestroy()) {
				break;
			}
		}
		if ((*it).second.shouldDestroy()) {
			onDestroy((*it).second.getEvent());
			it = _events.erase(it);
		} else {
			it++;
		}
	}
}

int TimableObject::addTimer(float periodInSeconds, std::wstring eventName)
{
	int eventId = getNewEventId();
	_events[eventId] = TimableObjectEvent(periodInSeconds, eventName);
	return eventId;
}

void TimableObject::doEvent(std::wstring event)
{
	// Be default does nothing. Method must be overridden.
}

void TimableObject::onDestroy(std::wstring event)
{
	// Be default does nothing. Method must be overridden.
}

void TimableObject::destroyTimerAfterNumTimes(int timerId, int numberOfTimes)
{
	_events[timerId].destroyAfterNumTimes(numberOfTimes);
}

void TimableObject::destroyTimerAfterAmountTime(int timerId, float amountTimeInSeconds)
{
	_events[timerId].destroyAfterAmountTime(amountTimeInSeconds);
}

void TimableObject::clearTimers()
{
	_events.clear();
}

void TimableObject::destroyTimer(int timerId)
{
	_events.erase(timerId);
}

bool TimableObject::timerExists(int timerId)
{
	auto result = _events.find(timerId);
	return result != _events.end();
}

void TimableObject::setTimerToDestroy(int timerId)
{
	_events[timerId].setToDestroy();
}

int TimableObject::getNewEventId()
{
	int eventId = _eventIdCounter;
	_eventIdCounter++;
	return eventId;
}
