#include "FileThread.h"
#include "FileSlow.h"
#include "Buffer.h"
#include "CoordinatorThread.h"

using namespace Azul;

FileThread::FileThread()
	: ThreadBase("----- FILE -----"),
	poFileName(new char[FileThread::FILE_NAME_SIZE]()),
	NumFilesRead(0),
	poBuffer(new Buffer(FileThread::BUFFER_SIZE)),
	prCoordTransfer(),
	FileToCoordComplete()
{
}

FileThread::~FileThread()
{
	delete poFileName;
	delete poBuffer;
}

void FileThread::ThreadMain()
{
	while (!ShouldShutdown())
	{
		if (NumFilesRead < MAX_NUM_WAVE_FILES)
		{
			char* pName = GetFileName(this->NumFilesRead);
			LoadFile(pName);

			prCoordTransfer.set_value(std::ref(poBuffer));

			this->prCoordTransfer = std::promise<Buffer*&>();

			std::unique_lock<std::mutex> m(FileToCoordComplete.mtx);
			FileToCoordComplete.cv.wait(m);
		}

		std::this_thread::sleep_for(200ms);
	}
}

FileToCoordCompleteFlag& FileThread::GetTransferCompleteCV()
{
	return FileToCoordComplete;
}

std::future<Buffer*&> FileThread::GetTransferFuture()
{
	return prCoordTransfer.get_future();
}

char* FileThread::GetFileName(int index)
{
	sprintf_s(poFileName, FileThread::FILE_NAME_SIZE, "../Data/wave_%d.wav", index);
	return poFileName;
}

void FileThread::LoadFile(const char* const pFileName)
{
	FileSlow::Handle fh;
	FileSlow::Error err;

	assert(pFileName);

	//Debug::out("%s load start <-- \n", pFileName);

	err = FileSlow::Open(fh, pFileName, FileSlow::Mode::READ);
	assert(err == FileSlow::Error::SUCCESS);

	err = FileSlow::Seek(fh, FileSlow::Position::END, 0);
	assert(err == FileSlow::Error::SUCCESS);

	DWORD size;
	err = FileSlow::Tell(fh, size);
	assert(err == FileSlow::Error::SUCCESS);

	err = FileSlow::Seek(fh, FileSlow::Position::BEGIN, 0);
	assert(err == FileSlow::Error::SUCCESS);

	err = FileSlow::Read(fh, poBuffer->GetRawBuffer(), size);
	assert(err == FileSlow::Error::SUCCESS);

	err = FileSlow::Close(fh);
	assert(err == FileSlow::Error::SUCCESS);

	poBuffer->SetFilled(size);
	this->NumFilesRead++;

	//Debug::out("%s load end --> %d bytes \n", pFileName, poBuffer->GetCurrSize());
}