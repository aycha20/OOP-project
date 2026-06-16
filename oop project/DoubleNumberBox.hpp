#pragma once

#include "Box.hpp"

class DoubleNumberBox : public Box
{
public:
	DoubleNumberBox(const std::string& dNum);
	DoubleNumberBox(const double dNum);

	double getDoubleNum() const;

	std::string getDataStr() const override;
	void print() const override;
	DoubleNumberBox* clone() const override;
	size_t getLength() const override;
	std::string getDisplayStr(const Spreadsheet& sheet) const override;

	double getValue(const Spreadsheet& sheet) const override;

private:
	double dNum;
};