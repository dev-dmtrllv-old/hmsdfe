#pragma once

#include "pch.hpp"

namespace ion::clock
{
	using TimePoint = LARGE_INTEGER;
	
	TimePoint now()
	{
		TimePoint time;
		QueryPerformanceCounter(&time);
		return time;
	}

	TimePoint getFrequency()
	{
		TimePoint freq;
		QueryPerformanceFrequency(&freq);
		return freq;
	}

	struct Timer
	{
		Timer(bool start = false):
			frequency_(getFrequency())
		{
			if(start)
				this->start();
		}

		std::size_t calculate(std::size_t startIndex, std::size_t endIndex)
		{
			assert(endIndex < ticks_.size());
			TimePoint elapsedMicroseconds;
			elapsedMicroseconds.QuadPart = ((ticks_.at(endIndex).QuadPart - ticks_.at(startIndex).QuadPart) * 1000000) / frequency_.QuadPart;
			return static_cast<std::size_t>(elapsedMicroseconds.QuadPart);
		}

		inline void start()
		{
			tick();
		}

		void tick()
		{
			ticks_.emplace_back(now());
		}

		std::size_t stop()
		{
			tick();
			return calculate(0, ticks_.size() - 1);
		}

		const std::vector<TimePoint>& ticks() const noexcept { return ticks_; }

	private:
		const TimePoint frequency_;
		std::vector<TimePoint> ticks_;
	};
}