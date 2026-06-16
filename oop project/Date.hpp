#pragma once
#include <string>

class Date
{
public:

	Date(unsigned day, unsigned month, unsigned year);
	Date() = default; //kakvo znachi tva

	bool setYear(unsigned year);
	bool setMonth(unsigned month);
	bool setDay(unsigned day);

	unsigned getYear() const;
	unsigned getMonth() const;
	unsigned getDay() const;

	void serialize() const;
	
	
	int getDaysBetween(const Date& other);
	bool isValidDate() const;

	unsigned getMaxDays(unsigned month) const;

private:
	unsigned day = 1;
	unsigned month = 1;
	unsigned year = 1;

	unsigned maxDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	void printTwoDigits(unsigned number) const;
};

static bool isDigit(char c);

unsigned convertToDays(const Date& date);

bool isLeapYear(unsigned year);
unsigned getNumFromStr(const std::string& str, unsigned start, unsigned count);
bool isValidDate(const std::string& date);