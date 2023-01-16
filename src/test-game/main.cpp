#include "pch.hpp"
#include "engine/Engine.hpp"

int main()
{
	using namespace ion::engine;

	const Engine& engine = Engine::initialize();
	
	Engine::terminate();

	return 0;
}