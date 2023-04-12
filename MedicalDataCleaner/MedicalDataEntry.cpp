#include "MedicalDataEntry.h"

const std::string& MedicalDataEntry::Keyword::getName() const 
{
    return name;
}

MedicalDataEntry::KeywordType MedicalDataEntry::Keyword::getType() const 
{
    return type;
}

bool MedicalDataEntry::Keyword::operator==(const Keyword& other) const 
{
    return name == other.name && type == other.type;
}

bool MedicalDataEntry::Keyword::operator<(const Keyword& other) const 
{
    return name == other.name ? type < other.type : name < other.name;
}

bool MedicalDataEntry::Keyword::operator>(const Keyword& other) const 
{
    return name == other.name ? type > other.type : name > other.name;
}

bool MedicalDataEntry::Keyword::operator<=(const Keyword& other) const 
{
    return name == other.name ? type <= other.type : name <= other.name;
}

bool MedicalDataEntry::Keyword::operator>=(const Keyword& other) const 
{
    return name == other.name ? type >= other.type : name >= other.name;
}

void MedicalDataEntry::addComplaintKeyword(const Keyword& keyword) 
{
    complaintKeywordList.push_back(keyword);
}

void MedicalDataEntry::addHistoryKeyword(const Keyword& keyword) 
{
    historyKeywordList.push_back(keyword);
}

void MedicalDataEntry::addFormerKeyword(const Keyword& keyword) 
{
    formerKeywordList.push_back(keyword);
}

const std::vector<MedicalDataEntry::Keyword>& MedicalDataEntry::getComplaintKeywordList() const 
{
    return complaintKeywordList;
}

const std::vector<MedicalDataEntry::Keyword>& MedicalDataEntry::getHistoryKeywordList() const
{
    return historyKeywordList;
}

const std::vector<MedicalDataEntry::Keyword>& MedicalDataEntry::getFormerKeywordList() const
{
    return formerKeywordList;
}

const std::string& MedicalDataEntry::getComplaint() const
{
    return complaint;
}

const std::string& MedicalDataEntry::getHistory() const
{
    return history;
}

const std::string& MedicalDataEntry::getFormer() const
{
    return former;
}
