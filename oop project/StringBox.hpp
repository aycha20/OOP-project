#pragma once

#include "Box.hpp"
#include <string>

class StringBox : public Box
{
public:
	StringBox(const std::string& text);

	std::string getDataStr() const override;

	void print() const override;

	StringBox* clone() const override;

	size_t getLength() const override;
	double getValue(const Spreadsheet& sheet) const override;
	std::string getDisplayStr(const Spreadsheet& sheet) const override;


private:
	std::string text;

	bool isValidText(const std::string& text);
	void removeFillerSymb(const std::string& text);
};