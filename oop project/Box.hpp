#pragma once

#include <string>

class Spreadsheet; // forward declaration to break circular dependancy

class Box
{
public:
	virtual void print() const = 0;

	virtual std::string getDataStr() const = 0;

	virtual std::string getDisplayStr(const Spreadsheet& sheet) const = 0;

	virtual Box* clone() const = 0;

	virtual size_t getLength() const = 0;

	virtual double getValue(const Spreadsheet& sheet) const = 0;

	virtual ~Box() = default;

};