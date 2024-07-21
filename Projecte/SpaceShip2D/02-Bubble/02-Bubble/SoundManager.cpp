#include "SoundManager.h"


SoundManager::SoundManager() {
	soundEngine = NULL;
}


SoundManager::~SoundManager() {
	soundEngine->drop();
}


void SoundManager::init() {
	soundEngine = createIrrKlangDevice();
}


ISoundEngine* SoundManager::getSoundEngine() {
	return soundEngine;
}