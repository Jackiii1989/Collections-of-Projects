#pragma once
#include <chrono>
#include <ctime>
template <typename clock_t = std::chrono::steady_clock, typename size_t, typename fun_t, typename ...args_t>
inline auto timed_run(size_t const n, fun_t && fun, args_t && ...args) noexcept (std::is_nothrow_invocable_v <fun_t, args_t...>) {
	static_assert (clock_t::is_steady);
	static_assert (std::is_integral_v <size_t>);

	typename clock_t::duration elapsed{};

	if (0 < n) {
		auto const start{ clock_t::now() };

		for (int i{ 0 }; i < n; ++i) {
			std::invoke(std::forward <fun_t>(fun), std::forward <args_t>(args)...);
		}

		elapsed = (clock_t::now() - start) / n;
	}

	return elapsed;
}