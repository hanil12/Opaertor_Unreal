
#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>


// CPU ����������
// 1. Fetch
// 2. Decode
// 3. Execute
// 4. Write-back

// ���� �����尡 ������ �޸𸮿� ���� ����, write ������ �� Race�� �Ͼ��.
// => Lock (Mutual Exclusive ����)
// => Atomic (������ ������ �̿�)

// atomic ���꿡 ����, ��� �����尡 ���� ��ü�� ���ؼ� ������ ���� ������ ����
// ���ŷδ� �� �� ����.

// ���ü� : ���̴� �� �״�� ������ �ֳ�? -> ĳ�ÿ� �ִ� ������ �� �ִ�.
// �ڵ� ���ġ : �����Ϸ��� CPU���� �ڵ带 ����ȭ�ϸ鼭 ���ġ�� �� �ִ�.

atomic<bool> flag;

atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;
	ready.store(true, memory_order::memory_order_seq_cst);
}

void Consumer()
{
	while(ready.load(memory_order_seq_cst) == false)
	;

	cout << value << endl;
}

int main()
{
	flag = false;

	flag.store(true, memory_order::memory_order_seq_cst);

	bool val = flag.load(memory_order::memory_order_seq_cst);

	// ���� flag  ���� prev�� �ְ�, flag ���� ����
	{
		bool prev = flag.exchange(true);


		flag = true;
	}

	// CAS (Compare And Swap) ���Ǻ� ����
	//{
	//	bool expected = false;
	//	bool desired = true;
	//	flag.compare_exchange_strong(expected, desired);

	//	if (flag == expected)
	//	{
	//		flag = desired;
	//		return true;
	//	}
	//	else
	//	{
	//		expected = flag;
	//		return false;
	//	}
	//}





	// Memomry Model
	// 1. Sequentially Consistent(seq_cst)
	// 2. Acquire-Release(cquire, release)
	// 3. Relaxed(relaxed)

	// 1) seq_cst(���� ���� = �����Ϸ� ����ȭ ���� ���� = ������)
	//	���ü� ���� ��� �ذ�! �ڵ� ���ġ �ٷ� �ذ�!
	// 2) acquire-release
	//	�� �߰�
	//	release ��� ������ �޸� ��ɵ��� �ش� ��� ���ķ� ���ġ �Ǵ� ���� ����
	//	�׸��� acquire�� ���� ������ �д� �����尡 �ִٸ�
	//	release ������ ��ɵ��� -> acquire �ϴ� ������ ���� ����(���ü� ����)
	// 
	// 3) relaxed(�����Ӵ� = �����Ϸ� ����ȭ ���� ���� = ���������� ����)
	//	�ʹ����� �����Ӵ�
	//	�ڵ� ���ġ�� �ڴ��
	//	���� �⺻����(���� ��ü�� ���� ���� ���� ����)

	// ����, AMD�� ��� �ִ��� ������ �ϰ����� ����
	// => seq_cst�� �ᵵ ���ٸ� ���ϰ� ����
	// AMD���� ��� ���ϰ� ���� �� ����.

	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
}
