#pragma once
#include <string>
#include <vector>

class MedicalDataEntry {
public:
	MedicalDataEntry(const std::string& complaint, const std::string& history, const std::string& former) :
		complaint(complaint), history(history), former(former) {}

	enum class KeywordType {
		NORMAL,
		PRESENT,
		ABSENT,
		MIXED
	};

	class Keyword {
	protected:
		std::string name;
		KeywordType type;
	public:
		Keyword(const std::string& name, const KeywordType type) : name(name), type(type) {}
		const std::string& getName() const;
		KeywordType getType() const;

		bool operator==(const Keyword&) const;
		bool operator<(const Keyword&) const;
		bool operator>(const Keyword&) const;
		bool operator<=(const Keyword&) const;
		bool operator>=(const Keyword&) const;
	};

	void addComplaintKeyword(const Keyword& keyword);
	void addHistoryKeyword(const Keyword& keyword);
	void addFormerKeyword(const Keyword& keyword);

	const std::vector<Keyword>& getComplaintKeywordList() const;
	const std::vector<Keyword>& getHistoryKeywordList() const;
	const std::vector<Keyword>& getFormerKeywordList() const;

	const std::string& getComplaint() const;
	const std::string& getHistory() const;
	const std::string& getFormer() const;

protected:
	std::vector<Keyword> complaintKeywordList, historyKeywordList, formerKeywordList;
	std::string complaint, history, former;
};

