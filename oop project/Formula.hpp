#pragma once
#include <string>
#include "Spreadsheet.hpp"

class Formula
{
public:
	Formula(const std::string& formula);
	Formula() = default;

	void setFormula(const std::string& newFormula);
	std::string getFormula() const;

	void printF() const;

	double evaluate(const Spreadsheet& table) const;

private:
	std::string formula;


	std::string readOperand(unsigned& index) const;
	std::string readOperator(unsigned& index) const;

	double evaluateOperand(const std::string& operand, const Spreadsheet& table) const;

	bool isCellReferenceString(const std::string& operand) const;
	void parseCellReferenceValue(const std::string& operand, unsigned& row, unsigned& col) const;
};
// хелпъри ОТДЕЛНО
static bool isDigit(char c);

static void skipSpaces(const std::string& str, unsigned& index);

bool parsePositiveInteger(const std::string& str, unsigned& index, unsigned& number);
bool parseNumber(const std::string& str, unsigned& index);
bool parseCellReference(const std::string& str, unsigned& index);
bool parseOperand(const std::string& str, unsigned& index);
bool parseOperator(const std::string& str, unsigned& index);
bool isValidFormula(const std::string& formula);
