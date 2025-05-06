#include <string>
#include <iostream>
#include <cwchar>

#pragma once

using namespace std;


class Write
{
public:

	static void wrt(string output); //prints the output on the screen
	static void wrtc(string output, string bColor, string fColor); //prints the ouptut on the screen in the desired color
	static void wrtc(string output, string fColor); //prints the ouptut on the screen in the desired color
	static int menu(string title, string lines[], int numLines, int selection, string color); //Menu template. Returns the menu selection.
	static int menu(string title, string lines[], string descriptions[], int numLines, int selection, string color); //Menu with descriptions
	static int menu(string title, string subTitle, string lines[], string descriptions[], int numLines, int selection, int color); //Menu with descriptions

	static void redGreen(int input);

	static void wc(string output, int bcolor, int fcolor); //prints to the screen with the desired color

	static void setColor(int bcolor, int fcolor); //set the console text color

	static void initiateFont(); //initiate the console font

	static bool yorn();
	static bool yornFull(string question);

private:

};

