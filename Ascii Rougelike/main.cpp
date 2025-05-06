#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <cwchar>

#include "Creator.h"
#include "Write.h"
#include "Weapon.h"
#include "Character.h"
#include "Duel.h"

using namespace std;



int main(int argc, char** argv)
{

	Write::initiateFont();
	bool running = true;
	int selection = 1;

	Creator creator;
	Duel duel;

	int characterLevel = 1;

	srand(time(NULL));
	while (running)
	{
		string menuLines[] = { "New Duel", "Create New Character", "Something", "Graphic Tutorial", "Creator", "Quit" };

		selection = Write::menu("The Duel Game - Alpha 0.2", menuLines, 6, selection, "Green");


		switch (selection)
		{
		case 1:
		{
			Character p1;
			Character p2;

			p1.setName("choosing--"); //if the player name is this, it is still being choosen
			p2.setName("choosing--");

			int optionSelection = 1;
			string optionLines[] = { "Load Character", "Create Character", "Random Selection", "Random" };

			while (p1.getName() == "choosing--")
			{
				optionSelection = Write::menu("Choose character for Player 1:", optionLines, 4, optionSelection, "Yellow");
				if (optionSelection == 1) p1 = Character::savedCharacters();
				else if (optionSelection == 2) p1 = Character::characterCreator();
				else if (optionSelection == 3) p1 = Character::characterChooser(characterLevel, "Player 1: ");
				else if (optionSelection == 4)
				{
					p1.generateRandom(characterLevel, 0);
				}
				else
				{
					break;
				}
			}
			if (p1.getName() == "choosing--")
			{
				break;
			}

			optionSelection = 1;
			while (p2.getName() == "choosing--")
			{
				optionSelection = Write::menu("Choose character for Player 2:", optionLines, 4, optionSelection, "Yellow");
				if (optionSelection == 1) p2 = Character::savedCharacters();
				else if (optionSelection == 2) p2 = Character::characterCreator();
				else if (optionSelection == 3) p2 = Character::characterChooser(characterLevel, "Player 2: ");
				else if (optionSelection == 4)
				{
					p2.generateRandom(characterLevel, 0);
				}
				else
				{
					break;
				}
			}
			if (p2.getName() == "choosing--")
			{
				break;
			}

			if (Write::yornFull("Player2 AI?"))
				p2.setAI(true);


			Duel duel;
			duel.run(&p1, &p2);
		}
		break;
		case 2:
		{
			Character::characterCreator();
		}
		break;

		case 3:
		{
			
		}
		break;

		case 4:
		{
			
			//return 1;
		}
		break;
		case 5:
			creator.run();
			break;
		case 6:
			running = false;
			break;
		case -1:
		{
			if (Write::yornFull("Really Quit?"))
				running = false;

			selection = 1;
		}
			break;

		default:

			break;
		}
	}

	return 0;
}
