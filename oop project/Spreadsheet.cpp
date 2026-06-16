#include "Spreadsheet.hpp"

#include "WholeNumberBox.hpp"
#include "DoubleNumberBox.hpp"
#include "StringBox.hpp"
#include "DateBox.hpp"
#include "FormulaBox.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

Spreadsheet::Spreadsheet()
    :boxes(), rows(0), cols(0), rowSizes(),
    currentFile(""), isOpen(false), isModified(false)
{}

Spreadsheet::Spreadsheet(unsigned rows, unsigned cols)
    :boxes(), rows(rows), cols(cols), rowSizes(),
    currentFile(""), isOpen(false), isModified(false)
{   
    try
    {
        rowSizes.resize(rows, cols);
        boxes.resize(rows);

        for (unsigned i = 0; i < rows; i++)
        {
            boxes[i].resize(cols, nullptr);

            for (unsigned j = 0; j < cols; j++)
            {
                boxes[i][j] = new StringBox("");
            }
        }
    }
    catch (...)
    {
        free();
        throw;
    }
}

Spreadsheet::Spreadsheet(const Spreadsheet& other)
    :boxes(), rows(0), cols(0)
{
    copyFrom(other);
}

Spreadsheet& Spreadsheet::operator=(const Spreadsheet& other)
{
    if (this != &other)
    {
        Spreadsheet temp(other);
        free();
        copyFrom(temp);
    }

    return *this;
}

Spreadsheet::~Spreadsheet()
{
    free();
}

void Spreadsheet::open(const std::string& fileName)
{
    if (isOpen && isModified)
    {
        throw std::runtime_error("There are unsaved changes. Save or close the current file before opening another one.");
    }

    if (fileName.empty())
    {
        throw std::invalid_argument("Empty string passed for file name\n");
    }

    std::ifstream file(fileName);

    if (!file.is_open())
    {
        throw std::runtime_error("File cannot be opened\n");
    }

    unsigned fRows = 0;
    unsigned fCols = 0;

    getTableSize(file, fRows, fCols);


    Spreadsheet temp(fRows, fCols);

    std::string line;
    unsigned row = 0;

    while (std::getline(file, line) && row < fRows)
    {
        unsigned idx = 0;
        unsigned col = 0;

        unsigned cellsInCurrLine = countCellsInLine(line, row + 1);

        if (row < temp.rowSizes.size())
        {
            temp.rowSizes[row] = cellsInCurrLine;
        }
       
        while (col < cellsInCurrLine && col < temp.cols)
        {
            std::string cellText = readCellFromLine(line, row + 1, idx);

            Box* newBox = temp.createBoxFromText(cellText, row + 1, col + 1);

            delete temp.boxes[row][col];
            temp.boxes[row][col] = newBox;

            col++;
        }
        row++;
    }
    *this = temp;

    currentFile = fileName;
    isOpen = true;
    isModified = false;
}

void Spreadsheet::save()
{
    if (!isOpen)
    {
        throw std::runtime_error("No file is currently open\n");
    }

    if (currentFile.empty())
    {
        throw std::runtime_error("No file is currently selected\n");
    }

    saveToFile(currentFile);

    isModified = false;
}

void Spreadsheet::saveAs(const std::string& newFileName)
{
    if (!isOpen)
    {
        throw std::runtime_error("No file is currently open\n");
    }
    if (newFileName.empty())
    {
        throw std::invalid_argument("Empty file name pssed\n");
    }

    saveToFile(newFileName);

    currentFile = newFileName;
    isModified = false;
}

void Spreadsheet::closeF()
{
    if (!isOpen)
    {
        throw std::runtime_error("No file is currently open\n");
    }
    if (isModified)
    {
        std::cout << "Cannot close file because there are unsaved changes. Save the file before closing.\n";       
        return;
    }

    free();

    currentFile.clear();
    isOpen = false;
    isModified = false;
}

void Spreadsheet::print() const
{
    if (rows == 0 || cols == 0)
    {
        return;
    }

    std::vector<size_t> widths = getColWidth();

    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {
            std::string value = "";

            if (i < boxes.size() &&
                i < rowSizes.size() &&
                j < rowSizes[i] &&
                j < boxes[i].size() &&
                boxes[i][j] != nullptr)
            {
                value = boxes[i][j]->getDisplayStr(*this);
            }

            std::cout << value;

            for (size_t k = value.length(); k < widths[j]; k++)
            {
                std::cout << " ";
            }

            std::cout << "|";
        }

        std::cout << std::endl;
    }
}

void Spreadsheet::edit(unsigned row, unsigned col, const std::string& newContent)
{

    if (!isOpen)
    {
        throw std::runtime_error("No file is currently open\n");
    }

    if (row == 0 || col == 0)
    {
        throw std::out_of_range("Rows and columns start from 1\n");
    }

    unsigned realRow = row - 1;
    unsigned realCol = col - 1;

    if (realRow >= rows || realCol >= cols)
    {
        throw std::out_of_range(
            "Invalid cell position: row " + std::to_string(row) +
            ", column " + std::to_string(col));
    }

    Box* newBox = createBoxFromText(newContent, row, col);

    delete boxes[realRow][realCol];
    boxes[realRow][realCol] = newBox;

    if (col > rowSizes[realRow])
    {
        rowSizes[realRow] = col;
    }

    isModified = true;
   
}

bool Spreadsheet::getIsOpen() const
{
    return isOpen;
}

bool Spreadsheet::getIsModified() const
{
    return isModified;
}

double Spreadsheet::getValueAtZeroBased(unsigned row, unsigned col) const
{
    if (row >= rows || col >= cols)
    {
        return 0;
    }

    if (row >= rowSizes.size() || col >= rowSizes[row])
    {
        return 0;
    }

    if (row >= boxes.size() || col >= boxes[row].size())
    {
        return 0;
    }

    if (boxes[row][col] == nullptr)
    {
        return 0;
    }

    return boxes[row][col]->getValue(*this);
}

void Spreadsheet::free()
{
    for (unsigned i = 0; i < boxes.size(); i++)
    {
        for (unsigned j = 0; j < boxes[i].size(); j++)
        {
            delete boxes[i][j];
            boxes[i][j] = nullptr;
        }
        boxes[i].clear();
    }

    boxes.clear();
    rowSizes.clear();
    rows = 0;
    cols = 0;
}

void Spreadsheet::copyFrom(const Spreadsheet& other)
{
    rows = other.rows;
    cols = other.cols;
    currentFile = other.currentFile;
    isOpen = other.isOpen;
    isModified = other.isModified;

    boxes.clear();
    rowSizes.clear();

    if (rows == 0 || cols == 0)
    {
        return;
    }

    rowSizes.resize(rows);

    for (unsigned i = 0; i < rows; i++)
    {
        rowSizes[i] = other.rowSizes[i];
    }

    boxes.resize(rows);

    try
    {
        for (unsigned i = 0; i < rows; i++)
        {
            boxes[i].resize(cols, nullptr);

            for (unsigned j = 0; j < cols; j++)
            {
                if (i < other.boxes.size() &&
                    i < other.rowSizes.size() &&
                    j < other.rowSizes[i] &&
                    j < other.boxes[i].size() &&
                    other.boxes[i][j] != nullptr)
                {
                    boxes[i][j] = other.boxes[i][j]->clone();
                }
                else
                {
                    boxes[i][j] = new StringBox("");
                }
            }
        }
    }
    catch (...)
    {
        free();
        throw;
    }
   
}

std::vector<size_t> Spreadsheet::getColWidth() const
{
    std::vector<size_t> widths(cols, 0);

    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {
            std::string value = "";

            if (i < boxes.size() && 
                i < rowSizes.size() &&
                j < rowSizes[i] && 
                j < boxes[i].size() && 
                boxes[i][j] != nullptr)
            {
                value = boxes[i][j]->getDisplayStr(*this);
            }

            if (value.length() > widths[j])
            {
                widths[j] = value.length();
            }
        }
    }

    return widths;
}

bool Spreadsheet::isSeparator(char ch)
{
    return ch == ',' || ch == ';' || ch == '\t';
}

unsigned Spreadsheet::countCellsInLine(const std::string& line, unsigned row)
{
    if (line.empty())
    {
        return 0;
    }

    unsigned count = 1;
    bool inString = false;
    unsigned openingQuotePosition = 0;

    for (unsigned i = 0; i < line.size(); i++)
    {
        if (line[i] == '"' && !isEscapedQuote(line, i))
        {
            inString = !inString;

            if (inString)
            {
                openingQuotePosition = i + 1; // 1-based position
            }
        }
        else if (!inString && isSeparator(line[i]))
        {
            count++;
        }
    }

    if (inString)
    {
        throw std::runtime_error(makeMissingQuoteMessage(row, openingQuotePosition));
    }

    return count;
}

void Spreadsheet::setCurrentFile(const std::string& newFile)
{
    if (newFile.empty())
    {
        throw std::invalid_argument("Empty file name passed\n");
    }

    currentFile = newFile;
}

void Spreadsheet::getTableSize(std::ifstream& file, unsigned& fRows, unsigned& fCols)
{
    if(!file.is_open())
    {
        throw std::runtime_error("Could not open file.");
    }

    fRows = 0;
    fCols = 0;

    std::string line;

    while (std::getline(file, line))
    {
        fRows++;

        unsigned currentCols = countCellsInLine(line, fRows);

        if (currentCols > fCols)
        {
            fCols = currentCols;
        }
    }

    file.clear();
    file.seekg(0, std::ios::beg);
}

std::string Spreadsheet::readCellFromLine(const std::string& line,unsigned row, unsigned& idx)
{
    std::string cell;

    while (idx < line.size() && line[idx] == ' ')
    {
        cell += line[idx];
        idx++;
    }

    if (idx >= line.size())
    {
        return cell;
    }

    bool startsWithQuote = line[idx] == '"';

    if (!startsWithQuote)
    {
        while (idx < line.size() && !isSeparator(line[idx]))
        {
            cell += line[idx];
            idx++;
        }

        if (idx < line.size() && isSeparator(line[idx]))
        {
            idx++;
        }

        return cell;
    }

    unsigned openingQuotePosition = idx + 1;

    cell += line[idx];
    idx++;

    bool closed = false;
    unsigned closingQuotePosition = 0;


    while (idx < line.size())
    {
        char ch = line[idx];
        cell += ch;

        if (ch == '"' && !isEscapedQuote(line, idx))
        {
            closed = true;
            closingQuotePosition = idx + 1;
            idx++;
            break;
        }

        idx++;
    }

    if (!closed)
    {
        throw std::runtime_error(makeMissingQuoteMessage(row, openingQuotePosition));
    }

    while (idx < line.size() && line[idx] == ' ')
    {
        cell += line[idx];
        idx++;
    }

    if (idx < line.size())
    {
        if (isSeparator(line[idx]))
        {
            idx++;
            return cell;
        }

        throw std::runtime_error(makeMissingSeparatorMessage(row, closingQuotePosition));
    }

    return cell;
}

Box* Spreadsheet::createBoxFromText(const std::string& text, unsigned row, unsigned col)
{
    std::string value = trim(text);

    if (value.empty())
    {
        return new StringBox("");
    }

    if (value[0] == '=')
    {
        try
        {
            return new FormulaBox(value);
        }
        catch (const std::exception&)
        {
            throw std::runtime_error(makeInvalidCellMessage(row, col, value));
        }
    }

    if (isStringContent(value))
    {
        return new StringBox(value.substr(1, value.size() - 2));
    }

    try
    {
        return new DateBox(value);
    }
    catch (const std::invalid_argument&)
    {
    }

    bool hasDot = false;

    for (unsigned i = 0; i < value.size(); i++)
    {
        if (value[i] == '.')
        {
            hasDot = true;
            break;
        }
    }

    if (hasDot)
    {
        try
        {
            return new DoubleNumberBox(value);
        }
        catch (const std::invalid_argument&)
        {
        }
    }
    else
    {
        try
        {
            return new WholeNumberBox(value);
        }
        catch (const std::invalid_argument&)
        {
        }
    }

    throw std::runtime_error(makeInvalidCellMessage(row, col, value));
}

void Spreadsheet::saveToFile(const std::string& fileName) const
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        throw std::runtime_error("File cannot be opened for writing\n");
    }

    for (unsigned i = 0; i < rows; i++)
    {
        unsigned currentRowSize = 0;

        if (i < rowSizes.size())
        {
            currentRowSize = rowSizes[i];
        }

        for (unsigned j = 0; j < currentRowSize; j++)
        {
            if (i < boxes.size() &&
                j < boxes[i].size() &&
                boxes[i][j] != nullptr)
            {
                file << boxes[i][j]->getDataStr();
            }

            if (j + 1 < currentRowSize)
            {
                file << ",";
            }
        }

        if (i + 1 < rows)
        {
            file << "\n";
        }
    }
}

bool Spreadsheet::isEscapedQuote(const std::string& line, unsigned idx) const
{
    unsigned backslashCount = 0;
    unsigned i = idx;

    while (i > 0 && line[i - 1] == '\\')
    {
        backslashCount++;
        i--;
    }

    return backslashCount % 2 == 1;
}

std::string Spreadsheet::trim(const std::string& str) const
{
    unsigned start = 0;

    while (start < str.size() &&
        (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
    {
        start++;
    }

    if (start == str.size())
    {
        return "";
    }

    unsigned end = str.size();

    while (end > start &&
        (str[end - 1] == ' ' || str[end - 1] == '\t' || str[end - 1] == '\r'))
    {
        end--;
    }

    return str.substr(start, end - start);
}

bool Spreadsheet::isStringContent(const std::string& content) const
{
    std::string trimmed = trim(content);

    if (trimmed.size() < 2)
    {
        return false;
    }

    if (trimmed[0] != '"' || trimmed[trimmed.size() - 1] != '"')
    {
        return false;
    }

    if (isEscapedQuote(trimmed, trimmed.size() - 1))
    {
        return false;
    }

    for (unsigned i = 1; i < trimmed.size() - 1; i++)
    {
        if (trimmed[i] == '"' && !isEscapedQuote(trimmed, i))
        {
            return false;
        }
    }

    return true;
}

std::string Spreadsheet::makeInvalidCellMessage(unsigned row, unsigned col, const std::string& text) const
{
    return "Invalid cell content at row " + std::to_string(row) +
        ", column " + std::to_string(col) +
        ": \"" + text + "\"";
}

std::string Spreadsheet::makeMissingSeparatorMessage(unsigned row, unsigned pos) const
{
    return "Missing separator at row " + std::to_string(row) +
        " after symbol " + std::to_string(pos);
}

std::string Spreadsheet::makeMissingQuoteMessage(unsigned row, unsigned pos) const
{
    return "Missing closing quotation mark at row " + std::to_string(row) +
        ". Opening quote is at symbol " + std::to_string(pos);
}