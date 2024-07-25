#pragma once

#include "QueueCommand.h"

struct Buffer;
class CoordinatorThread;

class QC_WaveFill : public QueueCommand
{

public:
	QC_WaveFill(Buffer* pBuffer, std::condition_variable& cv);

protected:
	virtual bool P_Execute() override;

private:
	Buffer* pBuffer;
	std::condition_variable& CV;
};
