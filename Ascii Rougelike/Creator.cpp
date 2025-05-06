#include "Creator.h"

#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "Write.h"


using namespace std;

Creator::Creator()
{
}


Creator::~Creator()
{
}

void Creator::run()
{
	bool running = true;
	int selection = 1;


	while (running)
	{
		string menuLines[] = { "Maps", "Items", "Enemies", "Back" };

		selection = Write::menu("The Creator", menuLines, 4, selection, "Green");


		switch (selection)
		{
		case 2:
			itemEditor();
			break;
		case 4:
			running = false;
			break;
		}
	}
}

void Creator::itemEditor()
{
	bool running = true;
	int selection = 1;


	while (running)
	{
		string menuLines[] = { "Weapons", "Armor", "Back" };

		selection = Write::menu("Item Editor", menuLines, 3, selection, "Purple");


		switch (selection)
		{
		case 1:
			displayWeap();
			break;
		case 3:
			running = false;
			break;
		}
	}
}




//This function reads in weapon archeotype information and displays the names of the weapons in a menu
void Creator::displayWeap()
{
	vector<Weapon> archeotypes;

	const string textFile = "WeaponArcheotypes.txt";

	ifstream fin;

	string input;
	string input2;

	fin.open(textFile);
	if (fin.fail()) {
		perror(textFile.c_str());
		return;
	}

	while (getline(fin, input))
	{
		Weapon tempWep;

		//input Name
		tempWep.setName(input);

		//input Weapon class
		getline(fin, input);
		tempWep.setClass(input);

		//Input Damage info
		getline(fin, input);
		getline(fin, input2);
		tempWep.setDamage(stoi(input2), stoi(input));

		//Input Slashing
		getline(fin, input);
		tempWep.setSlashing(stof(input));

		//Input Piercing
		getline(fin, input);
		tempWep.setPiercing(stof(input));

		//Input Crushing
		getline(fin, input);
		tempWep.setCrushing(stof(input));

		//Input Reach
		getline(fin, input);
		tempWep.setReach(stof(input));

		//Input Speed
		getline(fin, input);
		tempWep.setSpeed(stof(input));

		//Input Defensive Bonus
		getline(fin, input);
		tempWep.setDef(stoi(input));

		//Input OneHand Difficulty
		getline(fin, input);
		tempWep.setOneHand(stoi(input));

		//Input TwoHand Difficulty
		getline(fin, input);
		tempWep.setTwoHand(stoi(input));

		//Input Rarity
		getline(fin, input);
		tempWep.setRarity(stoi(input));

		//Input Value
		getline(fin, input);
		tempWep.setValue(stoi(input));

		//Input Value
		getline(fin, input);
		tempWep.setBalance(stof(input));

		archeotypes.push_back(tempWep);

		getline(fin, input);

	}

	//now display the names on the screen in a menu
	bool running = true;
	int selection = 1;
	int numberof = archeotypes.size();



	string menuLines[100];

	for (int i = 0; i < numberof; i++)
	{
		menuLines[i]= archeotypes[i].getName();
	}
	menuLines[numberof] = " ";
	menuLines[numberof+1] = "Create New Archeotype";
	menuLines[numberof + 2] = "Exit";

	while (running)
	{

		selection = Write::menu("Weapon Archeotypes", menuLines, numberof + 3, selection, "Red");

		if (selection <= numberof)
		{
			system("CLS");
			archeotypes[selection - 1].display();
			_getch();
		}

		if (selection == numberof + 2)
		{

		}

		if (selection == numberof + 3) return;
	}


	

}