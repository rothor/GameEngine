#pragma once
#include <string>

class TimableObjectEvent
{
public:
	TimableObjectEvent();
	TimableObjectEvent(float periodInSeconds, std::wstring eventName);
	int timeElapsed(long long microseconds);
	bool shouldDestroy();
	std::wstring getEvent();
	void destroyAfterNumTimes(int numberOfTimes);
	void destroyAfterAmountTime(float amountTimeInSeconds);
	void setToDestroy();

protected:
	long long _periodInMicroseconds{ 0 };
	std::wstring _eventName;
	long long _elapsedMicroseconds{ 0 };
	bool _shouldDestroy{ false };
	bool _willDestroyAfterNumTimes{ false };
	int _numTimesToDestroy{ 0 };
	int _eventFireCount{ 0 };
	bool _willDestroyAfterAmountTime{ false };
	long long _microsecondsToDestroy{ 0 };
	long long _totalElapsedMicroseconds{ 0 };
};

