#include "tasky/TaskScheduler.hpp"
#include "clock/clock.hpp"

using namespace ion::tasky;
using namespace ion::clock;

Task<int> B()
{
	static int i = 0;
	co_return i++;
}

Task<> A()
{
	Timer timer(true);

	co_await scheduler::awaitEach(500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(1500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(3000, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(5000, [](std::size_t) { return B(); });
	
	co_await scheduler::awaitEach(500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(1500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(3000, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(5000, [](std::size_t) { return B(); });
	
	co_await scheduler::awaitEach(500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(1500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(3000, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(5000, [](std::size_t) { return B(); });

	co_await scheduler::awaitEach(500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(1500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(3000, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(5000, [](std::size_t) { return B(); });

	co_await scheduler::awaitEach(500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(1500, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(3000, [](std::size_t) { return B(); });
	co_await scheduler::awaitEach(5000, [](std::size_t) { return B(); });

	const auto time = timer.stop();

	printf("50.000 calls made in %zu micro seconds\n", time);
}

int main()
{
	scheduler::schedule(A());

	std::vector<std::thread> workers;

	for(std::size_t i = 0; i < 7; i++)
		workers.emplace_back([] { while (scheduler::executor().runNext()) { } });

	while (scheduler::executor().runNext()) { }

	for (auto& t : workers)
		t.join();

	return 0;
}