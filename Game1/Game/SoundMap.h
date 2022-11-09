#pragma once
#include <map>

enum class SoundEnum {
	GameOver, PianoC6, PianoD6, PianoE6, PianoF6, PianoG6, PianoA6, PianoB6, PianoC7,
	DrugwarBeat, DrugwarXylo, DrugwarCoconut, DrugwarWhirly, DrugwarKeys, DrugwarOrgan
};

class SoundMap
{
public:
	static std::map<SoundEnum, int> idMap;
};

