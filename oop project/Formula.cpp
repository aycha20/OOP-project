#include "Formula.hpp"

#include <stdexcept>
#include <cstring>
#include <iostream>

Formula::Formula(const std::string& formula)
{
    setFormula(formula);
}

void Formula::setFormula(const std::string& newFormula)
{
    if (!isValidFormula(newFormula))
    {
        throw std::invalid_argument("Invalid function passed\n");
    }

    formula = newFormula; // пак правиш assignment, ако можеш ползвай сетъри
}

std::string Formula::getFormula() const
{
    return formula;
}

void Formula::printF() const
{
    std::cout << formula;
}

double Formula::evaluate(const Spreadsheet& table) const
{
    if (!isValidFormula(formula))
    {
        throw std::invalid_argument("Invalid formula\n");
    }

    unsigned idx = 1;

    std::string leftOperand = readOperand(idx);
    std::string operation = readOperator(idx);
    std::string rightOperand = readOperand(idx);

    skipSpaces(formula, idx);

    if (idx != formula.length())
    {
        throw std::invalid_argument("Unexpected symbols after formula\n");
    }

    double lhs = evaluateOperand(leftOperand, table);
    double rhs = evaluateOperand(rightOperand, table);

    if (operation == "+")
    {
        return lhs + rhs;
    }

    if (operation == "-")
    {
        return lhs - rhs;
    }

    if (operation == "*")
    {
        return lhs * rhs;
    }

    if (operation == "/")
    {
        if (rhs == 0)
        {
            throw std::invalid_argument("Division by zero.");
        }

        return lhs / rhs;
    }

    if (operation == "==")
    {
        return lhs == rhs ? 1 : 0;
    }

    if (operation == "!=")
    {
        return lhs != rhs ? 1 : 0;
    }

    if (operation == "<")
    {
        return lhs < rhs ? 1 : 0;
    }

    if (operation == ">")
    {
        return lhs > rhs ? 1 : 0;
    }

    if (operation == "<=")
    {
        return lhs <= rhs ? 1 : 0;
    }

    if (operation == ">=")
    {
        return lhs >= rhs ? 1 : 0;
    }

    throw std::invalid_argument("Unknown operator.");
}

// извън класа
std::string Formula::readOperand(unsigned& index) const
{
    skipSpaces(formula, index);

    unsigned start = index;

    if (!parseOperand(formula, index))
    {
        throw std::invalid_argument("Invalid operand\n");
    }

    return formula.substr(start, index - start);
}

// извън класа
std::string Formula::readOperator(unsigned& index) const
{
    skipSpaces(formula, index);

    unsigned start = index;

    if (!parseOperator(formula, index))
    {
        throw std::invalid_argument("Invalid operator\n");
    }

    return formula.substr(start, index - start);
}
// извън класа
double Formula::evaluateOperand(const std::string& operand, const Spreadsheet& table) const
{
    if (isCellReferenceString(operand))
    {
        unsigned row = 0;
        unsigned col = 0;

        parseCellReferenceValue(operand, row, col);

        return table.getValueAtZeroBased(row - 1, col - 1);
    }

    return std::stod(operand);
}
// извън класа
bool Formula::isCellReferenceString(const std::string& operand) const
{
    unsigned idx = 0;

    if (!parseCellReference(operand, idx))
    {

        return false;
    }

    return idx == operand.length();
}
// извън класа
void Formula::parseCellReferenceValue(const std::string& operand, unsigned& row, unsigned& col) const
{
    unsigned idx = 0;
   
    if (!parseCellReference(operand, idx) || idx != operand.length())
    {
        throw std::invalid_argument("Invalid cell referencen\n");
    }

    idx = 1; //skipping R

    if (!parsePositiveInteger(operand, idx, row))
    {
        throw std::invalid_argument("Invalid row in cell reference\n");
    }

    if (idx >= operand.length() || operand[idx] != 'C')
    {
        throw std::invalid_argument("Invalid cell reference\n");
    }

    idx++;//skiping C

    if (!parsePositiveInteger(operand, idx, col))
    {
        throw std::invalid_argument("Invlaid column in cell reference\n");
    }
}
// извън класа
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}
// извън класа
void skipSpaces(const std::string& str, unsigned& index)
{
    while (index < str.length() && str[index] == ' ')
    {
        index++;
    }
}
// извън класа
bool parsePositiveInteger(const std::string& str, unsigned& index, unsigned& number)
{
    unsigned start = index;
    number = 0;

    if (index >= str.length() || !isDigit(str[index]))
    {
        return false;
    }

    while (index < str.length() && isDigit(str[index]))
    {
        number = number * 10 + (str[index] - '0');
        index++;
    }

    return index > start;
}
// извън класа
bool parseNumber(const std::string& str, unsigned& index)
{
    unsigned start = index;

    // Позволява отрицателни числа като -5 или -3.14
    if (index < str.length() && (str[index] == '+' || str[index] == '-'))
    {
        index++;
    }

    bool hasDigitsBeforeDot = false;
    bool hasDigitsAfterDot = false;

    while (index < str.length() && isDigit(str[index]))
    {
        hasDigitsBeforeDot = true;
        index++;
    }

    if (index < str.length() && str[index] == '.')
    {
        index++;

        while (index < str.length() && isDigit(str[index]))
        {
            hasDigitsAfterDot = true;
            index++;
        }

        if (!hasDigitsBeforeDot || !hasDigitsAfterDot)
        {
            index = start;
            return false;
        }
    }

    if (!hasDigitsBeforeDot)//tva moje da se vdigne gore
    {
        index = start;
        return false;
    }

    return true;
}
// извън класа
bool parseCellReference(const std::string& str, unsigned& index)
{
    unsigned start = index;

    if (index >= str.length() || str[index] != 'R')
    {
        return false;
    }

    index++;

    unsigned row = 0;

    if (!parsePositiveInteger(str, index, row))
    {
        index = start;
        return false;
    }

    if (row == 0)
    {
        index = start;
        return false;
    }

    if (index >= str.length() || str[index] != 'C')
    {
        index = start;
        return false;
    }

    index++;

    unsigned col = 0;

    if (!parsePositiveInteger(str, index, col))
    {
        index = start;
        return false;
    }

    if (col == 0)
    {
        index = start;
        return false;
    }

    return true;
}
// извън класа
bool parseOperand(const std::string& str, unsigned& index)
{
    skipSpaces(str, index);

    return (parseNumber(str, index)) || (parseCellReference(str, index));
}
// извън класа
bool parseOperator(const std::string& str, unsigned& index)
{
    skipSpaces(str, index);

    if (index + 1 < str.length())
    {
        std::string twoSymbols = str.substr(index, 2);

        if (twoSymbols == "==" ||
            twoSymbols == "!=" ||
            twoSymbols == "<=" ||
            twoSymbols == ">=")
        {
            index += 2;
            return true;
        }
    }

    if (index < str.length())
    {
        char ch = str[index];

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
            ch == '<' || ch == '>')
        {
            index++;
            return true;
        }
    }

    return false;
}

bool isValidFormula(const std::string& formula)
{
    if (formula.length() < 4)
    {
        return false;
    }

    if (formula[0] != '=')
    {
        return false;
    }

    unsigned index = 1;

    if (!parseOperand(formula, index))
    {
        return false;
    }

    if (!parseOperator(formula, index))
    {
        return false;
    }

    if (!parseOperand(formula, index))
    {
        return false;
    }

    
    skipSpaces(formula, index);

    return index == formula.length();
}


