
#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>


// CPU 파이프라인
// 1. Fetch
// 2. Decode
// 3. Execute
// 4. Write-back

// 여러 쓰레드가 동일한 메모리에 동시 접근, write 연산할 때 Race가 일어난다.
// => Lock (Mutual Exclusive 접근)
// => Atomic (원자적 연산을 이용)

// atomic 연산에 한해, 모든 쓰레드가 동일 객체에 대해서 동일한 수정 순서를 관찰
// 과거로는 갈 수 없다.

// 가시성 : 보이는 거 그대로 정보가 있냐? -> 캐시에 있는 정보일 수 있다.
// 코드 재배치 : 컴파일러나 CPU에서 코드를 최적화하면서 재배치할 수 있다.

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

	// 이전 flag  값을 prev에 넣고, flag 값을 수정
	{
		bool prev = flag.exchange(true);


		flag = true;
	}

	// CAS (Compare And Swap) 조건부 수정
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

	// 1) seq_cst(가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적)
	//	가시성 문제 배로 해결! 코드 재배치 바로 해결!
	// 2) acquire-release
	//	딱 중간
	//	release 명령 이전의 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지
	//	그리고 acquire로 같은 변수를 읽는 쓰레드가 있다면
	//	release 이전의 명령들이 -> acquire 하는 순간에 관찰 가능(가시성 보장)
	// 
	// 3) relaxed(자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	//	너무나도 자유롭다
	//	코드 재배치도 멋대로
	//	가장 기본조건(동일 객체에 대한 동일 순서 보장)

	// 인텔, AMD의 경우 애당초 순차적 일관성을 보장
	// => seq_cst를 써도 별다른 부하가 없음
	// AMD같은 경우 부하가 있을 수 있음.

	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
}
