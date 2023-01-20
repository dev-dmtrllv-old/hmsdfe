#pragma once

#include "pch.hpp"
#include "tasky/task.hpp"

namespace ion::async
{
	tasky::Task<std::string> readFile(std::string_view path);

	// class File
	// {
	// public:
	// 	File(const std::string& path);
	// 	File(const File&) = delete;
	// 	File(File&&) = delete;
	// 	~File();

	// 	void read();

	// 	const std::string path;

	// private:
	// 	HANDLE fileHandle_;
	// 	HANDLE ioPort_;
	// 	std::string buffer_;

	// };
	/*
	int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE main_io,file_i;
	HANDLE file_handle;
	
	DWORD bytes_recvd;
	ULONG_PTR data = 0;
	OVERLAPPED overlapped;
	LPOVERLAPPED poverlapped = &overlapped;
	uint8_t read_data[1024];
	DWORD err;

	main_io = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	assert(main_io != NULL);

	file_handle = CreateFile(L"test.txt",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED, NULL);
	assert(file_handle != INVALID_HANDLE_VALUE);

	file_i = CreateIoCompletionPort(file_handle,main_io,data,0);
	assert(file_i != NULL);

	memset(&overlapped,0,sizeof(OVERLAPPED));

	err = ReadFileEx(file_handle,(LPVOID)read_data,1024,&overlapped,ReadCb);
	fprintf(stderr,"err %d\n",GetLastError());
	assert(err != 0);

	assert(GetQueuedCompletionStatus(file_i,&bytes_recvd,&data,&poverlapped,INFINITE));

	CloseHandle(main_io);
	return 0;
}
	*/
}