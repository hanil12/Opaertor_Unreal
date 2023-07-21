#pragma once

#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_s.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);

		if(_s.empty())
			return false;

		value = std::move(_s.top());
		_s.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] { return _s.empty() == false; });
		value = std::move(_s.top());
		_s.pop();
	}

private:
	stack<T> _s;
	mutex _mutex;
	condition_variable _condVar;
};

template <typename T>
class LockFreeStack
{

};