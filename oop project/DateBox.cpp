#include "DateBox.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

//the format is: DD-MM-YYYY

DateBox::DateBox(const Date& date)
{
	// ако тази ф-я беше статична, щеше да можеш да я викаш през името на класа
	// без да се обвързваш с това да трябва да ти се подава цял date обект тук
	if (!date.isValidDate())
	{
		// няма нужда от новия ред в ексешъна, това не е стандартно писане на конзолата, а е
		// на операционната система
		throw std::invalid_argument("Invalid date passed\n");
	}

	this->date = date; // това може да хвърли грешка
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
	
	// ненужно създаваш нов обект и после викаш assigment оператор;
	// използвай си сетърите в/у date обекта към DateBox класа (this->data.setNeshto(...))
	this->date = Date(day, month, year);
}

// махни хардкоднатите цифри
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
	
	// имай предвид, че по всяко време += оператора на стринг класа може да фейлне,
	// така че като ги викаш тези ф-ии от други места, го съобрази
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
	
	// отново - фокусирай се в/у енкапсулацията; предостави подходящи сетъри, а недей да правиш ненужно нови обекти
	this->date = Date(day, month, year);
}

void DateBox::print() const
{
	date.serialize(); // това ако фейлне пак никой не разбира
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

