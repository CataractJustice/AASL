#ifndef AASL_SOUNDCONTROLLER_H
#define AASL_SOUNDCONTROLLER_H
#include "SoundOptions.h"
#include "SoundResource.h"
namespace AASL 
{
	/*

	*/
	class SoundController 
	{
		private:
		int id;
		public:
		//
		SoundResource soundResource();
		//writable sound playback options
		SoundOptions& options();
		//
		void setTimeSeconds(float seconds);
		//
		float getTimeSeconds();
		//forwards playe audio
		void forwardTime(float seconds);
		//rewinds played audio
		void rewindTime(float seconds);
		void restart();
		void pause();
		void resume();
		//writable current sound sample index
		int& sample();
		//
		bool isPlaying();
		//
		bool isPaused();
		//
		bool isAlive();
		friend SoundController StartSound(SoundResource sound, SoundOptions options);
	};
}
#endif