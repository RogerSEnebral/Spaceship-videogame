#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include <irrKlang.h>
using namespace irrklang;

class SoundManager {

public:

	static SoundManager& instance() {
		static SoundManager soundManager;
		return soundManager;
	}

	SoundManager();
	~SoundManager();

	void init();
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* soundEngine;
};

#endif