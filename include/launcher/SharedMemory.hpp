#pragma once

#include "pch.hpp"

namespace ion::launcher
{
	class SharedMemory
	{
	private:
		constexpr const static std::uint32_t bufferSize = 1024 * 32;
	public:
		struct State
		{
			const std::size_t startOffset = sizeof(State);
			HANDLE event = 0;
			char* writePos = 0;
			std::atomic<std::size_t> messageCount = 0;

			State(HANDLE event): event(event) { }
		};

		class Writer
		{
		public:
			static Writer& get(const std::wstring& name)
			{
				static std::unordered_map<std::wstring, Writer> writers_;
				if (!writers_.contains(name))
					writers_.emplace(name, name);
				return writers_.at(name);
			}

			Writer(const std::wstring& name):
				fileHandle_(OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name.data()))
			{
				if (!fileHandle_)
					throw std::runtime_error("Could not open shared memory file mapping!");

				buffer_ = static_cast<char*>(MapViewOfFile(fileHandle_, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize));
				state_ = reinterpret_cast<State*>(buffer_);
			}

		public:
			void write(std::string_view str)
			{
				char* dest = state_->writePos;
				const std::size_t size = str.size();
				*reinterpret_cast<std::size_t*>(dest) = size;
				dest += sizeof(std::size_t);
				memcpy(dest, str.data(), size);
				state_->writePos = dest + size;
				state_->messageCount.fetch_add(1, std::memory_order::acq_rel);
				SetEvent(state_->event);
			}

		private:
			HANDLE fileHandle_;
			HANDLE eventHandle_;
			char* buffer_;
			State* state_;
		};

		SharedMemory(LPSECURITY_ATTRIBUTES securityAttributes, const std::wstring& name):
			fileHandle_(CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, bufferSize, name.data())),
			eventHandle_(CreateEvent(securityAttributes, false, false, (name + L"_event").data()))
		{
			if (!fileHandle_)
				throw std::runtime_error("Could not create shared memory file mapping!");
			buffer_ = static_cast<char*>(MapViewOfFile(fileHandle_, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize));
			new (buffer_) State(eventHandle_);
		}

		SharedMemory(const SharedMemory&) = delete;
		SharedMemory(SharedMemory&&) = delete;
		~SharedMemory()
		{
			UnmapViewOfFile(buffer_);
			CloseHandle(fileHandle_);
			CloseHandle(eventHandle_);
		}

		void read()
		{
			WaitForSingleObject(eventHandle_, INFINITE);
			puts("oki");
		}

	private:
		HANDLE eventHandle_;
		HANDLE fileHandle_;
		char* buffer_;
	};
}