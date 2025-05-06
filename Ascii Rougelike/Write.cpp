#include "Write.h"

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <array>
#include <string>



void Write::wrt(string output)
{
	cout << output;
}

void Write::wrtc(string output, string bColor, string fColor)
{	
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int color = 7;

	if (fColor == "Blue") color = 1;
	else if (fColor == "Red") color = 4;
	else if (fColor == "Yellow") color = 14;
	else if (fColor == "Purple") color = 5;
	else if (fColor == "Green") color = 2;
	else if (fColor == "White") color = 15;
	else if (fColor == "LBlue") color = 9;
	else if (fColor == "Gold") color = 6;
	else if (fColor == "DGrey") color = 8;
	else if (fColor == "Hlt") color = 112;

	SetConsoleTextAttribute(hstdout, color);

	cout << output;

	SetConsoleTextAttribute(hstdout, 7);
}

void Write::wc(string output, int bcolor, int fcolor)
{
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int color = fcolor + (bcolor * 16);

	SetConsoleTextAttribute(hstdout, color);

	cout << output;

	SetConsoleTextAttribute(hstdout, 7);
}

void Write::wrtc(string output, string fColor)
{
	wrtc(output, "", fColor);
}

int Write::menu(string title, string lines[], int numLines, int selection, string color)
{
	bool running = true;


	char input;

	while (running)
	{
		system("cls");

		//display
		wrtc(title, color);

		cout << "\n\n";

		for (int i = 1; i< numLines + 1; i++)
		{
			if (i == selection) wrtc(lines[i - 1], "Hlt");
			else  wrt(lines[i - 1]);
			cout << "\n";
		}


		input = _getch();
		//determine action
		switch (input)
		{

		case 'w':
			if (selection > 1) selection--;
			break;

		case 's':
			if (selection < numLines) selection++;
			break;

		case 'e':

			return selection;
			break;

		case 'q':
			return -1;
			break;
		default:
			break;

		}

	}


	return 0;
}

int Write::menu(string title, string lines[], string descriptions[], int numLines, int selection, string color)
{
	bool running = true;


	char input;

	while (running)
	{
		system("cls");

		//display
		wrtc(title, color);

		cout << "\n\n";

		for (int i = 1; i < numLines + 1; i++)
		{
			if (i == selection) wrtc(lines[i - 1], "Hlt");
			else  wrt(lines[i - 1]);
			cout << "\n";
		}

		cout << endl << endl << descriptions[selection - 1] << endl;

		input = _getch();
		//determine action
		switch (input)
		{

		case 'w':
			if (selection > 1) selection--;
			break;

		case 's':
			if (selection < numLines) selection++;
			break;

		case 'e':

			return selection;
			break;

		case 'q':
			return -1;
			break;
		default:
			break;

		}

	}


	return 0;
}

int Write::menu(string title, string subTitle, string lines[], string descriptions[], int numLines, int selection, int color) //used in a particularWay
{
	bool running = true;


	char input;

	while (running)
	{
		system("cls");

		//display
		wc(title, 0, color);

		cout << endl << subTitle;

		cout << "\n\n";

		for (int i = 1; i < numLines + 1; i++)
		{
			if (i == selection) wrtc(lines[i - 1], "Hlt");
			else  wrt(lines[i - 1]);
			cout << "\n";
		}

		cout << endl << endl << descriptions[selection - 1] << endl;
		input = _getch();
		//determine action
		switch (input)
		{

		case 'w':
			if (selection > 1) selection--;
			break;

		case 's':
			if (selection < numLines) selection++;
			break;

		case 'e':

			return selection;
			break;

		case 'q':
			return -1;
			break;
		default:
			break;

		}
	}
	return 0;
}

void Write::setColor(int bcolor, int fcolor)
{
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int color = fcolor + (bcolor * 16);

	SetConsoleTextAttribute(hstdout, color);
}


void Write::initiateFont()
{
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = 0;                   // Width of each character in the font
	fontInfo.dwFontSize.Y = 27;                  // Height
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_LIGHT;
	wcscpy_s(fontInfo.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
}

bool Write::yorn()
{
	char answer = ' ';

	while (answer != 'y' && answer != 'n')
	{
		answer = _getch();
	}
	
	if (answer == 'y') return true;
	else return false;
}

bool Write::yornFull(string question)
{
	system("CLS");
	cout << question << " y/n";
	return yorn();
}
void Write::redGreen(int input)
{
	int color;

	if (input > 0) color = 2;
	else if (input < 0) color = 4;
	else color = 7;

	if (input > 0)
		wc("+" + to_string(input), 0, color);
	else
		wc(to_string(input), 0, color);
}