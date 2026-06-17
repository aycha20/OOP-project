#pragma once
#include <string>

class Date
{
public:

	Date(unsigned day, unsigned month, unsigned year);
	
	// default означава, че понеже нямаш динамично-заделени ресурси в класа,
	// си файн с това, което компилаторът автоматично ще ти генерира като голяма тройка/петица
	// (конструктор по подразбиране, асайнмънт оператор и т.н.); в случая му казваш
	// да инициализира член-данните с дефолтните стойности, които си задала
	// помисли дали в data клас това има смисъл
	Date() = default; //kakvo znachi tva

	bool setYear(unsigned year);
	bool setMonth(unsigned month);
	bool setDay(unsigned day);

	unsigned getYear() const;
	unsigned getMonth() const;
	unsigned getDay() const;
	
	// serialize стандартно взима като аргумент някакъв output stream и е хубаво
	// или да връща bool, или задължително да хвърля ексепшън при проблем
	void serialize() const;
	
	
	int getDaysBetween(const Date& other);
	
	// има ли смисъл от това? след като имаш клас Data, един обект от тип Date
	// трябва да подсигуриш, че във всяка част на жизнения си цикъл ще бъде
	// във валидно състояние (това е RAII принципът и ООП подхода);
	// ако тази ф-я искаш да си я ползваш на други места (примерно в/у стирнгови обекти
	// или числа, за да видиш дали формират дата), направи я статична или по-добре я изкарай извън класа
	// в някакъв helper namespace
	bool isValidDate() const;

	unsigned getMaxDays(unsigned month) const;

private:
	unsigned day = 1;
	unsigned month = 1;
	unsigned year = 1;

	unsigned maxDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	void printTwoDigits(unsigned number) const;
};

// ползваш стринг, това е ненужно;
// аналог на това е std::isdigit(char);
// не си пълни хедърите с ненужни неща
static bool isDigit(char c);

unsigned convertToDays(const Date& date);

bool isLeapYear(unsigned year);
unsigned getNumFromStr(const std::string& str, unsigned start, unsigned count);
bool isValidDate(const std::string& date);