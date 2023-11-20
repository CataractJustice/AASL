#ifndef AASL_INTERNAL_H
#define AASL_INTERNAL_H
#include "AASL.h"
#include <alsa/asoundlib.h>
namespace AASL
{
#define max_sounds_playing 256
#define max_output_devices 64
#define max_sound_resources 2048

	struct SoundInternal 
	{
		SoundResource resource;
		SoundController controller;
		SoundOptions options;
		bool alive;
		bool paused;
		int currentSample;
		int id;
	};

	struct SoundResourceInternal 
	{
		AudioFile<float> audio;
		bool free;
	};

	struct OutputDeviceInternal 
	{
		snd_pcm_t *pcmHandle;
		snd_pcm_hw_params_t *params;
		snd_pcm_uframes_t frames;
		float* buffer;
		int bufferSize;
		int bufferLength;
		int channels;
		unsigned int periodTime;
		unsigned int sampleRate;
	};
}
#endif