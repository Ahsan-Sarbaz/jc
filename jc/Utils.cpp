#include "Utils.hpp"
#include <iostream>

std::string_view read_file_into_string_view(const char* path)
{
	FILE* file;
	fopen_s(&file, path, "rb");
	if (!file)
	{
		return std::string_view(nullptr, 0);
	}

	fseek(file, 0, SEEK_END);
	auto size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = new char[size + 1];
	if (!buffer)
	{
		fclose(file);
		return std::string_view(nullptr, 0);
	}
	memset(buffer, 0, size + 1ull);
	fread_s(buffer, size, size, 1, file);
	buffer[size] = 0;
	fclose(file);
	return std::string_view(buffer, size);
}

TimeIt::TimeIt(const std::string& name)
	: name(name)
{
	start = get_time();
}

TimeIt::~TimeIt()
{
	std::cout << name << ": " << get_time_diff_ms(start) << " ms" << std::endl;
}

auto get_time() -> std::chrono::time_point<std::chrono::high_resolution_clock>
{
	return std::chrono::high_resolution_clock::now();;
}

auto get_time_diff(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t
{
	return std::chrono::duration_cast<std::chrono::seconds>(get_time() - start).count();
}

auto get_time_diff_ms(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - start).count();
}

auto get_time_diff_us(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t
{
	return std::chrono::duration_cast<std::chrono::microseconds>(get_time() - start).count();
}

auto get_time_diff_ns(std::chrono::time_point<std::chrono::high_resolution_clock> start) -> uint64_t
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(get_time() - start).count();
}
