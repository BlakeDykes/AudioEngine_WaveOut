#pragma once

#include "ThreadBase.h"
#include "Buffer.h"

class FileThread : public ThreadBase
{
public:
	static const unsigned int BUFFER_SIZE = 512 * 1024;
	static const unsigned int FILE_NAME_SIZE = 20;
	static const int MAX_NUM_WAVE_FILES = 23;

	FileThread();
	virtual ~FileThread();

	FileToCoordCompleteFlag& GetTransferCompleteCV();
	std::future<Buffer*&> GetTransferFuture();

protected:
	virtual void ThreadMain() override;

private:
	char* GetFileName(int index);
	void LoadFile(const char* const pFileName);

	char* poFileName;
	int NumFilesRead;
	Buffer* poBuffer;

	std::promise<Buffer*&> prCoordTransfer;
	FileToCoordCompleteFlag& FileToCoordComplete;
};