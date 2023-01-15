#include "tasky/TaskScheduler.hpp"

using namespace ion::tasky;

Task<> test()
{
	puts("test()");
	co_return;
}

int main()
{
	scheduler::schedule(test(), test(), test(), test(), test(), test(), test(), test(), test(), test());

	std::thread t([]
	{
		scheduler::schedule(test(), test(), test(), test(), test(), test(), test(), test(), test(), test());
		while(scheduler::executor().runNext()) {}
	});
	
	while(scheduler::executor().runNext()) {}
	
	t.join();
	return 0;
}