#include "FormulaBox.hpp"
#include "Spreadsheet.hpp"
#include <stdexcept>

FormulaBox::FormulaBox(const Formula& formula)
    :formula(formula) {}

FormulaBox::FormulaBox(const std::string& formula)
{
    if (!isValidFormula(formula))
    {
        throw std::invalid_argument("Invalid formula passed\n");
    }

    this->formula = formula;
}

std::string FormulaBox::getDataStr() const
{
    return formula.getFormula();
}

const Formula& FormulaBox::getFormula() const
{
    return formula;
}

void FormulaBox::print() const
{
    formula.printF();
}

FormulaBox* FormulaBox::clone() const
{
    return new FormulaBox(*this);
}

size_t FormulaBox::getLength() const
{
    return getDataStr().length();
}

double FormulaBox::getValue(const Spreadsheet& sheet) const
{
    return formula.evaluate(sheet);
}

std::string FormulaBox::getDisplayStr(const Spreadsheet& sheet) const
{
    try
    {
        return std::to_string(formula.evaluate(sheet));
    }
    catch (...)
    {
        return "ERROR";
    }
}
