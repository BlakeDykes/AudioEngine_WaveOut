#pragma once

#include "ThreadBase.h"
#include "WaveTable.h"

class PlayController;

class PlaybackThread : public QueueThreadBase
{
public:
	static const int TOTAL_WAVES = 20;

	PlaybackThread() = delete;
	PlaybackThread(QueuePusher* coordQueue);
	virtual ~PlaybackThread();

	static void CALLBACK StaticWaveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	void WaveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

	bool RequestDataRequest();
	bool RequestPlay();

	WaveTable* GetWaveTable();

	void BeginPlay();
	void SignalShutdown();

protected:
	virtual void ThreadMain() override;
	virtual bool ShouldShutdown() override;


private:
	struct WaveOutData
	{
		HWAVEOUT HWaveOut;
		UINT uDeviceID;
		DWORD_PTR Callback;
		DWORD_PTR CallbackData;
		DWORD CallbackMechanism;
		
		WaveOutData() = delete;
		WaveOutData(DWORD_PTR callbackData);
	};

	bool InitializeWaveOut();
	void LaunchWaves();
	void ProcessIn();
	void Shutdown();

	void HandleWaveDone();

	bool bPlayEmpty;
	bool bPlayStarted;
	BeginPlayFlag& cvBeginPlay;
	WavesShouldShutdownFlag prShutdown;

	WaveTable* poTable;

	WaveOutData WData;
};