#include "StringBox.hpp"
#include <stdexcept>
#include <iostream>

StringBox::StringBox(const std::string& text)
{
    if (!isValidText(text))//tuk kato sme proverili nqma nujda dolu pak?
    {
        throw std::invalid_argument("Invalid text passed\n");
    }

    removeFillerSymb(text);
}

std::string StringBox::getDataStr() const
{
    std::string result = "\"";

    for (unsigned i = 0; i < text.size(); i++)
    {
        if (text[i] == '"')
        {
            result += "\\\"";
        }
        else if (text[i] == '\\')
        {
            result += "\\\\";
        }
        else
        {
            result += text[i];
        }
    }

    result += "\"";

    return result;
}

void StringBox::print() const
{
    std::cout << text;
}

StringBox* StringBox::clone() const
{
    return new StringBox (*this);
}

size_t StringBox::getLength() const
{
    return text.length();
}

double StringBox::getValue(const Spreadsheet& sheet) const
{
    return 0.0;
}

std::string StringBox::getDisplayStr(const Spreadsheet& sheet) const
{
    return text;
}

bool StringBox::isValidText(const std::string& text)
{
    for (size_t i = 0; i < text.size(); i++)
    {        
        if (text[i] == '\\')
        {
            if (i + 1 >= text.size())
            {
                return false;
            }
            if (text[i + 1] != '"' && text[i + 1] != '\\')
            {
                return false;
            }

            i++;
        }
    }
    return true;
}

void StringBox::removeFillerSymb(const std::string& text)
{
    this->text.clear();

    for (unsigned i = 0; i < text.size(); i++)
    {
        if (text[i] == '\\' && i + 1 < text.size() &&
            (text[i + 1] == '"' || text[i + 1] == '\\'))
        {
            this->text.push_back(text[i + 1]);
            i++;
        }
        else
        {
            this->text.push_back(text[i]);
        }
    }
}
