#include "AASL.h"
#include <iostream>

using namespace AASL;

int main(int argc, char** argv) 
{
	if(argc != 2) 
	{
		std::cout << "Usage: aaslplay <filename>\n";
		return -1;    	
	}
	AASL::Init();

	//std::cout << "Simple non-blocking sound play...\n";
	//StartSound("./helloworld.wav");
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	
	std::cout << "Controlling the the way an audio is played\n";
	///////////////
	std::cout << 
	"1 - pause/unpause\n"
	"2 - increase volume by 10%\n"
	"3 - decrease volume by 10%\n"
	"4 - rewind by 5 seconds\n"
	"5 - forward by 5 seconds\n"
	"6 - restart audio\n"
	"7 - turn on/off audio loop\n"
	"8 - decrease playback speed by 10%\n"
	"9 - increase playback speed by 10%\n"
	"0 - exit demo\n";

	SoundController audio = StartSound(argv[1]);
	
	int choice = -1;
	while(choice != 0) 
	{
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			if(audio.isPaused()) 
			{
				audio.resume();
			}
			else if(audio.isPlaying())
			{
				audio.pause();
			}
			break;
		case 2:
			audio.options().volume += 0.1f;
			std::cout << "Volume set to: " << audio.options().volume << "\n";
			break;
		case 3:
			audio.options().volume -= 0.1f;
			std::cout << "Volume set to: " << audio.options().volume << "\n";
			break;
		case 4:
			audio.rewindTime(5);
			break;
		case 5:
			audio.forwardTime(5);
			break;
		case 6:
			audio.restart();
			break;
		case 7:
			audio.options().loop = !audio.options().loop;
			break;
		case 8:
			audio.options().playSpeed -= 0.1f;
			break;
		case 9:
			audio.options().playSpeed += 0.1f;
			break;
		default:
			break;
		}
	}
	///////////////
}
