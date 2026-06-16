#pragma once

#include "Box.hpp"
#include <string>

class WholeNumberBox : public Box
{
public:
	WholeNumberBox(const int wholeNum);
	WholeNumberBox(const std::string& wholeNum);

	int getWholeNum() const;

	std::string getDataStr() const override;
	void print() const override;
	WholeNumberBox* clone() const override;
	size_t getLength() const override;
	std::string getDisplayStr(const Spreadsheet& sheet) const override;

	double getValue(const Spreadsheet& sheet) const override;

private:
	int wholeNum;
};

