#pragma once
#include <stdio.h>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <iostream>

using namespace std;

namespace delayTimer {

	class dTimObj {
	private:
		void* actOb{}, (*actF)(void*) {};
		unsigned int timer{}, timer_runner{};
		uint64_t endTime{};

		bool ready_planner{};
		volatile bool* ready_runner{};

		static inline uint64_t getTime() {
			using namespace std::chrono;
			return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		}

		struct con_Str {
			mutex mut;
			condition_variable con;
			template <class readyType>
			void wait(readyType ready, unique_lock<mutex>& lk) { con.wait(lk, [&] { return ready; }); }
		} wCon_planner, wCon_runner;

		void plannerF() {
			lock_guard<mutex> lk(wCon_runner.mut);
			if (!*ready_runner || !(timer_runner = (unsigned int)(getTime() + timer - endTime)))
				*ready_runner = true,
				endTime = getTime() + timer,
				timer_runner = -1;
		}

		void runnerF() {
			for (unsigned int timSet = timer; ; ) {
				sleep(timSet / 1000); 
				usleep(timSet % 1000);
				lock_guard<mutex> lk(wCon_runner.mut);
				if (timer_runner && (timer_runner != -1 || (actF(actOb), false))) {
					endTime += (timSet = timer_runner);
					continue;
				}
				*ready_runner = false;
				break;
			}
		}

	public:
		
		dTimObj(volatile bool * ready, unsigned int tim) {
			timer = tim;
			*(ready_runner = ready) = ready_planner = 0;
			thread([](void* pObj) 
				{
					for (unique_lock<mutex> lk(((dTimObj*)pObj)->wCon_planner.mut);; ((dTimObj*)pObj)->ready_planner = false) 
						((dTimObj*)pObj)->wCon_planner.wait<bool&>(((dTimObj*)pObj)->ready_planner, lk),
						((dTimObj*)pObj)->plannerF();
				}
			, this).detach();

			thread([](void* pObj) 
				{
					for (;;) {
						{
							unique_lock<mutex> lk(((dTimObj*)pObj)->wCon_runner.mut);
							((dTimObj*)pObj)->wCon_runner.wait<volatile bool&>(*((dTimObj*)pObj)->ready_runner, lk);
							((dTimObj*)pObj)->timer_runner;
						}
						((dTimObj*)pObj)->runnerF();
					}
				}
			, this).detach();
		}

		void set(void(*fP)(void*), void* oP) {
			lock_guard<mutex> lk(wCon_runner.mut);
			actOb = oP;
			actF = fP;
		}

		void setPlannerF() {
			{ lock_guard<mutex> lk(wCon_planner.mut);
			ready_planner = true; }
			wCon_planner.con.notify_one();
		}

		void clearF() {
			lock_guard<mutex>lk(wCon_runner.mut);
			timer_runner = 0;
		}	
	};
}