#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>

class ThreadPool {

	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	std::once_flag flag;
	std::mutex queue_mutex;
	std::condition_variable cv;

	bool stop;

public:
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;


	ThreadPool(int&& num) :stop(false) {
		std::call_once(flag, [this, num]() {
			for (int i = 0; i < num; ++i) {
				threads.emplace_back([this]() {
					while (true) {
						std::unique_lock <std::mutex> lock(queue_mutex);
						cv.wait(lock, [this] {return !tasks.empty() || this->stop; });
						if (this->stop && this->tasks.empty()) {
							return;
						}
						std::function<void()> task = std::move(this->tasks.front());
						tasks.pop();
						lock.unlock();
						task();
					}
					});
			}
			});

	}


	~ThreadPool() {
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
		lock.unlock();
		cv.notify_all();
		for (std::thread& worker : threads) {
			if (worker.joinable()) {
				worker.join();
			}
		}
	}

	template<class F, class... Args>
	void enqueue(F&& f, Args&&... args) {
		std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		std::unique_lock<std::mutex> lock(queue_mutex);
		tasks.emplace(std::move(task));
		lock.unlock();
		cv.notify_one();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	void set_stop(bool i ) {
		stop = i;
		std::cout << "stop set to " << stop << std::endl;
	}
};

int main() {
	ThreadPool pool(3);
	for (int i = 0; i < 10; ++i) {
		pool.set_stop(true);
		pool.enqueue([i]() {
			std::cout << "Task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
			
			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		});
		
	}
}