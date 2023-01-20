#include "engine/MainWorker.hpp"
#include "engine/Engine.hpp"

namespace ion::engine
{
	MainWorker::MainWorker(const Engine& engine):
		engine(engine),
		executor_(tasky::scheduler::executor())
	{ }

	MainWorker::~MainWorker()
	{ }

	void MainWorker::enterEventLoop() const
	{
		bool didWork = false;
		bool hasMessage = false;
		MSG msg = {};

		HANDLE threadHandle = GetCurrentThread();

		while (engine.isRunning())
		{
			didWork = executor_.runNext(false);
			hasMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			if (hasMessage)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (!didWork && !hasMessage)
			{
				switch (WaitForSingleObjectEx(threadHandle, INFINITE, true))
				{
					case WAIT_ABANDONED:
						MessageBoxA(NULL, "WAIT_ABANDONED", "WAIT_ABANDONED", MB_OK);
						break;
					case WAIT_IO_COMPLETION:
						MessageBoxA(NULL, "WAIT_IO_COMPLETION", "WAIT_IO_COMPLETION", MB_OK);
						break;
					case WAIT_OBJECT_0:
						MessageBoxA(NULL, "WAIT_OBJECT_0", "WAIT_OBJECT_0", MB_OK);
						break;
					case WAIT_TIMEOUT:
						MessageBoxA(NULL, "WAIT_TIMEOUT", "WAIT_TIMEOUT", MB_OK);
						break;
					case WAIT_FAILED:
						MessageBoxA(NULL, "WAIT_FAILED", "WAIT_FAILED", MB_OK);
						break;
				}
			}
		}
	}
}