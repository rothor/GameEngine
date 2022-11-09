#include "TimableObjectEvent.h"

TimableObjectEvent::TimableObjectEvent()
{
	// don't use this
}

TimableObjectEvent::TimableObjectEvent(float periodInSeconds, std::wstring eventName)
{
	_periodInMicroseconds = llroundf(periodInSeconds * 1000000);
	_eventName = eventName;
}

int TimableObjectEvent::timeElapsed(long long microseconds)
{
	_elapsedMicroseconds += microseconds;
	_totalElapsedMicroseconds += microseconds;
	int doEventCounter = 0;
	while (_elapsedMicroseconds >= _periodInMicroseconds) {
		doEventCounter++;
		_elapsedMicroseconds -= _periodInMicroseconds;
		_eventFireCount++;
	}

	if (_willDestroyAfterAmountTime && _totalElapsedMicroseconds >= _microsecondsToDestroy) {
		_shouldDestroy = true;
	}
	if (_willDestroyAfterNumTimes && _eventFireCount >= _numTimesToDestroy) {
		_shouldDestroy = true;
	}

	return doEventCounter;
}

bool TimableObjectEvent::shouldDestroy()
{
	return _shouldDestroy;
}

std::wstring TimableObjectEvent::getEvent()
{
	return _eventName;
}

void TimableObjectEvent::destroyAfterNumTimes(int numberOfTimes)
{
	_willDestroyAfterNumTimes = true;
	_numTimesToDestroy = numberOfTimes;
}

void TimableObjectEvent::destroyAfterAmountTime(float amountTimeInSeconds)
{
	_willDestroyAfterAmountTime = true;
	_microsecondsToDestroy = llroundf(amountTimeInSeconds * 1000000);
}

void TimableObjectEvent::setToDestroy()
{
	_shouldDestroy = true;
}
