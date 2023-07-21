
#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

// SpinLock
class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare - And - Swap) =
		bool expected = false;
		bool desired = true;

		// locked == false..
		// lock = desired로 교체
		// true 반환

		// locked == true...
		// expected = locked
		// false 반환

		// => 이러한 단계들이 atomic하게 연산된다... CAS

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			//this_thread::sleep_for(std::chrono::milliseconds(100));
			//this_thread::sleep_for(100ms);
			this_thread::yield(); // 커널모드로 돌아가서 알아서 스케쥴링해라
		}
	}

	void unlock()
	{
		_locked.store(false);
	}

private:
	// volatile : 컴파일한테 최적화하지 말아달라.
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 100000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}
