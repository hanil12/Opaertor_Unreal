
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
	// ����(synchronous) ����
	// �񵿱�(asynchronous) ����

	//int64 sum = Calculate();
	//cout << sum << endl;

	// std::future
	{
		// 1) deferred -> lazy evaluation : �����ؼ� ����
		// 2) async -> ������ �����带 ���� ����
		// 3) deferred | async -> �� �� �˾Ƽ�
		// ������ �̷��� ������� ����ٰž�(���� or �񵿱�)
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// TODO
		auto status = future.wait_for(1ms);
		if (status == future_status::ready)
		{
			cout << "�غ� �Ϸ�" << endl;
		}

		int64 sum = future.get(); // ������� �������� �ʿ��ϴ�.
	}

	// std::promise
	{
		// �̷�(future)�� ������� ��ȯ���ٰŶ� ���(promise) ����
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

	// mutexm condition_valuable���� ���� �ʰ� �ܼ��� ����� ó���� �� �ִ� ��Ƽ���������α׷���
	// 1. async
	// ���ϴ� �Լ��� �񵿱������� ����
	// 2. promise
	// ������� promise�� ���� future�� �޾���
	// 3. packaged_task
	// ���ϴ� �Լ��� ���� ����� packaged_task�� ���� future�� �޾���
}
