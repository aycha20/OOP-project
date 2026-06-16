#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "Box.hpp"

class Spreadsheet
{
public:
	
	Spreadsheet(); 
	Spreadsheet(unsigned rows, unsigned cols);

	Spreadsheet(const Spreadsheet& other);
	Spreadsheet& operator=(const Spreadsheet& other);
	~Spreadsheet();

	void open(const std::string& fileName);
	void save();
	void saveAs(const std::string& newFileName);
	void closeF();

	void print() const;
	void edit(unsigned row, unsigned col, const std::string& newContent);

	unsigned getRows() const;
	unsigned getCols() const;
	const std::string& getCurrentFile() const;
	bool getIsOpen() const;
	bool getIsModified() const;

	double getValueAtZeroBased(unsigned row, unsigned col) const;

private:
	std::vector<std::vector<Box*>> boxes;
	unsigned rows = 0;
	unsigned cols = 0;

	std::vector<unsigned> rowSizes;

	std::string currentFile;

	bool isOpen = false;
	bool isModified = false;

	void free();
	void copyFrom(const Spreadsheet& other);

	std::vector<size_t> getColWidth() const;
	
	bool isSeparator(char ch);
	unsigned countCellsInLine(const std::string& line, unsigned row);

	void setCurrentFile(const std::string& newFile);

	void getTableSize(std::ifstream& file, unsigned& fRows, unsigned& fCols);
	std::string readCellFromLine(const std::string& line, unsigned row ,unsigned& idx);
	Box* createBoxFromText(const std::string& text, unsigned row, unsigned col);
	void saveToFile(const std::string& fileName) const;
	bool isEscapedQuote(const std::string& line, unsigned idx) const;
	std::string trim(const std::string& str) const;
	bool isStringContent(const std::string& content) const;
	std::string makeInvalidCellMessage(unsigned row, unsigned col, const std::string& text) const;
	std::string makeMissingSeparatorMessage(unsigned row, unsigned pos) const;
	std::string makeMissingQuoteMessage(unsigned row, unsigned pos) const;
};
 
