#pragma once

#include "Wave.h"
#include "CircularQueue.h"

class PlaybackThread;
class PlayController;

class WaveTable
{
public:
	static const int NUM_WAVES = 20;
	static const int NUM_WAVE_STATES = 5;

	enum class WaveStatus : byte
	{
		Open			= 0x0,
		WaitingForData	= 0x1,
		WaitingPlay		= 0x2,
		Closed			= 0x3,

		Playing			= 0x4,
	};

	struct WaveEntry
	{
		int WaveIndex;
		std::promise<void> prPlay;

		WaveEntry();
		~WaveEntry() = default;

		void Set(int index);
	};

	WaveTable() = delete;
	WaveTable(PlaybackThread* playback, QueuePusher* pCoordPusher);

	WaveTable(const WaveTable&) = default;
	WaveTable& operator=(const WaveTable&) = default;
	~WaveTable();

	void InitializeTable(const HWAVEOUT& hWaveOut, QueuePusher* pPlaybackPusher
		, BeginPlayFlag& beginPlay, std::future<void>& shutdownFlag);

	bool UpdateStatus(int index, WaveStatus status);

	Wave* GetWave(int index);
	
	void SignalWaveFinished();

	void SignalPlayQueueReady(int* pQueue, int count);
	void SignalPlayQueueFull(int* pQueue, int count);
	void SignalPlay(int index);
	void SignalPlayEmpty();

private:
	// StatusUpdateFuncs
	static const int NUM_WAVE_STATE_FUNCS = 4;
	void SetOpen(int index);
	void SetWaitingForData(int index);
	void SetWaitingPlay(int index);
	void SetClosed(int index);

	// Data
	std::mutex mtx;

	PlaybackThread* pPlayback;

	Wave* pWaves[NUM_WAVES];
	QueuePusher* pCoordPusher;
	PlayController* poPlayController;

	// Record keeping data
	WaveEntry Table[NUM_WAVES];
	int OpenWaves;
	typedef void (WaveTable::* StatusUpdates)(int);
	StatusUpdates StatusUpdateFuncs[NUM_WAVE_STATE_FUNCS];
};