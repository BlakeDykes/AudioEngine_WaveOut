//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "FileSlow.h"
#include "ThreadWrapper.h"
using namespace Azul;

int main()
{
	START_BANNER_MAIN("-Main-");

	FileThread f;
	CoordinatorThread c(f);
	PlaybackThread p(c.GetQueuePusher());

	f.Launch();
	c.Launch();
	p.Launch();

	ThreadManager::ShutdownOnKeyPress();
}

// ---  End of File ---
