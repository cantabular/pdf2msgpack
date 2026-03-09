#pragma once

#include <string>
#include <locale>
#include <codecvt>

template <typename T>
std::string toUTF8(
	const std::basic_string<T, std::char_traits<T>, std::allocator<T>> &source
) {
	std::string result;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
#pragma GCC diagnostic pop
	result = convertor.to_bytes(source);

	return result;
}

// toUTF8 returns the whole string in UTF-8.
std::string toUTF8(const Unicode *u, int n) {
	std::u32string s;
	for (int i = 0; i < n; i++) {
		s += static_cast<char32_t>(*(u + i));
	}
	return toUTF8(s);
}

// toUTF8 returns the whole string in UTF-8.
std::string toUTF8(const TextWord *w) {
	std::u32string s;
	for (int i = 0; i < w->getLength(); i++) {
		s += static_cast<char32_t>(*w->getChar(i));
	}
	return toUTF8(s);
}

// toUTF8 writes returns character of TextWord w in UTF-8.
std::string toUTF8(const TextWord *w, int i) {
       std::u32string s;
       s += static_cast<char32_t>(*w->getChar(i));
	return toUTF8(s);
}
