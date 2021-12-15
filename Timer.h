#pragma once
#include <chrono>
#include <format>
#include <iostream>

namespace Utils {
	template<typename T>
	concept TimeUnit = std::is_same_v < T, std::chrono::seconds > ||
		std::is_same_v <T, std::chrono::milliseconds> ||
		std::is_same_v <T, std::chrono::microseconds> ||
		std::is_same_v <T, std::chrono::nanoseconds>;

	template<TimeUnit DurationType = std::chrono::milliseconds>
	class Timer {
	private:
		using clock = std::chrono::high_resolution_clock;
		using time_point = clock::time_point;
		using time_duration = DurationType;
	private:
		std::string name;
		time_point start;
		time_point end;
		bool stopped = false;
		std::ostream& os;
	public:
		Timer(std::ostream& os = std::cout) : name(), start(clock::now()), os(os) {
			os << "Timer started!\n";
		}
		Timer(std::string name, std::ostream& os = std::cout) :name(name), start(clock::now()), os(os) {
			os << std::format("Timer {} started!\n", name);
		}
		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

		auto timeElapsed() const {
			time_point end = this->stopped ? this->end : clock::now();
			time_duration duration = std::chrono::duration_cast<time_duration>(end - start);
			return duration.count();
		}

		void stop() {
			if (!stopped) {
				this->end = clock::now();
				this->stopped = true;
			}
		}

		~Timer() {
			stop();
			auto duration = timeElapsed();
			bool nameNotSet = name.empty();
			os << std::format("Timer{} ended!\n", nameNotSet ? "" : (" " + name));
			os << std::format("Execution took {}{}!\n", duration, getTimeUnitString());
		}

		friend std::ostream& operator<<(std::ostream& os, const Timer<DurationType> timer) {
			auto duration = timer->timeElapsed();
			os << std::format("Timer{} ended!\n", name.empty() ? "" : (" " + name));
			os << std::format("Execution took {}{}!\n", duration, timer->getTimeUnitString());
		}

		static consteval const char* getTimeUnitString() {
			if (std::is_same_v<time_duration, std::chrono::seconds>) {
				return "s";
			}
			else if (std::is_same_v<time_duration, std::chrono::milliseconds>) {
				return "ms";
			}
			else if (std::is_same_v<time_duration, std::chrono::microseconds>) {
				return "us";
			}
			else if (std::is_same_v<time_duration, std::chrono::nanoseconds>) {
				return "ns";
			}
			else {
				return "";
			}
		}
	};
}