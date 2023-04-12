#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include "MedicalDataEntry.h"

class JsonParseError : public std::runtime_error {
public:
	JsonParseError(const std::string& message) : std::runtime_error(message) {}
};

std::vector<MedicalDataEntry> parseMedicalDataFile(const std::string path);