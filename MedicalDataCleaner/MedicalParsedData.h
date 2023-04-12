#pragma once
#include "MedicalDataEntry.h"

class MedicalParsedData {
public:
	static MedicalParsedData parse(const MedicalDataEntry& entry);

	const std::string& getComplaint() const;
	const std::string& getHistory() const;
	const std::string& getFormer() const;

protected:
	MedicalParsedData(const std::string& complaint, const std::string& history, const std::string& former)
		: complaint(complaint), history(history), former(former) {}

	std::string complaint, history, former;
};

