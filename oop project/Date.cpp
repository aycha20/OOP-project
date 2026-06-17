#include "Date.hpp"
#include <iostream>

Date::Date(unsigned day, unsigned month, unsigned year)
{
	this->year = year;
    maxDays[1] = isLeapYear(year) ? 29 : 28;
    this->month = month;
    this->day = day;

    if (!isValidDate())
    {
		// аз лично бих хвърлила изключение тук;
		// като потребител, подавайки грешни входни данни, ти няма да ми сигнализираш, че
		// поисканата от мен операция не се е получила; аз обаче заблудено ще продължа да си рънвам
		// програмата, мислейки си, че обекта ми е създаден с подадените данни;
		// ако не искаш да хвърлиш изключение и да прекратиш създаването на обекта
		// поне изпиши съобщение на конзолата (някакъв warning или все тая какво), за да съобщиш
        this->day = 1;
        this->month = 1;
        this->year = 1;
        maxDays[1] = 28;
		// тези хардкоднати стойности ги измести в константи;
		// за проекти нямаш оправдание да ги забиваш така грубо в кода навсякъде
    }

}

bool Date::setYear(unsigned year)
{
    unsigned oldYear = this->year;
    unsigned oldFebruaryDays = maxDays[1];

    this->year = year;
    maxDays[1] = isLeapYear(year) ? 29 : 28;
	
	// на тоя етап е по-смислено isValidDate да взима примерна дата (unsigned year, month, day) 
	// и да казва дали тия числа формират правилна дата и съответно цялата ф-я да е статична;
	// а не да я обвързваш с конкретна инстанция и потенциално да reassign-ваш член-данни по няколко пъти
    if (!isValidDate())
    {
        this->year = oldYear;
        maxDays[1] = oldFebruaryDays;
        return false;
    }

    return true;
}

bool Date::setMonth(unsigned month)
{
    unsigned oldMonth = this->month;
    this->month = month;
    if (!isValidDate())
    {
        this->month = oldMonth;
        return false;
    }

    return true;
}

bool Date::setDay(unsigned day)
{
    unsigned oldDay = this->day;
    this->day = day;
    if (!isValidDate())
    {
        this->day = oldDay;
        return false;
    }

    return true;
}

unsigned Date::getYear() const
{
    return year;
}

unsigned Date::getMonth() const
{
    return month;
}

unsigned Date::getDay() const
{
    return day;
}

void Date::serialize() const
{
    printTwoDigits(day);
    std::cout << '-';
    printTwoDigits(month);
    std::cout << '-';
    std::cout << year;
	
	// никаква идея дали писането е минало успешно - ако нещо се е прегазило, никой не разбира :)
}

// това не е за date класът, ами за някакъв отделен хелпър
void Date::printTwoDigits(unsigned number) const
{
    if (number < 10)
    {
        std::cout << '0';
    }

    std::cout << number;
}

// сейм
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

unsigned convertToDays(const Date& date)
{
    unsigned totalDays = 0;

    for (unsigned year = 1; year < date.getYear(); year++)
    {
        totalDays += isLeapYear(year) ? 366 : 365;
    }

    for (unsigned month = 1; month < date.getMonth(); month++)
    {
        totalDays += date.getMaxDays(month);
    }

    totalDays += date.getDay() - 1;

    return totalDays;
}

bool Date::isValidDate() const
{
    return !((month == 0 || month > 12)
        || (day == 0 || day > maxDays[month - 1])
        || (year == 0)
        || (year == 1916 && month == 4 && day < 14));
}

unsigned Date::getMaxDays(unsigned month) const
{
    if (month < 1 || month > 12)
    {
        return 0;
    }

    return maxDays[month - 1];
}

bool isLeapYear(unsigned year)
{
    if (year <= 1916)
    {
        return year % 4 == 0;
    }
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

// отново в някакъв хелпър може би (date utilities примерно, include-ва Date и работи с Date обекти, но е отделно)
unsigned getNumFromStr(const std::string& str, unsigned start, unsigned count)
{
    unsigned result = 0;

    for (unsigned i = start; i < start + count; i++)
    {
        result = result * 10 + (str[i] - '0');
    }

    return result;
}

bool isValidDate(const std::string& date)
{
    if (date.length() != 10)
    {
        return false;
    }

    // Само DD-MM-YYYY
    if (date[2] != '-' || date[5] != '-')
    {
        return false;
    }

    // Всички останали символи трябва да са цифри
    for (unsigned i = 0; i < date.length(); i++)
    {
        if (i == 2 || i == 5)
        {
            continue;
        }

        if (!isDigit(date[i]))
        {
            return false;
        }
    }

    unsigned day = getNumFromStr(date, 0, 2);
    unsigned month = getNumFromStr(date, 3, 2);
    unsigned year = getNumFromStr(date, 6, 4);

    Date temp(day, month, year);

    return temp.isValidDate();
}

int Date::getDaysBetween(const Date& other)
{
    int days1 = convertToDays(*this);
    int days2 = convertToDays(other);

    return days2 - days1;
}
