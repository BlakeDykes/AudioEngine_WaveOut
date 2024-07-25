#include "QueueIdentifiers.h"

const char* Stringify(CommandName name)
{
	switch (name)
	{
	case CommandName::Uninitialized:	return "-- UNINITIALIZED --";
	case CommandName::Callback:			return "Callback";
	case CommandName::Package:			return "Command package";
	case CommandName::WaveReady:		return "Wave Ready";
	case CommandName::WaveStatusUpdate: return "Wave status update";
	case CommandName::WaveFill:			return "Fill Wave";
	case CommandName::WavePlay:			return "Play Wave";
	case CommandName::WaveFillPackage:	return "Fill Wave package";

	default:
		assert(false);
		return "Command name not set";
	}
}