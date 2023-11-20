#include "SoundResource.h"
#include "Internal.h"

namespace AASL 
{
	extern SoundResourceInternal soundResources[max_sound_resources];
	AudioFile<float>& SoundResource::audio() 
	{
		return soundResources[this->id % max_sound_resources].audio;
	}
}