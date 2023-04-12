#pragma once

#include <string>

void trim_left(std::string &str);
void trim_right(std::string &str);
void trim(std::string &str);

std::wstring ansiToWideString(const std::string& raw);
std::string wideToAnsiString(const std::wstring& raw);
std::string utf8ToAnsiString(const std::string& raw);
