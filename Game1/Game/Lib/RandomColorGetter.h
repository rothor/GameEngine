#pragma once
#include <random>
#include <functional>

class RandomColorGetter
{
public:
	static int GetRandomColor();

private:
	static std::default_random_engine _generator;
	static std::uniform_int_distribution<int> _distribution;
	static std::_Binder<std::remove_cv<std::_Unforced>::type, std::uniform_int_distribution<int>&, std::default_random_engine&> _shade;
};