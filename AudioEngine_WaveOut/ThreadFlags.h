#pragma once

struct PromiseFlag
{
	std::promise<void> pr;
	std::future<void> f = pr.get_future();

	inline void Flag() { pr.set_value(); }
	inline bool IsFlagged() const { return f.wait_for(0s) == std::future_status::ready; }
};

struct CVFlag
{
	std::mutex mtx;
	std::condition_variable cv;

	inline void FlagOne() { cv.notify_one(); }
	inline void FlagAll() { cv.notify_all(); }
};

typedef struct PromiseFlag ShutdownCompleteFlag;
typedef struct PromiseFlag ShouldShutdownFlag;
typedef struct PromiseFlag WavesShouldShutdownFlag;;

typedef struct CVFlag ThreadCountZeroFlag;
typedef struct CVFlag FileToCoordReadyFlag;
typedef struct CVFlag FileToCoordCompleteFlag;

typedef struct CVFlag BeginPlayFlag;
typedef struct CVFlag WaveFill;


