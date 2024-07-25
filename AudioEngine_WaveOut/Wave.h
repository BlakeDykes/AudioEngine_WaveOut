#pragma once

#include "ThreadBase.h"

struct Buffer;
class QC_WaveFill;
class QC_WaveStatusUpdate;

class Wave : public ThreadBase
{
public:
	static const unsigned int BUFFER_SIZE = 2 * 1024;

	Wave() = delete;
	Wave(const HWAVEOUT& hOutHandle, QueuePusher*& playbackPusher
		, BeginPlayFlag& beginPlay, std::future<void>& shutdownFlag, int index);

	Wave(const Wave&) = delete;
	Wave& operator=(const Wave&) = delete;
	~Wave();
	void Write();

	QC_WaveFill* GetFillCommand(std::future<void>&& playFuture);

	void RequestPlay();

protected:
	virtual void ThreadMain() override;
	virtual bool ShouldShutdown() override;

private:
	void WaitForWrite();
	void WaitForPlaySignal();
	void WaitForStopSignal();

	WaveFill cvFill;
	BeginPlayFlag& cvBeginPlay;
	std::future<void>& fShutdown;
	std::future<void> fPlay;

	Buffer* poBuffer;
	WAVEHDR* poHeader;
	const HWAVEOUT& OutHandle;

	QueuePusher* pPlaybackPusher;

	QC_WaveStatusUpdate* poStatusUpdate;
	QC_WaveFill* poWaveFill;
	QC_Callback* poStatusCallback;

};