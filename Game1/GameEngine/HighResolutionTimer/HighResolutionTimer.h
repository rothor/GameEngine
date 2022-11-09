#pragma once
#include <Windows.h>


class HighResolutionTimer
{
public:
	HighResolutionTimer();

private:
	LARGE_INTEGER startTime;
	LARGE_INTEGER frequency;

public:
	void reset();
	/* Note:
	 *   The return value for readMicro can safely be stored in an int if the timer has
	 *     been running for less than 71 minutes: 1000 * 1000 * 60 / (2 ^ 32) ~= 71.5
	 *   Also, readMilli rounds down to the nearest millisecond.
	 */
	long long readMilli();
	long long readMicro();
};