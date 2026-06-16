#pragma once
#include "Box.hpp"
#include "Formula.hpp"
#include <iostream>

class FormulaBox : public Box
{
public:
	FormulaBox(const Formula& formula);
	FormulaBox(const std::string& formula);

	std::string getDataStr() const override;

	const Formula& getFormula() const;

	void print() const override;

	FormulaBox* clone() const override;
	size_t getLength() const override;

	double getValue(const Spreadsheet& sheet) const override;
	std::string getDisplayStr(const Spreadsheet& sheet) const override;

private:
	Formula formula;
};

