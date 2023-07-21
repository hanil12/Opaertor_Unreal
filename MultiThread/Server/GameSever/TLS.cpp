
#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

// TLS
// Thread Local Storage

// 쓰레드 마다 고유하게 들고 있어야하는 정보
thread_local int32 LThread = 0;

void ThreadMain(int32 threadID)
{
	LThread = threadID;

	while (true)
	{
		cout << "Hi! i am Thread" << LThread << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; i++)
	{
		int32 threadID = i + 1;
		threads.push_back(thread(ThreadMain, threadID));
	}

	for (auto& t : threads)
	{
		t.join();
	}
}
