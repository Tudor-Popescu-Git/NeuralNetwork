#include "stdafx.h"
#include "../_h/Threadpool.h"
namespace mythreading {
	void threadpool::threadExecutor(void) {
		callback work;
		std::unique_lock<std::mutex> lk(m, std::defer_lock);
		while (true) {
			lk.lock();
			while (!callbackQueue.size()) {
				cv.wait(lk);
			}
			work = std::move(callbackQueue.front());
			callbackQueue.pop();
			lk.unlock();
			work();
			if (_terminate) {
				break;
			}
		}
	}
	threadpool::threadpool(size_t count) {
		threads.reserve(count);
		for (size_t i = 0; i < count; i++) {
			threads.emplace_back(&threadpool::threadExecutor, this);
		}
	}
	threadpool::~threadpool() {
		join_with_terminate();
	}
	void threadpool::enqueueWork(callback work) {
		std::unique_lock<std::mutex> lk(m);
		callbackQueue.push(work);
		cv.notify_one();
	}
	void threadpool::terminate() {
		for (decltype(threads.size()) i = 0; i < threads.size(); i++){
			enqueueWork([=]() {_terminate = true; });
		}
	}
	void threadpool::join_with_terminate() {
		terminate();
		for (decltype(threads.size()) i = 0; i < threads.size(); i++) {
			threads[i].join();
		}
	}
}