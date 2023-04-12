#include "MedicalParsedData.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include "util.h"

typedef MedicalDataEntry::Keyword Keyword;
typedef MedicalDataEntry::KeywordType KeywordType;

namespace {
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

	std::string commentString(const std::string& raw, const std::vector<Keyword>& keywordList) {
		std::wstring s = ansiToWideString(raw);
		std::wstring sicase = s;
		std::transform(sicase.begin(), sicase.end(), sicase.begin(), tolower);
		int* absent = new int[s.size() + 1];
		int* present = new int[s.size() + 1];
		memset(absent, 0, sizeof(int) * (s.size() + 1));
		memset(present, 0, sizeof(int) * (s.size() + 1));
		for (const Keyword& keyword : keywordList) {
			std::wstring ws = ansiToWideString(keyword.getName());
			std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
			int start = 0, pos, size = (int)ws.size();
			while ((pos = (int)sicase.find(ws, start)) != std::string::npos) {
				if (keyword.getType() == KeywordType::ABSENT) {
					++absent[pos];
					--absent[pos + size];
				}
				else {
					++present[pos];
					--present[pos + size];
				}
				start = pos + size;
			}
		}

		std::wstring result;
		KeywordType prev = KeywordType::NORMAL;
		int sumPresent = 0, sumAbsent = 0;
		for (int i = 0; i <= (int)s.size(); ++i) {
			sumAbsent += absent[i];
			sumPresent += present[i];
			if (absent[i] || present[i]) {
				int sum = sumAbsent + sumPresent;
				if (sum == 0) {
					if (prev != KeywordType::NORMAL) {
						prev = KeywordType::NORMAL;
						result += L"\033[0m";
					}
				}
				else if (sum >= 2) {
					if (prev != KeywordType::MIXED) {
						prev = KeywordType::MIXED;
						result += L"\033[0;33m";
					}
				}
				else if (sumAbsent) {
					if (prev != KeywordType::ABSENT) {
						prev = KeywordType::ABSENT;
						result += L"\033[0;31m";
					}
				}
				else if (sumPresent) {
					if (prev != KeywordType::PRESENT) {
						prev = KeywordType::PRESENT;
						result += L"\033[0;32m";
					}
				}
			}

			if (i != s.size()) {
				result.push_back(s[i]);
			}
		}

		delete[] absent;
		delete[] present;

		std::string finalResult = wideToAnsiString(result);
		trim(finalResult);
		return finalResult;
	}
}

const std::string& MedicalParsedData::getComplaint() const
{
	return complaint;
}

const std::string& MedicalParsedData::getHistory() const
{
	return history;
}

const std::string& MedicalParsedData::getFormer() const
{
	return former;
}

MedicalParsedData MedicalParsedData::parse(const MedicalDataEntry& entry)
{
	const std::vector<Keyword> &complaintKeywordList = entry.getComplaintKeywordList(),
		&historyKeywordList = entry.getHistoryKeywordList(),
		&formerKeywordList = entry.getFormerKeywordList();
	const std::string &complaint = entry.getComplaint(),
		&history = entry.getHistory(),
		&former = entry.getFormer();
	
	std::string commentComplaint = commentString(complaint, complaintKeywordList),
		commentHistory = commentString(history, historyKeywordList),
		commentFormer = commentString(former, formerKeywordList);

	return MedicalParsedData(commentComplaint, commentHistory, commentFormer);
}