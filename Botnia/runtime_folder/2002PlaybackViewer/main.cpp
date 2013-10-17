//################
//### Playback ###
//################
#include "playbackGUI/playbackGUI.h"
#include "main.h"
#include <iostream.h>

int main(int argc, char **argv)
{
	cout << "-Starting Playback GUI...\n";
	cout.flush();
	if (argc == 2)
	{
		launchGUI(argv[1]);
	}
	else
	{
		launchGUI(NULL);
	}
	cout << "-Closing Playback GUI...\n";
	cout.flush();
	return 0;
	//===============================
}

//===================================================================================
