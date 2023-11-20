#ifndef AASL_SOUNDRESOURCE_H
#define AASL_SOUNDRESOURCE_H
#include "AudioFile/AudioFile.h"
namespace AASL
{
	class SoundResource 
	{
		private:
		int id;
		public:
		AudioFile<float>& audio();

		friend SoundResource LoadSound(const char* soundFilePath);
	};
}
#endif