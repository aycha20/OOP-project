#pragma once

#include <string>

class Spreadsheet; // forward declaration to break circular dependancy

// Пиши коментари кой клас какво предназначение има
// и всяка ф-я какво прави (основна функционалност, аргументи, return value);
// в момента примерно нямам представа кое с какъв intention е правено
class Box
{
public:
	// Тук може би е добре да се подава някакъв default параметър
	// (може да не е конзола)
    virtual void print(/*std::ostream& out = std::cout*/) const = 0;

	// Тук всеки път, като връщаш резултат, правиш ново копие, което потенциално може да е скъпо
	virtual std::string getDataStr() const = 0;

	virtual std::string getDisplayStr(const Spreadsheet& sheet) const = 0;

	virtual Box* clone() const = 0;

	virtual size_t getLength() const = 0;

	virtual double getValue(const Spreadsheet& sheet) const = 0;

	virtual ~Box() = default;

};