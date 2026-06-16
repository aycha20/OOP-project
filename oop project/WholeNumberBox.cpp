#include "WholeNumberBox.hpp"
#include <iostream>
#include <stdexcept>

WholeNumberBox::WholeNumberBox(const int wholeNum)
{
    this->wholeNum = wholeNum;
}

WholeNumberBox::WholeNumberBox(const std::string& wholeNum)
{
    if (wholeNum.empty())
    {
        throw std::invalid_argument("Empty string passed for number\n");
    }

    size_t pos = 0;
    this->wholeNum = std::stoi(wholeNum, &pos);

    if (pos != wholeNum.length())
    {
        throw std::invalid_argument("Invalid whole number\n");
    }
}

int WholeNumberBox::getWholeNum() const
{
    return wholeNum;
}

std::string WholeNumberBox::getDataStr() const
{
    return std::to_string(wholeNum);
}

void WholeNumberBox::print() const
{
    std::cout << wholeNum;
}

WholeNumberBox* WholeNumberBox::clone() const
{
    return new WholeNumberBox(*this);
}

size_t WholeNumberBox::getLength() const
{
    return getDataStr().length();
}

std::string WholeNumberBox::getDisplayStr(const Spreadsheet& sheet) const
{
    return getDataStr();
}

double WholeNumberBox::getValue(const Spreadsheet& sheet) const
{
    return (double)wholeNum;
}
