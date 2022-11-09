#include "RandomColorGetter.h"

std::default_random_engine RandomColorGetter::_generator{};
std::uniform_int_distribution<int> RandomColorGetter::_distribution(0, 255);
std::_Binder<std::remove_cv<std::_Unforced>::type, std::uniform_int_distribution<int>&, std::default_random_engine&> RandomColorGetter::_shade = std::bind(_distribution, _generator);
int RandomColorGetter::GetRandomColor()
{
	return _shade() * 0x10000 + _shade() * 0x100 + _shade();
}