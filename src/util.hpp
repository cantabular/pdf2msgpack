#pragma once

#include <string>
#include <locale>
#include <codecvt>

template <typename T>
std::string toUTF8(
	const std::basic_string<T, std::char_traits<T>, std::allocator<T>> &source
) {
	std::string result;

	std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
	result = convertor.to_bytes(source);

	return result;
}

// toUTF8 returns the whole string in UTF-8.
std::string toUTF8(Unicode *u, int n) {
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
	std::u32string s(reinterpret_cast<const char32_t*>(w->getChar(i)), 1);
	return toUTF8(s);
}
