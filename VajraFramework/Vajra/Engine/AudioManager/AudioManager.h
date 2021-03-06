//
//  AudioManager.h
//  Created by Matt Kaufmann on 12/04/13.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

class AudioManager {
public:
	~AudioManager();

private:
	AudioManager();

	void init();
	void destroy();

	ALCdevice* device;
	ALCcontext* context;

	friend class Engine;
};

#endif // AUDIOMANAGER_H
