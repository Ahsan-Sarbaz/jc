#pragma once
#include <string_view>
#include <chrono>

std::string_view read_file_into_string_view(const char* path);

class TimeIt
{
public:
	TimeIt(const std::string& name);
	~TimeIt();

private:
	std::string name;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
#define TIMEIT(name) TimeIt __timeit__(name)

auto get_time() -> std::chrono::time_point<std::chrono::high_resolution_clock>;
auto get_time_diff(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t;
auto get_time_diff_ms(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t;
auto get_time_diff_us(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t;
auto get_time_diff_ns(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t;


