#pragma once
#include <string>

class DumpableObject
{
public:
	DumpableObject();
	virtual std::wstring dump() = 0;
	virtual void load(std::wstring loadStr) = 0;
};
