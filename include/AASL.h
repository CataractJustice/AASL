#ifndef AASL_H
#define AASL_H
#include "SoundController.h"
#include "SoundResource.h"
#include "SoundOptions.h"
#include "OutputDevice.h"
namespace AASL 
{
	void Init();
	/*
	"Amber" Audio and Sound Library API
	*/
	SoundController StartSound(const char* soundFilePath, SoundOptions options = SoundOptions());
	SoundController StartSound(SoundResource sound, SoundOptions options = SoundOptions());

	SoundResource LoadSound(const char* soundFilePath);
	SoundResource MakeSoundResource();

	OutputDevice GetOutputDevice(const char* handle);
	const char** OutputDeviceList();
	OutputDevice DefaultOutput();

	int Error();
}
#endif