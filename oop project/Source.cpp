#include "Box.hpp"
#include "WholeNumberBox.hpp"
#include "DoubleNumberBox.hpp"
#include "StringBox.hpp"
#include "DateBox.hpp"
#include "FormulaBox.hpp"
#include "Spreadsheet.hpp"

#include <iostream>

enum class Command
{
	Open,
	Save,
	SaveAs,
	Close,
	Print,
	Edit,
	Invalid
};

void toLowerCase(std::string& input)
{
	for (unsigned i = 0; i < input.size(); i++)
	{
		if (input[i] >= 'A' && input[i] <= 'Z')
		{
			input[i] += ('a' - 'A');
		}
	}
}

Command getCommand(std::string& input)
{
	toLowerCase(input);
	if (input.compare("open") == 0)
	{
		return Command::Open;
	}
	if (input.compare("save") == 0)
	{
		return Command::Save;
	}
	if (input.compare("saveas") == 0)
	{
		return Command::SaveAs;
	}
	if (input.compare("close") == 0)
	{
		return Command::Close;
	}
	if (input.compare("print") == 0)
	{
		return Command::Print;
	}
	if (input.compare("edit") == 0)
	{
		return Command::Edit;
	}
	else
	{
		return Command::Invalid;
	}
}

int main()
{
	Spreadsheet sheet;
	
	bool canEnd = false;
	Command lastCommand = Command::Invalid;


	std::string input;
	Command command;
    std::string fileName;

    do
    {
        try
        {
            std::string input;

            std::cout << "Enter command\n";
            std::cin >> input;

            command = getCommand(input);

            if (command == Command::Invalid)
            {
                std::cout << "Invalid command.\n";
                continue;
            }

            if (command == Command::Open)
            {
                if (sheet.getIsOpen())
                {
                    std::cout << "A file is already open. Close it first.\n";
                    continue;
                }

                std::cout << "Enter file name:\n";
                std::cin >> fileName;

                try
                {
                    sheet.open(fileName);
                    std::cout << "Successfully opened " << fileName << "\n";
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what() << "\n";
                }
            }
            else if (command == Command::Save)
            {
                if (!sheet.getIsOpen())
                {
                    std::cout << "No open file, open file first\n";
                    continue;
                }

                sheet.save();
                lastCommand = Command::Save;
                std::cout << "File: " << fileName << " saved successfully\n";

            }
            else if (command == Command::SaveAs)
            {
                if (!sheet.getIsOpen())
                {
                    std::cout << "No open file, open file first\n";
                    continue;
                }

                std::string fName;

                std::cout << "Enter new file name:\n";
                std::cin >> fName;

                sheet.saveAs(fName);
                lastCommand = Command::SaveAs;

                std::cout << "File: " << fileName << " saved as: " <<
                    fName << " successfully\n";

                fileName = fName;

            }
            else if (command == Command::Close)
            {
                if (!sheet.getIsOpen())
                {
                    std::cout << "No open file, open file first\n";
                    continue;
                }
                if (lastCommand == Command::Close)
                {
                    canEnd = true;
                    continue;
                }
                if (lastCommand == Command::Save ||
                    lastCommand == Command::SaveAs)
                {
                    canEnd = true;
                    sheet.closeF();

                    std::cout << "File closed successfully\n";
                }
                else
                {
                    std::cout << "Unsaved changes detected, do you still want to close?\n";
                }
                lastCommand = Command::Close;
            }
            else if (command == Command::Print)
            {
                if (!sheet.getIsOpen())
                {
                    std::cout << "No open file, open file first\n";
                    continue;
                }

                sheet.print();
                lastCommand = Command::Print;
            }
            else if (command == Command::Edit)
            {
                if (!sheet.getIsOpen())
                {
                    std::cout << "No open file, open file first\n";
                    continue;
                }

                unsigned r = 0;
                unsigned c = 0;
                std::string data;

                std::cout << "Enter row, col, and input data:\n";
                std::cin >> r >> c >> data;

                sheet.edit(r, c, data);
                lastCommand = Command::Edit;
                std::cout << "File edited successfully\n";
            }
        }
        catch (const std::exception& exc)
        {
            std::cout << "Error occured: " << exc.what() << std::endl;
            return -1;
        }

    } while (canEnd == false);

	return 0;
}