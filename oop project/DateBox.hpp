#pragma once
#include "Box.hpp"
#include "Date.hpp"
#include <string>

class DateBox : public Box
{
public:
	// бих предпочела да подаваш данните по отделно, не като обект, или поне предостави и двата варианта
	DateBox(const Date& date);
	DateBox(const std::string& date);

	std::string getDataStr() const override;

	const Date& getDate() const;

	void setDate(const std::string& newDate);

	void print() const override;

	DateBox* clone() const override;

	size_t getLength() const override;

	std::string getDisplayStr(const Spreadsheet& sheet) const override;

	double getValue(const Spreadsheet& sheet) const override;

private:
	Date date;
};
