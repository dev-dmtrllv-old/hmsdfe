#pragma once

#include "engine/SubSystem.hpp"

namespace ion::engine
{
	class ResourceManager : public SubSystem<ResourceManager>
	{
	protected:
		void onInitialize() override {}
		void onTerminate() override {}
	};
}