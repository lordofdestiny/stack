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
		static inline size_t s_id_counter = 0;
	private:
		std::string name;
		time_point start, end{};
		bool stopped = false;
		std::ostream& os;
		size_t id = s_id_counter++;
	public:
		Timer(std::ostream& os = std::cout)
			: name(), start(clock::now()), os(os) {
			os << "Timer started!\n";
		}
		Timer(std::string const& name, std::ostream& os = std::cout)
			:name(name), start(clock::now()), os(os) {
			os << std::format("Timer {} started!\n", name);
		}
		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

		auto timeElapsed() const {
			auto end = this->stopped ? this->end : clock::now();
			auto duration = std::chrono::duration_cast<time_duration>(end - start);
			return duration.count();
		}

		void stop() {
			if (!stopped) {
				this->end = clock::now();
				this->stopped = true;
			}
			bool nameNotSet = name.empty();
			os << std::format("Timer{} ended!\n", nameNotSet ? "" : (" " + name));
			display();
		}

		void display() {
			display(this->os);
		}

		void display(std::ostream& os) {
			auto const& name = this->name.empty()
				? std::format("Timer({})", id)
				: this->name;
			
			os << std::format("{} - {}{}\n", name, timeElapsed(), getTimeUnitString());
		}
		
		~Timer() {
			if (!stopped) stop();
		}

		friend std::ostream& operator<<(std::ostream& os, const Timer<DurationType> timer) {
			display(os);
			return os;
		}

		static const char* getTimeUnitString() {
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