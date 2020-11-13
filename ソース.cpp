#include <iostream>
#include <mutex>
#include <type_traits>
#include <future>

//maybe this is Semaphore.

struct SemaphoreData {
protected:
	std::mutex M;
};

template<class F>
class Semaphore :public SemaphoreData{

public:

	Semaphore() = delete;
	Semaphore(F In) :Fun(In) {}
/** /
	bool Lock() {
		M.lock();
		return true;
	}
	bool UnLock() {
		M.unlock();
		return true;
	}
/**/
	template<class... As>
	auto operator() (const As&... In) {
		std::lock_guard<std::mutex> S{ M };
		return Fun(In...);
	}

protected:
	F Fun;
};

template<class F>
Semaphore<F> MakeSemaphore(F In) {
	return Semaphore<F>(In);
}

int main() {
	int i = 0;
	auto S = MakeSemaphore([&]()->bool {std::cout << i++ << std::endl; return true; });

	for (std::size_t X = 0; X < 1024; X++) {
		std::async(std::launch::async, [&]() {	auto S = MakeSemaphore([&]()->bool {std::cout << i++ << std::endl; return true; }); S(); });
	}
	return 0;

}