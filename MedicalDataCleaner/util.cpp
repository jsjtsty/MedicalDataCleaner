#include "util.h"
#include <Windows.h>

void trim_left(std::string &str) {
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

void trim_right(std::string &str) {
	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), str.end());
}

void trim(std::string &str) {
	trim_left(str);
	trim_right(str);
}

std::wstring ansiToWideString(const std::string& raw) {
	int len = MultiByteToWideChar(CP_ACP, 0, raw.c_str(), -1, NULL, 0);
	wchar_t* temp = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, raw.c_str(), -1, temp, len);
	std::wstring result = temp;
	delete[] temp;
	return result;
}

std::string wideToAnsiString(const std::wstring& raw) {
	int len = WideCharToMultiByte(CP_ACP, 0, raw.c_str(), -1, NULL, 0, NULL, NULL);
	char* temp = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, raw.c_str(), -1, temp, len, NULL, NULL);
	std::string result = temp;
	delete[] temp;
	return result;
}

std::string utf8ToAnsiString(const std::string& raw) {
	int len = MultiByteToWideChar(CP_UTF8, 0, raw.c_str(), -1, NULL, 0);
	wchar_t* temp = new wchar_t[len + 1];
	MultiByteToWideChar(CP_UTF8, 0, raw.c_str(), -1, temp, len);
	len = WideCharToMultiByte(CP_ACP, 0, temp, -1, NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, temp, -1, buffer, len, NULL, NULL);
	delete[] temp;

	std::string result = buffer;
	delete[] buffer;

	return result;
}