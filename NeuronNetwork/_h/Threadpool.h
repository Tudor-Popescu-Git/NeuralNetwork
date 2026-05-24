#include <vector>
#include <deque>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
namespace mythreading {
	typedef std::function<void()> callback;
	class threadpool {
		std::vector<std::thread> threads;
		std::mutex m;
		std::condition_variable cv;
		std::queue<callback, std::deque<callback>> callbackQueue;
		bool _terminate = false;
		void threadExecutor(void);

	public:
		threadpool(size_t count);
		~threadpool();
		void enqueueWork(callback work);
		void terminate();
		void join_with_terminate();
	};
}