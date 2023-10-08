#include <fmt/format.h>

#include <coroutine>
#include <future>
#include <type_traits>

template <typename T>
struct Promise;

template <typename T>
struct Lazy : std::coroutine_handle<Promise<T>> {
	using promise_type = ::Promise<T>;

	bool await_ready() {
		fmt::println("await ready");
		return false;
	}
	int await_resume() {
		fmt::println("await resume");
		return 42;
	}
	void await_suspend(std::coroutine_handle<> cont) {
		fmt::println("await suspend");
		cont.resume();
	}
};

template <typename T>
struct Promise {
	T value;

	Promise(T v) : value(v) {}

	Promise() = default;

	Lazy<T> get_return_object() { return {Lazy<T>::from_promise(*this)}; }

	void return_value(T value) {}

	std::suspend_always initial_suspend() noexcept { return {}; }
	std::suspend_always final_suspend() noexcept { return {}; }
	void				unhandled_exception() {}
};

Lazy<int> GetNumber() { co_return 42; }

struct Noop {
	struct promise_type : std::promise<void> {
		std::suspend_never initial_suspend() noexcept { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void			   return_void() {}
		void			   unhandled_exception() {}

		Noop get_return_object() { return Noop{}; }
	};
};

Noop Launch() {
	auto value = co_await GetNumber();

	fmt::println("Value is: {}", value);
}

int main() {
	Launch();

	fmt::println("Don't {}\n", "panic");
	return 0;
}