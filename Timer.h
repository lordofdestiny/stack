#pragma once
#include <chrono>
#include <format>
#include <iostream>
#include <type_traits>

namespace Utils {
	template<class T, typename... REST>
	struct is_any_of {
		static const inline bool value = (... || std::is_same_v<T, REST>);
	};

	template<class T, typename... REST>
	static constexpr bool is_any_of_v = is_any_of<T, REST...>::value;

	template<typename T>
	concept ChronoTimeType = is_any_of_v<T,
		std::chrono::seconds,
		std::chrono::milliseconds,
		std::chrono::microseconds,
		std::chrono::nanoseconds>;

	template<typename T> static constexpr auto TimeTypeUnit = "Unknown";
	template<> static constexpr auto TimeTypeUnit< std::chrono::seconds> = "s";
	template<> static constexpr auto TimeTypeUnit< std::chrono::milliseconds> = "ms";
	template<> static constexpr auto TimeTypeUnit< std::chrono::microseconds> = "us";
	template<> static constexpr auto TimeTypeUnit< std::chrono::nanoseconds> = "ns";

	template<ChronoTimeType T = std::chrono::milliseconds>
	class Timer final {
	private:
		using clock = std::chrono::high_resolution_clock;
		using TTimePoint = clock::time_point;
		using TTimeDuration = T;
		static inline size_t s_id_counter = 0;
	public:
		Timer(std::ostream& os = std::cout) : Timer("Timer", os) {}
		Timer(std::string const& name, std::ostream& os = std::cout)
			:name(name), start(clock::now()), os(os) {
			os << std::format("{}({}) started!\n", name, id);
		}
		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

		auto timeElapsed() const {
			auto end = this->stopped ? this->end : clock::now();
			auto duration = std::chrono::duration_cast<TTimeDuration>(end - start);
			return duration.count();
		}
		void stop() {
			if (stopped) return;
			this->end = clock::now();
			this->stopped = true;
			os << std::format("{}({}) ended!\n", name, id);
			display();
		}

		void display() const {
			display(this->os);
		}

		void display(std::ostream& os) const {
			os << std::format("{}({}) - {}{}\n", name, id, timeElapsed(), TimeTypeUnit<TTimeDuration>);
		}

		~Timer() {
			if (!stopped) stop();
		}

		friend std::ostream& operator<<(std::ostream& os, Timer<T> const& timer) {
			timer.display(os);
			return os;
		}
	private:
		TTimePoint start, end{};
		bool stopped = false;
		std::string name;
		size_t id = s_id_counter++;
		std::ostream& os;
	};
}