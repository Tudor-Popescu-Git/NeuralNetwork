#ifndef H_BARRIER_H_
#define H_BARRIER_H_

#include <vector>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace mythreading {
	typedef std::thread::id thread_id;

	class barrier {
		size_t threads_count;
		std::condition_variable cv;
		std::mutex m;
		std::vector<thread_id> tids;
		size_t generation;
		bool incrementing = true;
		decltype(tids)::iterator find_tid(thread_id tid);
		void acqLock();
	public:
		barrier(size_t threads_count);
		void wait();

	};
}
#endif