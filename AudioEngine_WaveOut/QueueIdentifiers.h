#pragma once

enum class CommandName : int
{
	Uninitialized = 0,
	Callback,
	Package,

	WaveStatusUpdate,
	WaveReady,
	WaveFillPackage,
	WaveFill,
	WavePlay,
};

enum class QueueName
{
	Coordinator,
	Playback
};


const char* Stringify(CommandName);