
#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'00000; i++)
	{
		sum += i;
	}

	return sum;
}

void PromiseWork(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWork(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// 동기(synchronous) 실행
	// 비동기(asynchronous) 실행

	//int64 sum = Calculate();
	//cout << sum << endl;

	// std::future
	{
		// 1) deferred -> lazy evaluation : 지연해서 실행
		// 2) async -> 별도의 쓰레드를 만들어서 실행
		// 3) deferred | async -> 둘 중 알아서
		// 언젠가 미래에 결과물을 뱉어줄거야(지연 or 비동기)
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// TODO
		auto status = future.wait_for(1ms);
		if (status == future_status::ready)
		{
			cout << "준비 완료" << endl;
		}

		int64 sum = future.get(); // 결과물이 이제서야 필요하다.
	}

	// std::promise
	{
		// 미래(future)에 결과물을 반환해줄거라 약속(promise) 해줘
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWork, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		auto future = task.get_future();

		std::thread t(TaskWork, std::move(task));

		int64 sum = future.get();

		cout << sum << endl;
		t.join();
	}

	// mutexm condition_valuable까지 가지 않고 단순한 얘들을 처리할 수 있는 멀티쓰레딩프로그래밍
	// 1. async
	// 원하는 함수를 비동기적으로 실행
	// 2. promise
	// 결과물을 promise를 통해 future로 받아줌
	// 3. packaged_task
	// 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌
}
