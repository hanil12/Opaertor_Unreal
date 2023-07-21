
#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

// CV�� User_Level Object
// * Ŀ�ο�����Ʈ�� �ٸ� ���α׷��̶� ���� ����

condition_variable cv;

void Producer()
{
	while (true)
	{
		// 1) Lock�� ���
		// 2) ���� ���� ���� ����
		// 3) Lock�� Ǯ��
		// 4) ���Ǻ����� ���� �ٸ� �����忡�� ����

		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		// �����
		cv.notify_one(); // wait ���� �����尡 ������ �� 1���� �����.
		//::SetEvent(handle);
		//this_thread::sleep_for(100000ms);
	}
}

void Consumer()
{
	while (true)
	{
		// ���
		//::WaitForSingleObject(handle, INFINITE);

		unique_lock<mutex> lock(m);
		cv.wait(lock, []() { return q.empty() == false; }); // -> ù��° �Ű������� ������ unique lock => �������� Ǯ���� �� �־���ϱ� ������
		// 1) Lock�� ���
		// 2) ���� Ȯ��
		// - ���� => ���� ���ͼ� �̾ �ڵ带 ����
		// - �Ҹ� => Lock�� Ǯ���ְ� ������

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
	// Ŀ�� ������Ʈ ??
	// Usage Count
	// Signal / Non Signal
	// Auto / Manual

	handle = ::CreateEvent(NULL/*���ȼӼ�*/, FALSE, FALSE, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}
