#include "DoubleNumberBox.hpp"
#include <stdexcept>
#include <iostream>

DoubleNumberBox::DoubleNumberBox(const std::string& dNum)
{
	if (dNum.empty())
	{
		throw std::invalid_argument("Empty string passed for number\n");
	}

	size_t pos = 0;
	this->dNum = std::stod(dNum, &pos);

	if (pos != dNum.length())
	{
		throw std::invalid_argument("Invalid double number\n");
	}
}

DoubleNumberBox::DoubleNumberBox(const double dNum)
{
	this->dNum = dNum;
}

double DoubleNumberBox::getDoubleNum() const
{
	return dNum;
}

std::string DoubleNumberBox::getDataStr() const
{
	return std::to_string(dNum);
}

void DoubleNumberBox::print() const
{
	std::cout << dNum;
}

DoubleNumberBox* DoubleNumberBox::clone() const
{
	return new DoubleNumberBox(*this);
}

size_t DoubleNumberBox::getLength() const
{
	return getDataStr().length();
}

std::string DoubleNumberBox::getDisplayStr(const Spreadsheet& sheet) const
{
	return getDataStr();
}

double DoubleNumberBox::getValue(const Spreadsheet& sheet) const
{
	return dNum;
}
