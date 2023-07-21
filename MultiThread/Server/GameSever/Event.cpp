
#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

// CV는 User_Level Object
// * 커널오브젝트는 다른 프로그램이랑 교류 가능

condition_variable cv;

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수를 통해 다른 쓰레드에게 통지

		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		// 깨우기
		cv.notify_one(); // wait 중인 쓰레드가 있으면 딱 1개를 깨운다.
		//::SetEvent(handle);
		//this_thread::sleep_for(100000ms);
	}
}

void Consumer()
{
	while (true)
	{
		// 대기
		//::WaitForSingleObject(handle, INFINITE);

		unique_lock<mutex> lock(m);
		cv.wait(lock, []() { return q.empty() == false; }); // -> 첫번째 매개변수는 무조건 unique lock => 수동으로 풀어줄 수 있어야하기 때문에
		// 1) Lock을 잡고
		// 2) 조건 확인
		// - 만족 => 빠져 나와서 이어서 코드를 진행
		// - 불만 => Lock을 풀어주고 대기상태

		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();

			cout << data << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트 ??
	// Usage Count
	// Signal / Non Signal
	// Auto / Manual

	handle = ::CreateEvent(NULL/*보안속성*/, FALSE, FALSE, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}
