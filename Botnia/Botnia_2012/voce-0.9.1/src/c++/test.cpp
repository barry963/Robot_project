#include <iostream>
#include "voce.h"


int main()
{
	while (voce::getRecognizerQueueSize() > 0)
	{
	      std::string s = voce::popRecognizedString();
	      std::cout << "You said: " << s << std::endl;
	}
return 1;
}