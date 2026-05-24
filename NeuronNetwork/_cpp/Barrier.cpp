#include "stdafx.h"
#include "../_h/Barrier.h"
#include <vector>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;
namespace mythreading {
	auto barrier::find_tid(thread_id tid) -> decltype(tids)::iterator{
		return find_if(tids.begin(), tids.end(), [tid](const decltype(tids)::value_type val) { return val == tid; });
	}
		/*
		void barrier::acqLock() {
			stringstream ss;
			unique_lock<mutex> lk(m, std::defer_lock);
			static int iter = 0;
			static int counter = 0;
			size_t _generation = generation;
			auto THREAD_ID = this_thread::get_id();
			do {
				lk.lock();
				auto found = find_tid(THREAD_ID);
				if (incrementing) {
					if (found == tids.end()) {
						tids.push_back(THREAD_ID);

					}
					if (tids.size() == threads_count) {
						incrementing = false;
						ss.str("");
						ss << "---------iter = " << ++iter << "----------" << endl;
						cout << ss.str();
						if ((iter % 10) == 0) {
							ss.str("");
							//ss << "---------iter = " << ++iter << "----------" << endl;
							//cout << ss.str() << std::flush;
						}
						cv.notify_all();
					}
					else {
						while (tids.size() != threads_count && incrementing) {
							cv.wait(lk);
						}
					}
				}
				else
				{
					if (found != tids.end()) {
						tids.erase(found);
					}
					if (tids.size() == 0) {
						incrementing = true;
						//cout << "freeing thread with id = " << this_thread::get_id() << endl;
						cv.notify_all();
						break;
					}
					else
					{
						while (tids.size() != 0 && !incrementing) {
							cv.wait(lk);
						}
						//cv.wait(lk, [&]() {return tids.size() != 0 && !incrementing; });
						//cout << "freeing thread with id = " << this_thread::get_id() << endl;
						break;
					}
				}
				lk.unlock();
				//cout << "dowhile counter = " << ++counter << endl;
			} while (true);
			lk.unlock();
		}
		*/
	void barrier::acqLock() {
		stringstream ss;
		unique_lock<mutex> lk(m, std::defer_lock);
		static int iter = 0;
		static int counter = 0;
		size_t _generation = generation;
		auto THREAD_ID = this_thread::get_id();

		lk.lock();
		auto found = find_tid(THREAD_ID);
		if (found == tids.end()) {
			tids.push_back(THREAD_ID);

		}
		if (tids.size() == threads_count) {
			ss.str("");
			ss << "---------iter = " << ++iter << "----------" << endl;
			cout << ss.str();
			tids.clear();
			generation++;
			cv.notify_all();
		}
		else {
			while (generation == _generation) {
				cv.wait(lk);
			}
		}
		lk.unlock();
	}
	barrier::barrier(size_t threads_count) :threads_count(threads_count) { tids.reserve(threads_count); }
	void barrier::wait() {
		acqLock();
	}
}
