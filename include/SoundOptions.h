#ifndef AASL_SOUNDOPTIONS_H
#define AASL_SOUNDOPTIONS_H
#include "OutputDevice.h"
namespace AASL 
{
	OutputDevice DefaultOutput();
	class SoundOptions 
	{
		public:
		float playSpeed;
		float volume;
		//audio start time in seconds
		float startTime;
		//audio end time in seconds, < startTime for default end 
		float endTime;
		bool loop;
		//defines whenever the sound should be kept or freed from memory at the end of play, false by default
		bool keepAlive;
		OutputDevice outputDevice;
		
		inline SoundOptions()
			{
				playSpeed = 1.0f;
				volume = 1.0f;
				startTime = 0.0f;
				loop = false;
				keepAlive = false;
				outputDevice = DefaultOutput();
				endTime = startTime - 1.0f;
			}
	};
}
#endif