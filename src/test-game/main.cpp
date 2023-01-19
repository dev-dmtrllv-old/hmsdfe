#include "pch.hpp"

#include "engine/Engine.hpp"
#include "engine/SubSystem.hpp"
#include "engine/Game.hpp"

#include "utils/String.hpp"
#include "utils/Pointer.hpp"

#include "test-game/Launcher.hpp"

#include "async/file.hpp"
#include "async/Worker.hpp"
#include "win/wrapper.hpp"

class Game: public ion::engine::Game
{
public:
	Game(): ion::engine::Game() { }

	Game(const Game&) = delete;
	Game(Game&&) = delete;

	virtual ~Game() override { }

	virtual const char* name() const noexcept override
	{
		return "Test Game";
	}

	virtual void onLoad() override
	{

	}
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
	using namespace ion::utils::pointer;

	try
	{
		alignas(0x8) int value = 0;
		int result = 0;

		{
			TaggedPtr<int> taggedInt(&value, { 1, 1, 1 });

			*taggedInt = 200;
			taggedInt.setTag({ 1, 0, 1 });

			const int i = *taggedInt;
			const Tag tag = taggedInt.tag();

			result = i + tagToByte(tag);
		}

		return result; // 205
	}
	catch (const std::runtime_error& e)
	{
		MessageBoxA(NULL, e.what(), e.what(), MB_OK);
	}

	return 0;
}