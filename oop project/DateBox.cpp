#include "DateBox.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

//the format is: DD-MM-YYYY

DateBox::DateBox(const Date& date)
{
	if (!date.isValidDate())
	{
		throw std::invalid_argument("Invalid date passed\n");
	}

	this->date = date;
}

DateBox::DateBox(const std::string& date)
{
	if (!isValidDate(date))
	{
		throw std::invalid_argument("Invalid date passed\n");
	}

	unsigned day = getNumFromStr(date, 0, 2);
	unsigned month = getNumFromStr(date, 3, 2);
	unsigned year = getNumFromStr(date, 6, 4);

	this->date = Date(day, month, year);
}

std::string DateBox::getDataStr() const
{
	std::string temp;

	if (date.getDay() < 10)
	{
		temp += '0';
	}

	temp += std::to_string(date.getDay());
	temp += '-';

	if (date.getMonth() < 10)
	{
		temp += '0';
	}
	temp += std::to_string(date.getMonth());
	temp += '-';

	if (date.getYear() < 10)
	{
		temp += '0';
		temp += '0';
		temp += '0';
		temp += std::to_string(date.getYear());
	}
	else if (date.getYear() < 100)
	{
		temp += '0';
		temp += '0';
		temp += std::to_string(date.getYear());
	}
	else if (date.getYear() < 1000)
	{
		temp += '0';
		temp += std::to_string(date.getYear());
	}
	else
	{
		temp += std::to_string(date.getYear());
	}
	return temp;
}

const Date& DateBox::getDate() const
{
	return date;
}

void DateBox::setDate(const std::string& newDate)
{
	if (!isValidDate(newDate))
	{
		throw std::invalid_argument("Invalid date passed\n");
	}

	unsigned day = getNumFromStr(newDate, 0, 2);
	unsigned month = getNumFromStr(newDate, 3, 2);
	unsigned year = getNumFromStr(newDate, 6, 4);

	this->date = Date(day, month, year);
}

void DateBox::print() const
{
	date.serialize();
}

DateBox* DateBox::clone() const
{
	return new DateBox(*this);
}

size_t DateBox::getLength() const
{
	return getDataStr().length();
}

std::string DateBox::getDisplayStr(const Spreadsheet& sheet) const
{
	return getDataStr();
}

double DateBox::getValue(const Spreadsheet& sheet) const
{
	return convertToDays(date);
}

