#include "MedicalDataParser.h"
#include <fstream>
#include <functional>
#include <json/json.h>
#include <iostream>
#include <Windows.h>
#include "util.h"

constexpr const char* ERR_INVALID_JSON = "Invalid JSON file.";

constexpr const wchar_t *KEY_COMPLAINT = L"����", *KEY_HISTORY = L"�ֲ�ʷ", *KEY_FORMER = L"����ʷ",
*KEY_COMPLAINT_ENTITY = L"����ʵ��", *KEY_HISTORY_ENTITY = L"�ֲ�ʷʵ��", *KEY_FORMER_ENTITY = L"����ʷʵ��";

namespace {
	typedef MedicalDataEntry::Keyword Keyword;
	typedef MedicalDataEntry::KeywordType KeywordType;

	typedef std::function<void(const Keyword&)> ExtractKeywordCallback;

	std::string extractString(const Json::Value& value, const std::string& key) {
		std::string result;
		//std::string utf8Key = ansiTo
		if (value.isMember(key)) {
			const Json::Value target = value[key];
			if (target.isString()) {
				 result = utf8ToAnsiString(target.asString());
			}
		}
		return result;
	}

	void extractKeywordList(const Json::Value& value, const std::string& key, ExtractKeywordCallback callback) {
		if (value.isMember(key)) {
			const Json::Value list = value[key];
			if (list.isArray()) {
				int size = (int)list.size();
				for (int i = 0; i < size; ++i) {
					const Json::Value element = list[i];
					if (element.isString()) {
						std::string s = utf8ToAnsiString(element.asString());
						std::string name, present;
						int start, end;

						start = 2;
						end = (int)s.find('\t');
						name = s.substr(start, end - start);

						end = (int)s.size() - 1;
						start = (int)s.rfind('\t') + 3;
						present = s.substr(start, end - start + 1);

						bool isPresent = present == "present";
						KeywordType type = isPresent ? KeywordType::PRESENT : KeywordType::ABSENT;
						callback(Keyword(name, type));
					}
				}
			}
		}
	}

	
}

std::vector<MedicalDataEntry> parseMedicalDataFile(const std::string path) {
	bool funcResult;
	std::vector<MedicalDataEntry> result;
	Json::CharReaderBuilder rbuilder;
	Json::Value root;
	std::string rawString;
	std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
	std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
	std::string errs;
	//funcResult = reader->parse(convertedString.c_str(), convertedString.c_str() + convertedString.size(), &root, &errs);
	funcResult = Json::parseFromStream(rbuilder, ifs, &root, &errs);
	if (!funcResult) {
		throw JsonParseError(errs);
	}
	ifs.close();

	const Json::Value::Members rootMembers = root.getMemberNames();
	if (rootMembers.size() != 1) {
		throw JsonParseError(ERR_INVALID_JSON);
	}
	const std::string name = rootMembers[0];
	
	const Json::Value dataList = root[name];
	if (dataList.type() != Json::ValueType::arrayValue) {
		throw JsonParseError(ERR_INVALID_JSON);
	}
	for (int i = 0; i < (int)dataList.size(); ++i) {
		const Json::Value entryValue = dataList[i];
		std::string complaint = extractString(entryValue, wideToUtf8String(KEY_COMPLAINT)), 
			history = extractString(entryValue, wideToUtf8String(KEY_HISTORY)),
			former = extractString(entryValue, wideToUtf8String(KEY_FORMER));
		MedicalDataEntry entry(complaint, history, former);
		extractKeywordList(entryValue, wideToUtf8String(KEY_COMPLAINT_ENTITY), [&entry](const Keyword& keyword) {
			entry.addComplaintKeyword(keyword);
			});
		extractKeywordList(entryValue, wideToUtf8String(KEY_HISTORY_ENTITY), [&entry](const Keyword& keyword) {
			entry.addHistoryKeyword(keyword);
			});
		extractKeywordList(entryValue, wideToUtf8String(KEY_FORMER_ENTITY), [&entry](const Keyword& keyword) {
			entry.addFormerKeyword(keyword);
			});
		result.push_back(entry);
	}
	return result;
}
