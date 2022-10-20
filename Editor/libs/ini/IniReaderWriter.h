#pragma once

#include <string>

#include <optional>
#include <type_traits>
#include <cinttypes>
#include <cwchar>
#include <cctype>
#include <string>
#include <limits>
#include <type_traits>

#include "ini.h"

class IniReaderWriter final
{
public:
	IniReaderWriter(const std::string& filename);

	template <class T>
	std::optional<T>  GetValue(const std::string& section, const std::string& name);

	//string scope
	template <class T>
	std::enable_if_t <!std::is_arithmetic_v<T> && !std::is_same_v<bool,T>, std::optional<T>> FromString(const std::string& str);

	//float scope
	template<class T>
	std::enable_if_t<std::is_floating_point_v<T>, std::optional<T>> FromString(const std::string& str);

	//int scope
	template<class T>
	std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<bool, T>, std::optional<T>> FromString(const std::string& str);

	//bool scope
	template <class T>
	std::enable_if_t<std::is_arithmetic_v<T> && std::is_same_v<bool,T>, std::optional<T >> FromString(const std::string& str);

protected:
	mINI::INIFile file;

	// create a data structure
	mINI::INIStructure ini;
};

template <class T>
std::optional<T> IniReaderWriter::GetValue(const std::string& section,
	const std::string& name)
{
	//update ini structure
	file.read(ini);

	bool const has_section = ini.has(section);
	bool const has_key = has_section ? ini[section].has(name) : false;

	if(!has_key)
		return std::nullopt;
	
	return FromString<T>(ini[section][name]);
}

template <class T>
std::enable_if_t<std::is_floating_point_v<T>, std::optional<T>> IniReaderWriter::FromString(const std::string& str)
{
	const char* ptr = str.c_str();
	char* endptr;

	long double ans = std::strtold(ptr, &endptr);

	//invalid to_integer argument
	if (ptr == endptr)
		return std::nullopt;

	//invalid to_integer argument
	while (*endptr != '\0') {
		if (std::isspace(*endptr) == 0) {
			return std::nullopt;
		}
		++endptr;
	}

	//to_integer argument out of range"
	if (ans < std::numeric_limits<T>::lowest() ||
		ans > (std::numeric_limits<T>::max)()) {
		return std::nullopt;
	}

	return static_cast<T>(ans);
}

template <class T>
std::enable_if_t<std::is_arithmetic_v<T> && std::is_same_v<bool, T>, std::optional<T>> IniReaderWriter::FromString(
	const std::string& str)
{
	if (str == "True")
		return true;
	if (str == "False")
		return true;
	return std::nullopt;
}

template <class T>
std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_same_v<bool, T>, std::optional<T>> IniReaderWriter::FromString(
	const std::string& str)
{
	std::istringstream is(str);
	T out;
	is >> out;
	return out;
}

template <class T>
std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<bool, T>,std::optional<T>> IniReaderWriter::FromString(const std::string& str)
{
	const char* ptr = str.c_str();
	char* endptr;

	std::intmax_t ans = std::strtoimax(ptr, &endptr, 10);
	
	//invalid to_integer argument
	if (ptr == endptr)
		return std::nullopt;

	//invalid to_integer argument
	while (*endptr != '\0') {
		if (std::isspace(*endptr) == 0) {
			return std::nullopt;
		}
		++endptr;
	}

	//to_integer argument out of range"
	if (ans < std::numeric_limits<T>::lowest() ||
		ans > (std::numeric_limits<T>::max)()) {
		return std::nullopt;
	}

	return static_cast<T>(ans);
}
