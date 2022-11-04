#pragma once

#include <string>

#include <optional>
#include <type_traits>
#include <cinttypes>
#include <cctype>
#include <string>
#include <limits>
#include <ranges>

#include "../Core/libs/magic_enum.hpp"

#define MINI_CASE_SENSITIVE
#include "../Core/libs/ini/ini.h"
#include "InputCoreSystem/KeyStruct.h"
#include "InputCoreSystem/NameMapping/ActionMapping.h"

template<typename T, typename _ = void>
struct is_container : std::false_type {};

template<typename... Ts>
struct is_container_helper {};

template<typename T>
struct is_container<
	T,
	std::conditional_t<
	false,
	is_container_helper<
	typename T::value_type,
	typename T::size_type,
	typename T::allocator_type,
	typename T::iterator,
	typename T::const_iterator,
	decltype(std::declval<T>().size()),
	decltype(std::declval<T>().begin()),
	decltype(std::declval<T>().end()),
	decltype(std::declval<T>().cbegin()),
	decltype(std::declval<T>().cend())
	>,
	void
	>
> : std::true_type{};

class IniReaderWriter
{
public:
	using const_iterator = mINI::INIMap<mINI::INIMap<std::string>>::const_iterator;

	IniReaderWriter(const std::string& filename);

	void SetFileName(const std::string& filename);

	void RemoveKey(const std::string& section, const std::string& name);

	void RemoveSection(const std::string& section);

	const_iterator Begin();

	const_iterator End() const;

	void Clear();

	//initializer_list scope
	template <class T>
	void SetValue(const std::string& section, const std::string& name, const std::initializer_list<T>& value_array)
	{
		if (!file.read(ini))
			ini.clear();

		std::stringstream ss;

		for (auto it = value_array.begin(); it != value_array.end(); ++it) {
			if (it != value_array.begin()) {
				ss << ",";
			}
			ss << ToString(*it);
		}

		ini[section][name] = ss.str();
		file.write(ini);
	}

	//container scope
	template <class T, std::enable_if_t<!std::is_same_v<std::string, T>&& is_container<T>::value, bool> = true>
	void SetValue(const std::string& section, const std::string& name, const T& value_array)
	{
		if (!file.read(ini))
			ini.clear();

		std::stringstream ss;

		for (auto it = value_array.begin(); it != value_array.end(); ++it) {
			if (it != value_array.begin()) {
				ss << ",";
			}
			ss << ToString(*it);
		}

		ini[section][name] = ss.str();
		file.write(ini);
	}

	//string scope

	void SetValue(const std::string& section, const std::string& name, const std::string& value)
	{
		if (!file.read(ini))
			ini.clear();

		ini[section][name] = ToString(value);
		file.write(ini);
	}

	//numbers,enums scope
	template <class T, std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, bool> = true>
	void SetValue(const std::string& section, const std::string& name, T value)
	{
		if (!file.read(ini))
			ini.clear();

		ini[section][name] = ToString(value);
		file.write(ini);
	}

	//container scope
	template <template<class, class> class TContainer, class TObject,
		std::enable_if_t<!std::is_same_v<TContainer<TObject, std::allocator<TObject>>, std::string>&& is_container<TContainer<TObject, std::allocator<TObject>>>::value, bool> = true>
	TContainer<TObject, std::allocator<TObject>> GetValue(const std::string& section, const std::string& name)
	{
		//update ini structure
		if (!file.read(ini))
		{
			ini.clear();
			return {};
		}

		const bool has_section = ini.has(section);
		const bool has_key = has_section ? ini[section].has(name) : false;

		if (!has_key)
			return {};

		TContainer<TObject, std::allocator<TObject>> arr_result;

		std::istringstream ss(ini[section][name]);
		std::string token;

		while (std::getline(ss, token, ',')) {
			std::optional<TObject> StrFromValue = FromString<TObject>(token);
			if (StrFromValue.has_value())
				arr_result.push_back(StrFromValue.value());
		}

		return arr_result;
	}

	template <class T, std::enable_if_t<std::is_same_v<std::string, T> || !is_container<T>::value, bool> = true>
	std::optional<T> GetValue(const std::string& section, const std::string& name)
	{
		//update ini structure
		if (!file.read(ini))
		{
			ini.clear();
			return std::nullopt;
		}

		const bool has_section = ini.has(section);
		const bool has_key = has_section ? ini[section].has(name) : false;

		if (!has_key)
			return std::nullopt;

		return FromString<T>(ini[section][name]);
	}

public:

	//string scope
	template <class T, std::enable_if_t<!std::is_same_v<std::string, T>&& std::is_class_v<T>, bool> = true>
	std::string ToString(const T& value)
	{
		std::ostringstream str1;
		str1 << value;
		return str1.str();
	}

	//string scope
	template <class T, std::enable_if_t<std::is_same_v<std::string, T>, bool> = true>
	std::string ToString(const T& value)
	{
		std::ostringstream str1;
		str1 << value;
		return str1.str();
	}

	//number scope
	template <class T, std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<bool, T>, bool> = true>
	std::string ToString(T value)
	{
		std::ostringstream str1;
		str1 << value;
		return str1.str();
	}

	//bool scope
	template <class T, std::enable_if_t<std::is_same_v<bool, T>, bool> = true>
	std::string ToString(T value)
	{
		std::ostringstream str1;
		str1 << std::boolalpha << value;
		return str1.str();
	}

	template <class T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
	std::string ToString(T value)
	{
		return std::string(magic_enum::enum_name(value));
	}


	//FKey scope
	template <class T>
	std::enable_if_t<std::is_same_v<T, FKey>, std::optional<FKey>> FromString(
		const std::string& str)
	{
		T Key(str);
		if (Key.IsValid())
			return Key;
		return std::nullopt;
	}

	//FInputAxisKeyMapping scope
	template <class T>
	std::enable_if_t<std::is_same_v<T, FInputAxisKeyMapping>, std::optional<FInputAxisKeyMapping>> FromString(
		const std::string& str)
	{
		auto tokenizer = [](std::string const& s, char del, std::vector<std::string>& words_out)
		{
			words_out.clear();
			std::stringstream ss(s);
			std::string word;
			while (!ss.eof()) {
				getline(ss, word, del);
				words_out.push_back(word);
			}
		};
		std::vector<std::string> parameters;
		tokenizer(str, '/', parameters);
		auto KeyOpt = FromString<FKey>(parameters[0]);
		auto ScaleOpt = FromString<float>(parameters[1]);
		if (KeyOpt.has_value() && ScaleOpt.has_value())
			return T(KeyOpt.value(), ScaleOpt.value());
		return std::nullopt;
	}


	//enum scope
	template <class T>
	std::enable_if_t<std::is_enum_v<T>, std::optional<T>> FromString(
		const std::string& str)
	{
		return magic_enum::enum_cast<T>(str);
	}

	//string scope
	template <class T>
	std::enable_if_t<std::is_same_v<std::string, T>, std::optional<T>> FromString(
		const std::string& str)
	{
		std::istringstream is(str);
		T out;
		is >> out;
		return out;
	}

	//float scope
	template <class T>
	std::enable_if_t<std::is_floating_point_v<T>, std::optional<T>> FromString(const std::string& str)
	{
		const char* ptr = str.c_str();
		char* endptr;

		long double ans = std::strtold(ptr, &endptr);

		//invalid to_integer argument
		if (ptr == endptr)
			return std::nullopt;

		//invalid to_integer argument
		while (*endptr != '\0')
		{
			if (std::isspace(*endptr) == 0)
			{
				return std::nullopt;
			}
			++endptr;
		}

		//to_integer argument out of range"
		if (ans < std::numeric_limits<T>::lowest() ||
			ans >(std::numeric_limits<T>::max)())
		{
			return std::nullopt;
		}

		return static_cast<T>(ans);
	}

	//int scope
	template <class T>
	std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<bool, T>, std::optional<T>> FromString(const std::string& str)
	{
		const char* ptr = str.c_str();
		char* endptr;

		std::intmax_t ans = std::strtoimax(ptr, &endptr, 10);

		//invalid to_integer argument
		if (ptr == endptr)
			return std::nullopt;

		//invalid to_integer argument
		while (*endptr != '\0')
		{
			if (std::isspace(*endptr) == 0)
			{
				return std::nullopt;
			}
			++endptr;
		}

		//to_integer argument out of range"
		if (ans < std::numeric_limits<T>::lowest() ||
			ans >(std::numeric_limits<T>::max)())
		{
			return std::nullopt;
		}

		return static_cast<T>(ans);
	}

	//bool scope
	template <class T>
	std::enable_if_t<std::is_same_v<bool, T>, std::optional<T>> FromString(const std::string& str)
	{
		if (str == "true")
			return true;
		if (str == "false")
			return true;
		return std::nullopt;
	}

protected:
	mINI::INIFile file;

	// create a data structure
	mINI::INIStructure ini;
};
