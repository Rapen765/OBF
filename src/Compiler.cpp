#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <Windows.h>
#include <filesystem>

using namespace std;

string move_(int *num, char c1, char c2)
{
	if (*num != 0)
	{
		if (*num > 0)
			return (string)to_string(abs(*num)) + c1 + ";";

		else if (*num < 0)
			return (string)to_string(abs(*num)) + c2 + ";";
	}
	return "";
}

bool error(ifstream *ifile, string path)
{
	HANDLE hc = GetStdHandle(STD_ERROR_HANDLE);
	string line;
	string lines;
	
	while (ifile->good())
	{
		*ifile >> line;
		lines += line;
	}
	int loop = 0;
	for (char c : lines)
	{
		if (c == '[')
			loop++;
		
		if (c == ']')
			loop--;
	}
	if (loop != 0)
	{
		SetConsoleTextAttribute(hc, FOREGROUND_RED);
		std::cout << "\nError: ";
		SetConsoleTextAttribute(hc, 7);
		std::cout << "Loop Error\n";
		std::cout << "|In " + path + "\n|\n";
		SetConsoleTextAttribute(hc, 0xf);
		std::cout << "|The number of opening square brackets doesn't match the number of closing bracket.\n\n";

		SetConsoleTextAttribute(hc, 7);
		return true;
	}
	return false;

}

void optimise(ifstream* ifile, ofstream* ofile)
{
	string line;
	string lines;
	while (ifile->good())
	{
		*ifile >> line;
		lines += line;
	}
	int add = 0;
	int array_move = 0;
	char func_name;
	bool func = false;
	bool func_def = false;
	string func_code = "";
	for (char c : lines)
	{
		if (func && c == '(')
		{
			func_def = true;
		}
		else if (func_def && c == ')')
		{
			func_code += move_(&array_move, '>', '<');
			func_code += move_(&add, '+', '-');
			*ofile << "|" + string(1, func_name) + "(" + func_code + ")";
			func_def = false;
			func_name = ' ';
			func = false;
			func_code = "";
			add = 0;
			array_move = 0;
		}
		else if(func && !func_def)
		{

			*ofile << move_(&add, '+', '-');
			*ofile << move_(&array_move, '>', '<');
			add = 0;
			array_move = 0;
			*ofile << func_name << ";";
			func_name = ' ';
			func = false;
		}
		switch (c)
		{
		case '+':
			if (!func_def)
			{
				*ofile << move_(&array_move, '>', '<');
			}
			else {
				func_code += move_(&array_move, '>', '<');
			}

			array_move = 0;
			add ++;
			break;
		case '-':
			if (!func_def)
			{
				*ofile << move_(&array_move, '>', '<');
			}
			else {
				func_code += move_(&array_move, '>', '<');
			}
			array_move = 0;

			add --;
			break;
		case '>':
			if (!func_def)
			{
				*ofile << move_(&add, '+', '-');
			}
			else {
				func_code += move_(&add, '+', '-');
			}
			add = 0;
			array_move++;
			break;
		case '<':
			if (!func_def)
			{
				*ofile << move_(&add, '+', '-');
			}
			else {
				func_code += move_(&add, '+', '-');
			}
			add = 0;
			array_move--;
			break;
		case '.':

			if (!func_def)
			{
				*ofile << move_(&add, '+', '-');
				*ofile << move_(&array_move, '>', '<');


				*ofile << ".;";
			}
			else {
				func_code += move_(&add, '+', '-');
				func_code += move_(&array_move, '>', '<');
				func_code += ".;";
			}
			array_move = 0;
			add = 0;
			break;
		case ',':

			if (!func_def)
			{
				*ofile << move_(&add, '+', '-');
				*ofile << move_(&array_move, '>', '<');


				*ofile << ",;";
			}
			else {
				func_code += move_(&add, '+', '-');
				func_code += move_(&array_move, '>', '<');
				func_code += ",;";
			}
			array_move = 0;
			add = 0;
			break;

		case '[':

			if (!func_def)
			{
				*ofile << move_(&add, '+', '-');
				*ofile << move_(&array_move, '>', '<');


				*ofile << "[;";
			}
			else {
				func_code += move_(&add, '+', '-');
				func_code += move_(&array_move, '>', '<');


				func_code +=  "[;";
			}
			array_move = 0;
			add = 0;
			break;

		case ']':

			if (!func_def)
			{
				*ofile << move_(&add, '+', '-');
				*ofile << move_(&array_move, '>', '<');


				*ofile << "];";
			}
			else {
				func_code += move_(&add, '+', '-');
				func_code += move_(&array_move, '>', '<');


				func_code += "];";
			}
			array_move = 0;
			add = 0;
			break;
		default:
			if (isalpha(c))
			{

				func_name = c;
				func = true;
			}
			break;
		}


	}
}

void compile(ifstream *ifile, ofstream *ofile)
{
	string line;
	string lines;
	while (ifile->good())
	{
		*ifile >> line;
		lines += line;
	}
	string output = "int main(){";
	*ofile << "#include<iostream>\nusing namespace std;\nint arr[100];\nint i = 50;\n";
	string number;
	bool func = false;
	for (char c : lines)
	{
		switch (c)
		{
		case '+':
			if (func)
				*ofile << "\tarr[i] += " + number + ";\n";
			else
				output +=  "\tarr[i] += " + number + ";\n";
			number = "";
			break;
		case '-':
			if (func)
				*ofile << "\tarr[i] -= " + number + ";\n";
			else
				output += "\tarr[i] -= " + number + ";\n";
			number = "";
			break;
		case '>':
			if (func)
				*ofile << "\ti+= " + number + ";\n";
			else
				output += "\ti+= " + number + ";\n";
			number = "";
			break;
		case '<':
			if (func)
				*ofile << "\ti-= " + number + ";\n";
			else
				output += "\ti-= " + number + ";\n";
			number = "";
			break;
		case '.':
			if (func)
				*ofile << "\tcout<<arr[i];\n";
			else
				output += "\tcout<<arr[i];\n";
			break;
		case ',':
			if (func)
				*ofile << "\tcin>>arr[i];\n";
			else
				output += "\tcin>>arr[i];\n";
			break;

		case '[':
			if (func)
				*ofile << "\twhile(arr[i]!=0){\n";
			else
				output += "\twhile(arr[i]!=0){\n";
			break;

		case ']':
			if (func)
				*ofile << "\t}\n";
			else
				output += "\t}\n";
			break;
		case '0':
			number += "0";
			break;
		case '1':
			number += "1";
			break;
		case '2':
			number += "2";
			break;
		case '3':
			number += "3";
			break;
		case '4':
			number += "4";
			break;
		case '5':
			number += "5";
			break;
		case '6':
			number += "6";
			break;
		case '7':
			number += "7";
			break;
		case '8':
			number += "8";
			break;
		case '9':
			number += "9";
			break;
		case '|':
			*ofile << "void ";
			func = true;
			break;
		case '(':
			*ofile << "{";
			break;
		case ')':
			*ofile << "}";
			func = false;
			break;
		default:
			if (isalpha(c))
			{
				if (func)

					*ofile << c << "()";
				else
					output += string(1,c) + "();";
			}
			break;
		}

			
	}
	*ofile << output + "}";
}

int main(int argc, char *argv[])

{
	if (argc < 2)
		return 1;
	try
	{

		ifstream ifile(argv[1]);
		string path;
		if (argc > 2)
		{
			path = argv[2];
		}
		else 
		{
			path = "o.obf";
		}
		ofstream ofile(path);
		optimise(&ifile, &ofile);
		
		ofile.close();
		ifile.close();
		ifile.open(argv[1]);
		if (!error(&ifile, path))
		{
			ifile.close();
			ifile.open(path);
			ofile.open("o.cpp");
			compile(&ifile, &ofile);
			ifile.close();
			ofile.close();
			system("g++ o.cpp");
		}
		else
		{
			ifile.close();
			cout << "x";
		}
		
	}
	catch (...){
		return 1;
	}
}