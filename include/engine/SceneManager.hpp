#pragma once

#include "engine/SubSystem.hpp"

namespace ion::engine
{
	class SceneManager : public SubSystem<SceneManager>
	{
	protected:
		void onInitialize() override {}
		void onTerminate() override {}
	};
}