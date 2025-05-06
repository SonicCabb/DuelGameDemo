#include "Duel.h"



Duel::Duel()
{

	move = 0;
	attacking = true;

	attOp = -1;
	defOp = -1;

	round = 0;
	phase = 0;

	_postureLock = true;
	_advanced = true;
}


Duel::~Duel()
{
}

int Duel::run(Character *p1, Character *p2)
{
	//the players are pointers to the players
	player1 = p1;
	player2 = p2;
	
	if (player1->getColor() == player2->getColor()) player2->setAltColorNeeded(true);

	//this sets the starting reaction of each character
	player1->setReaction(player1->getStartingReact());
	player2->setReaction(player2->getStartingReact());
	
	
	//player one is always the starting attacker. They get the first offensive move.
	player1->setAttacking(true);
	player2->setAttacking(false);
	

	//determine advantages and who goes first
	setAdvantage();
	
	//if player 1 has advantage, they go second
	if (advantage == 1)
	{
		players[0] = player2;
		players[1] = player1;
	}
	else
	{
		players[0] = player1;
		players[1] = player2;
	}

	

	//bool for the loop. False when someone wins
	bool running = true;

	//bool for that is true while waiting for input
	bool waiting = false;

	//input from the keyboard
	char input;
	while (running)
	{
		int other;

		//"phase" and "other" need to be opposites
		if (phase == 1) other = 0;
		else other = 1;

		//display information on the screen if player is real
		if (!players[phase]->isAI())
		{
			display(false);
			waiting = true;	//if the player isn't an ai, wait for a correct input
			

		}
		else//if the player is an ai, get it's decision
		{
			
			/*
			string otherchoice; int otherWepIndex;
			if (phase == 1 && players[phase]->getAttacking())
			{
				otherchoice = defendChoice();
				otherWepIndex = _defenderWeaponIndex;
			}
			else if (phase == 1 && !players[phase]->getAttacking())
			{
				otherchoice = attackChoice(players[other]);
				otherWepIndex = _attackerWeaponIndex;
			}
			else
			{
				otherchoice = "";
				otherWepIndex = 0;
			}
			*/

			//int aiChoice = players[phase]->aiDecision(players[other], phase, otherchoice, otherWepIndex);
			
			int aiChoice = players[phase]->aiChoice(players[other], phase);
			
			char choice = aiChoice + 48;

			if (players[phase]->getAttacking())
			{
				//setAttackWepIndex(choice);
				setupCharacterOptions(true, choice, players[phase], players[other]);
			}
			else
			{
				//setDefenseNumber(choice);
				setupCharacterOptions(false, choice, players[other], players[phase]);
			}



			waiting == false;
			phase++; //at the moment we are assuming the ai picks a valid option
		}

		while (waiting)
		{
			//get input from the player
			input = _getch();

			//if the input is one of the number keys
			if (input >= '0' && input <= '9')
			{
				//determine the 
				bool success; //success is true if it is a valid option
				if (players[phase]->getAttacking())
					success = setupCharacterOptions(true, input, players[phase], players[other]);
				else
					success = setupCharacterOptions(false, input, players[other], players[phase]);

				if (success)
				{
					//we can move on to the next phase, because a successful attack/defense has been done
					//attOp = input;

					//this should fix a bug in the code...these weren't being set if human decision...ai needs them
					//if (players[phase]->getAttacking()) setAttackWepIndex(input);
					//else setDefenseNumber(input);

					waiting = false;
					phase++;
				}
	
			}
			//if the input is h there is a help screen
			else if (input == 'h')
			{
				//in theory
			}
			else if (input == 'l')
			{
				_postureLock = !_postureLock;
				
				waiting = false;
			}
			else if (input == 'a')
			{
				if (!_postureLock)
				{
					if (changePosture(players[phase], -1))
					{
						waiting = false;
					}
				}
			}
			else if (input == 'd')
			{
				if (!_postureLock)
				{
					if (changePosture(players[phase], 1))
					{
						waiting = false;
					}
				}
			}

			else if (input == 'w')
			{
				if (players[phase]->getAttacking())
				{
					changeTargetArea(players[phase], players[other], 1);
					waiting = false;
				}
			}
			else if (input == 's')
			{
				if (players[phase]->getAttacking())
				{
					changeTargetArea(players[phase], players[other], -1);
					waiting = false;
				}
			}
			else if (input == 't')
			{
				cycleAdvanced();
				waiting = false;
			}

			//if the input is c, display the current characters stats
			else if (input == 'c')
			{
				system("cls");
				players[phase]->display();
				_getch();
				system("cls");
				waiting = false;
			}
			else if (input == 'e')//display enemy stats
			{
				int num = phase + 1;
				if (num == 2) num = 0;
				
				system("cls");
				players[num]->display();
				_getch();
				system("cls");
				waiting = false;
			}
			//if the input is q then we should quit
			else if (input == 'q')
			{
				waiting = false;
				running = false;
			}
			else if (input == 'm')
			{
				players[phase]->viewModifiers();
				waiting = false;
			}
			


		}
		
		//when the phase is 2, we know that both attacker defender have selected their options, so we will execute the clash and move on to the next round.
		if (phase == 2)
		{
			//when the round is up, execute the combat, and move on to the next round
			characterClash();
			nextRound();
		}
	}
	return 0;
}

void Duel::display(bool justTop)
{
	system("CLS");

	string pl1 = player1->getName();
	string pl2 = player2->getName();

	if (player1->getAttacking())
	{
		pl1 = pl1 + " - Attacker";
		pl2 = "Defender - " + pl2;
	}
	else
	{
		pl2 = "Attacker - " + pl2;
		pl1 = pl1 + " - Defender";
	}
	cout << setw(width) << left;
	Write::wc(pl1, 0, player1->getColor());
	cout << setw(width) << right;
	Write::wc(pl2, 0, player2->getColor());
	cout << endl;


	healthBars();
	reaction();
	posture();

	cout << endl;

	//if still waiting on decisions from players
	if (phase < 2) 
	{
		Write::setColor(7, 0);
		string dispRound = to_string(round + 1) + "-" + to_string(phase);
		cout << "Round " << setw(width - 6) << left << dispRound;
		for (int i = 0; i < width; i++)cout << " ";
		Write::setColor(0, 7);
	}
	else
	{
		
		Write::setColor(7, 0);
		string dispRound = to_string(round + 1);
		cout << "Clash! | Round " << setw(width - 15) << left << dispRound;
		for (int i = 0; i < width; i++)cout << " ";
		Write::setColor(0, 7);
	}

	//if displaying everything
	if (!justTop)
	{
		//if the in the reaction phase, add information for the previous players move
		if (phase == 1)
		{


			cout << "Last move by  ";
			players[0]->printName(false, 0);
			cout << ": ";

			players[0]->printOptionName(false);

			//display attack/defense from other player
			if (players[0]->getAttacking() && (players[0]->getOption() == 0 || players[0]->getOption() == 1)) //if the other player is attacking
			{
	
				cout << " | ";
				cout << players[0]->getFullAttDefVal(true, players[0]->isTwoHandedWielding(), players[0]->getWepSlotOption(), players[0]->getAttTypeOption(), 0, players[1]->getTargetArea(players[0]->getAreaTargeted()));
				cout << (char)175;

				cout << " - Target: " << players[1]->getTargetArea(players[0]->getAreaTargeted())->name;

			}
			else if (!players[0]->getAttacking() && players[0]->getOption() != -1) //if defending
			{
				cout << " | ";
				cout << players[0]->getFullAttDefVal(false, players[0]->getOption(), players[0]->getWepSlotOption(), players[0]->getAttTypeOption(), 0, players[1]->getTargetArea(players[0]->getAreaTargeted()));
			}


			cout << endl;
		}

		//options();
		if (players[phase]->getAttacking())
			displayAttackOptions();
		else
			displayDefenseOptions();

		cout << endl << "(0) Do Nothing" << endl;

		cout << endl << endl << "(a)-posture+(d) ";
		
		if (_postureLock) Write::wc("(l)ocked", 0, 2);
		else Write::wc("un(l)ocked", 0, 4);
		
		cout << "  (w)-target-(s)  (c)haracter  (e)nemy  (m)odifiers  (t)oggle adv     (h)elp  (q)uit" << endl;
	}
}

void Duel::healthBars()
{
	const int barsize = 20;

	char bar1[barsize];
	char bar2[barsize];

	int numGreen1 = ((float)player1->getCurrentHp() / (float)player1->getmaxHp()) * barsize;
	int numGreen2 = ((float)player2->getCurrentHp() / (float)player2->getmaxHp()) * barsize;

	string num1 = to_string(player1->getCurrentHp());
	int numm1 = num1.size();

	string num2 = to_string(player2->getCurrentHp());
	int numm2 = num2.size();


	for (int i = 0; i < barsize; i++)
	{
		if (i < numm1) bar1[i] = num1[i];
		else bar1[i] = ' ';
		if (i < numm2) bar2[barsize - 1 - i] = num2[numm2 - 1 - i];
		else bar2[barsize - 1 - i] = ' ';
		
	}

	string temp;
	for (int i = 0; i < numGreen1; i++)
	{
		temp = bar1[i];
		Write::wc(temp, 2, 0);
	}
	for (int i = numGreen1; i < barsize; i++)
	{
		temp = bar1[i];
		Write::wc(temp, 8, 0);
	}

	for (int i = 0; i < width + width - barsize - barsize; i++) cout << ' ';
	for (int i = 0; i < barsize - numGreen2; i++)
	{
		temp = bar2[i];
		Write::wc(temp, 8, 0);
	}
	for (int i = barsize - numGreen2; i < barsize; i++)
	{
		temp = bar2[i];
		Write::wc(temp, 2, 0);
	}

	cout << endl;
}

void Duel::reaction()
{
	if (advantage == 1)
	{
		cout << setw(10) << left << "Reaction ";
		Write::setColor(0, 2);
		cout << setw(3) << left << player1->getReact();
		Write::setColor(0, 7);

		for (int i = 0; i < (width*2) - 20 - 6; i++) cout << " ";

		cout << setw(10) << right << "Reaction ";
		cout << setw(3) << right << player2->getReact();
	}
	else
	{
		cout << setw(10) << left << "Reaction ";
		cout << setw(3) << left << player1->getReact();
		for (int i = 0; i < (width*2) - 20 - 6; i++ ) cout << " ";
		cout << setw(10) << right << "Reaction ";
		Write::setColor(0, 2);
		cout << setw(3) << right << player2->getReact();
		Write::setColor(0, 7);
		
	}

	cout << endl;
}

void Duel::posture()
{

	if (player1->getPosture() == -1) Write::setColor(0, 6);
	else if (player1->getPosture() == 1) Write::setColor(0, 4);

	cout << setw(width) << left << player1->getPostureString();
	Write::setColor(0, 7);


	if (player2->getPosture() == -1) Write::setColor(0, 6);
	else if (player2->getPosture() == 1) Write::setColor(0, 4);

	cout << setw(width) << right << player2->getPostureString();
	Write::setColor(0, 7);
	cout << endl;
}

void Duel::nextRound()
{
	//reset the phase
	phase = 0;

	//move on to next round
	round++;

	//this switched who the attacker and defender are
	player1->cycleAttacking();
	player2->cycleAttacking();

	//cycle the weapon modifiers

	for (int i = 0; i < player1->numEquippedWeapons(); i++)
		player1->getWeapon(i)->cycleModRounds();

	for (int i = 0; i < player2->numEquippedWeapons(); i++)
		player2->getWeapon(i)->cycleModRounds();

	player1->cycleModRounds();
	player2->cycleModRounds();

	//this changes the order of which the players can choose their option. The player with the advantage always goes second.
	
	setAdvantage();

	if (advantage == 1)
	{
		players[0] = player2;
		players[1] = player1;
	}
	else
	{
		players[0] = player1;
		players[1] = player2;
	}


	//Posture Lock Starts out locked
	_postureLock = true;

}

void Duel::characterClash()
{
	//pointer to the current attacker and defender
	Character* attacker, * defender;

	int attack; //total attack value
	int defense; //total defense value

	

	//determine the attacker and defender (set the pointers)
	if (player1->getAttacking())
	{
		attacker = player1;
		defender = player2;
	}
	else
	{
		attacker = player2;
		defender = player1;
	}

	Character::targetArea* target = defender->getTargetArea(attacker->getAreaTargeted());

	//Determine what they are doing 
	int attOption = attacker->getOption();
	int defOption = defender->getOption();


	//weapon advantage
	int attAdvantage = 0;
	int defAdvantage = 0;

	//if two weapons are clashing, then determine the weapon advantage
	if (attacker->getWepSlotOption() != -1 && defender->getWepSlotOption() != -1)
		attAdvantage += attacker->getCurrentWeapon()->weaponAdvantage(true, defender->getCurrentWeapon());

	//Advantage is different against a dodge
	if (defOption == 3 && attacker->getWepSlotOption() != -1)
		attAdvantage += attacker->getCurrentWeapon()->getDodgeModifier();

	//a negative attack advantage is a positive defender advantage
	if (attAdvantage < 0)
	{
		defAdvantage -= attAdvantage;
		attAdvantage = 0;
	}


	//determine attack values
	attack = attacker->getFullAttDefVal(true, attOption, attacker->getWepSlotOption(), attacker->getAttTypeOption(), attAdvantage, target);
	defense = defender->getFullAttDefVal(false, defOption, defender->getWepSlotOption(), defender->getAttTypeOption(), defAdvantage);


	//determine who wins the clash
	bool attackSuccess;
	int attRoll = 0; int defRoll = 0;
	attackSuccess = success(attack, defense, attRoll, defRoll);

	attacker->setAttRoll(attRoll);
	defender->setDefRoll(defRoll);

	//should restore canceled defense after a turn
	for (int i = 0; i < defender->numEquippedWeapons(); i++)
		if (defender->getWeapon(i)->isDefenseCanceled())
		{
			defender->getWeapon(i)->restoreDefense();
		}
	//Display Stuff
	system("CLS");


	//this makes the names appear even
	int namePadding = 3;
	if (attacker->getName().size() > defender->getName().size())
		namePadding += attacker->getName().size();
	else
		namePadding += defender->getName().size();


	display(true);
	cout << endl << endl;
	
	//counter special display
	if (defOption == 4 && attOption == 4)//(attOption == 0 || attOption == 1)) //if there is a counter, we have the guaranteed hit first if the attacker did in fact attack
	{
		attacker->printName(true, namePadding);
		cout << "Att: ";

		//sets the attacker option to their original attack, then back to counter dodge
		attacker->setOption(attacker->isTwoHandedWielding());
		attacker->printOptionName(false);
		attacker->setOption(4);
		
		cout << "(Guaranteed hit into counter!)" << endl;

		damageChar(attacker, defender, attacker->getAttTypeOption(), namePadding);
		cout << endl;

		_getch();
	}


	//this makes the attack stuff appear even
	int attOpLength = attacker->printOptionName(true) + to_string(attack).size();
	int defOpLength = defender->printOptionName(true) + to_string(defense).size();

	if (attOpLength > defOpLength)
	{
		attOpLength -= (defOpLength - 3);
		defOpLength = 3;
		
	}
	else
	{
		defOpLength -= (attOpLength - 3);
		attOpLength = 3;
	}


	//Print ATTACKER information
	attacker->printName(true, namePadding);
	cout << "Att: ";
	attacker->printOptionName(false);
	
	if (attOption != -1)
	{
		cout << " " << attack;
		for (int i = 0; i < defOpLength; i++) cout << " ";
		if (defOption != -1) {
			if (attRoll > defRoll)
				Write::wc("(" + to_string(attRoll) + " roll)", 0, 2);
			else
				cout << "(" << attRoll << " roll)";
		}
		if (attackSuccess) cout << " Success!";
		
	}
	cout << endl;
	cout << setw(namePadding) << ' ' << "Target: " << defender->getTargetArea(attacker->getAreaTargeted())->name << endl << endl;

	//print DEFENDER information
	defender->printName(true, namePadding);
	cout << "Def: ";
	defender->printOptionName(false);

	if (defOption != -1 && attOption != -1)
	{
		cout << " " << defense;
		for (int i = 0; i < attOpLength; i++) cout << " ";
		if (defRoll > attRoll)
			Write::wc("(" + to_string(defRoll) + " roll)", 0, 2);
		else 
			cout << "(" << defRoll << " roll)";

		if (!attackSuccess) cout << " Success!";
		
	}
	cout << endl << endl;



	//do damage
	if (attackSuccess)
	{
		if (attOption == 0 || attOption == 1)
		{
			damageChar(attacker, defender, attacker->getAttTypeOption(), namePadding);
			
			if (attacker->getCurrentWeapon()->getClass() == "Unarmed") //if successfully punch an opponent, take damage
				damageChar(attacker, 1, namePadding);
		
		}
		
	}
	else
	{
		//counter success. The -1 for the type indicates to do the highest value damage attack
		if (defOption == 4 && attOption == 4) //attack option becomes 4 if defence is a counterattack
		{
			damageChar(defender, attacker, -1, namePadding);
		}

		if ((defOption == 0 || defOption == 1 ) && (attOption == 0 || attOption == 1) && defender->getCurrentWeapon()->getClass() == "Unarmed" && attacker->getCurrentWeapon()->getClass() != "Polearm") //if a successful block with a fist, you take damage, unless it's a polearm
		{
			double multipliers[3] = { 0.5, 0.2, 0.2 };
			damageChar(defender, attacker->getCurrentWeapon()->randomDamage(attacker->getAttTypeOption(), multipliers), namePadding);
		}
	}

	//reaction changes

	if (attOption == -1)
	{
		attacker->printName(true, namePadding);
		Write::redGreen(attacker->alterReaction((attacker->getReflex() / 2.0) + 1));
		cout << " reaction" << endl;


	}
	if (defOption == -1 && attOption == -1)
	{
		defender->printName(true, namePadding);
		Write::redGreen(defender->alterReaction((defender->getReflex() / 2.0) + 1));
		cout << " reaction" << endl;
	}



	//Add Weapon modifiers


	cout << endl;

	determineModifiers(true, attacker, defender, attackSuccess, namePadding);
	
	cout << endl << endl << "(B)egin next round.";
	
	while (_getch() != 'b')
	{
	}

	attacker->setLastAreaTargeted(attacker->getAreaTargeted());
}

bool Duel::success(int desired, int opposing, int &roll, int &roll2)
{
	if (desired != 0) roll = (rand() % desired) + 1;
	else roll = 0;
	if (opposing != 0) roll2 = (rand() % opposing) + 1;
	else roll2 = 0;

	if (desired == 0 && opposing == 0) //not sure how to get here, but check anyway
	{
		roll = 0;
		roll2 = 0;
		return true;
	}

	while (roll == roll2) //no ties
	{
		if (desired != 0) roll = (rand() % desired) + 1;
		else roll = 0;
		if (opposing != 0) roll2 = (rand() % opposing) + 1;
		else roll2 = 0;
	}

	
	if (roll > roll2)
		return true;
	else
		return false;
}

/*
string Duel::attackChoice(Character * attacker)
{

	if (_attackerWeaponIndex < 0)
		return "Still";
	else
	{
		if (attacker->getWeapon(_attackerWeaponIndex)->wepAttack( _attackerAttackIndex).type == 3) //feint
			return "Feint";
		else
			return "Attack";
	}

}
string Duel::defendChoice()
{
	if (_defenderWeaponIndex == -2)
		return "Still";
	else if (_defenderWeaponIndex == -1)
		return "Dodge";
	else
	{
		if (_defenderDefenseIndex == 0)
			return "Parry";
		else
			return "Counter";
	}

}
*/

void addModifier(bool display, Weapon* weapon, Character* character, int type, int value, int rounds, string name, string reason, int displayPadding)
{//type -1 is whole weapon, type -2 is parry, otherwise it is for a weapon slot

	switch (type)
	{
	case -2://parry
		weapon->newParryMod(name, value, rounds);
		break;
	case -1://whole weapon
		weapon->newAttackMod(name, value, rounds);
		break;

	default://specific attack
		weapon->newAttackMod(name, type, value, rounds);
		break;
	}


	if (display)
	{
		character->printName(true, displayPadding);
		cout << "gets a ";
		Write::redGreen(value);
		cout << "% ";
		if (type == -2) cout << "parry";
		else cout << (char)175;
		cout << " modifier on their ";
		weapon->displayName();
		if (type >= 0) cout << " " << weapon->wepAttack(character->getAttTypeOption()).name;
		cout << " " << reason << endl;
	}
	
}

void addModifier(bool display, Character* character, int type, int value, int rounds, string name, string reason, int displayPadding)
{//type 0 is attack, type 1 is parry, 2 is dodge

	character->newMod(type, name, value, rounds);


	if (display)
	{
		character->printName(true, displayPadding);
		cout << "gets a ";
		Write::redGreen(value);
		cout << "% ";
		if (type == 0) cout << (char)175;
		else if (type == 1) cout << "parry";
		else if (type == 2) cout << "dodge";
		cout << " modifier " << reason << endl;
	}

}

void Duel::determineModifiers(bool display, Character* attacker, Character* defender, bool success, int displayPadding)
{
	//penalty/bonus to mods based on posture
	const double POSTURE_PENALTY = 0.5;
	
	double attackPostPenalty = POSTURE_PENALTY * attacker->getPosture();
	double defensePostPenalty = POSTURE_PENALTY * defender->getPosture();

	Weapon* attackerWep = attacker->getCurrentWeapon();
	Weapon* defenderWep = defender->getCurrentWeapon();;

	int atOp = attacker->getOption();
	int defOp = defender->getOption();

	double modifierValue = 0;	//used for positive modier values
	double negModifierValue = 0;	//used for negative modifier values

	
	if (atOp != 4 && atOp != -1 && atOp != 2) //if an attack with a weapon, add stale penalty
	{	
		_getch();
		//stale modifier
		double staleValue = -30;
		double staleValueForWeapon = -30;
		
		//stale value higher for defensive posture, lower for attacking posture
		staleValue -= staleValue * attackPostPenalty;
		staleValueForWeapon -= staleValueForWeapon * attackPostPenalty;


		//addModifier(display, attackerWep, attacker, -1, staleValue, 1, "Stale Penalty", "for using their weapon.", displayPadding);
		addModifier(display, attackerWep, attacker, attacker->getAttTypeOption(), staleValueForWeapon, 1, "Stale Penalty", "as a stale penalty.", displayPadding);


		if (success) //if the attack was a success, add parry bonus to the attackers weapon
		{
			double parryBonus = 30;
			parryBonus -= parryBonus * attackPostPenalty; //defensive makes this bonus more

			addModifier(display, attackerWep, attacker, -2, parryBonus, 1, "Successful Attack Bonus", "for their successful hit.", displayPadding);

		}
		
	}

	
	if (!success && (defOp == 0 || defOp == 1) && (atOp == 0 || atOp == 1))
	{	
		_getch();
		//attack bonus for successful parry
		double parryAttackBonus = 30;
		parryAttackBonus -= parryAttackBonus * defensePostPenalty;

		addModifier(display, defenderWep, defender, -1, parryAttackBonus, 1, "Successful Parry Bonus", "for their successful parry.", displayPadding);


		if (defenderWep->getClass() == "Unarmed" && attackerWep->getClass() == "Polearm") //parrying a polearm
		{
			addModifier(display, attackerWep, attacker, -1, -70, 1, "Weapon Grabbed Penalty", "for getting their " + attackerWep->getName() + " grabbed", displayPadding);
		}
		
	}

	

	if (success && atOp == 2)
	{	
		_getch();
		//Successful Feint Bonuses
		double feintSelfBonus = attacker->getCurrentAttackRoll(); //Bonus equal to your feint attack on your weapon
		double feintEnemyValue = feintSelfBonus * -0.5;			  //defensive penalty to enemy's weapon
		
		defenderWep->cancelDefense(); //cancels the defense bonus to advantage for a round

		feintSelfBonus += feintSelfBonus * attackPostPenalty; //higher bonus if in aggressive posture
		feintEnemyValue -= feintEnemyValue * defensePostPenalty; //worse penalty if aggressive posture


		addModifier(display, attackerWep, attacker, -1, feintSelfBonus, 1, "Successful Feint Bonus", "for their successful feint.", displayPadding);
		cout << setw(displayPadding) << " ";
		Write::redGreen(attacker->alterReaction((attacker->getReflex() / 4.0) + 1));
		cout << " reaction" << endl << endl;

		addModifier(display, defenderWep, defender, -2, feintEnemyValue, 1, "Feinted Against Penalty", "for being feinted against.", displayPadding);
		cout << setw(displayPadding) << " ";
		Write::redGreen(defender->alterReaction(((4 / (defender->getReflex() + 1)) + 1) * -1));
		cout << " reaction" << endl;


		if (display) //special callout to losing the weapons defense
		{
			if (defenderWep->getDef() > 0)
			{
				cout << setw(displayPadding) << " " << "They also lose it's ";
				Write::redGreen(defenderWep->getDef());
				cout << " defense bonus for a turn." << endl;
			}
			
		}

		
	}
	if (!success && atOp == 2)
	{
		_getch();

		//Failed Feint Bonus (Pernalty to your attack and defnese with your weapon
		double feintSelfBonus = defender->getCurrentAttackRoll() * -1.0; //Penalty equal to enemy's feint defense

		feintSelfBonus -= feintSelfBonus * attackPostPenalty; //worse penalty if in aggressive posture


		addModifier(display, attackerWep, attacker, -1, feintSelfBonus, 1, "Failed Feint Penalty", "for their failed feint.", displayPadding);
		addModifier(display, attackerWep, attacker, -2, feintSelfBonus, 1, "Failed Feint Penalty", "for their failed feint.", displayPadding);

		defender->printName(true, displayPadding);
		cout << " ";
		Write::redGreen(defender->alterReaction((defender->getReflex() / 4.0) + 1));
		cout << " reaction" << endl << endl;
	}

	
}

int Duel::damageChar(Character* characterAttacking, Character* characterDamaged, int attackType, int padding)
{
	Character::targetArea* target = characterDamaged->getTargetArea(characterAttacking->getAreaTargeted());

	int damageDone = 0;
	
	if (attackType != -1)
		damageDone = characterAttacking->getCurrentWeapon()->randomDamage(attackType, target->damageMultiplier);
	else//this will check all attacks, and pick the highest damage one (for counterattacks I think)
	{
		for (int i = 0; i < characterAttacking->getCurrentWeapon()->numberAttacks(); i++)
		{
			int temp = characterAttacking->getCurrentWeapon()->randomDamage(i, target->damageMultiplier);
			if (damageDone < temp) damageDone = temp;
		}
	}

	characterDamaged->alterHP(damageDone * -1);

	characterDamaged->printName(true, padding);
	Write::redGreen(damageDone * -1);
	cout << " hp  |  ";
	
	int reactionLost = ((damageDone / (characterDamaged->getToughness() + 4)) + 1) * -1;
	reactionLost = characterDamaged->alterReaction(reactionLost);
	
	Write::redGreen(reactionLost);

	cout << " reaction" << endl;

	if (target->attackPenalty != 0)//if there us a penalty due to beign hit in a specific spot
	{
		addModifier(true, characterDamaged, 0, target->attackPenalty, 1, target->name + " Injury", "for getting hit in the " + target->name, padding);
	}
	if (target->defPenalty != 0)//if there us a penalty due to beign hit in a specific spot
	{
		addModifier(true, characterDamaged, 1, target->defPenalty, 1, target->name + " Injury", "for getting hit in the " + target->name, padding);
	}
	if (target->movementPenalty != 0)//if there us a penalty due to beign hit in a specific spot
	{
		addModifier(true, characterDamaged, 2, target->movementPenalty, 1, target->name + " Injury", "for getting hit in the " + target->name, padding);
	}



	return damageDone;
}

int Duel::damageChar(Character* characterDamaged, int damage, int padding)
{


	characterDamaged->alterHP(damage * -1);

	characterDamaged->printName(true, padding);
	Write::redGreen(damage * -1);
	cout << " hp  |  ";

	int reactionLost = ((damage / (characterDamaged->getToughness() + 4)) + 1) * -1;
	reactionLost = characterDamaged->alterReaction(reactionLost);

	Write::redGreen(reactionLost);

	cout << " reaction" << endl;


	return damage;
}

void Duel::setAdvantage()
{
	if (player1->getReact() > player2->getReact()) //if player 1 has a higher reeaction, the player to first is player 2
		advantage = 1;
	else if (player1->getReact() < player2->getReact())
		advantage = 2;
	else //if the same reaction
	{
		if (player1->getAttacking() == true) advantage = 2;
		else advantage = 1;
	}
}


int determineAttackNamePadding(Character* character)
{
	int padding = 0;

	for (int i = 0; i < character->numEquippedWeapons(); i++)
	{
		for(int j = 0; j < character->getWeapon(i)->numberAttacks(); j++)
		{
			if (character->getWeapon(i)->wepAttack(j).name.length() > padding)
			{
				padding = character->getWeapon(i)->wepAttack(j).name.length();
			}
		}
	}

	padding += 2;

	return padding;
}

void Duel::displayAttackOptions()
{
	int other = (phase + 1) % 2; //other players phase

	int attackNamePadding = determineAttackNamePadding(players[phase]);

	Character::targetArea* target =  players[other]->getTargetArea(players[phase]->getAreaTargeted());

	cout << endl;
	players[phase]->printName(false, 0);
	cout << " - Attack Options - Target: ";

	cout << target->name;

	if (players[phase]->getAreaTargeted() == players[phase]->getLastAreaTargeted())
	{
		cout << " (stale ";
		Write::redGreen(-50);
		cout << "%)";
	}
	//imax is the number of equipped weapons
	int imax = players[phase]->numEquippedWeapons();

	for (int i = 0; i < imax; i++)
	{
		//pointer to the weapon in the "i" slot
		Weapon * wep = players[phase]->getWeapon(i);

		cout << endl << endl;
		wep->displayName();
		
		cout << " ";

		vector<int> advantages;

		//Display Weapon Advantage based on either the opposing weapon, or weapons
		if (phase == 1)
		{//If going second and being parried with a weapon, we'll show the weapon advantage
			int advantage;
			if (players[other]->getOption() != 3 && players[other]->getOption() >= 0)
				advantage = players[phase]->getWeapon(i)->weaponAdvantage(true, players[other]->getCurrentWeapon()); //This should be fine..just changed it
			else
				advantage = players[phase]->getWeapon(i)->getDodgeModifier();

			advantages.push_back(advantage);
			Write::redGreen(advantage);
		}
		else if (phase == 0)
		{

			int advantage;

			//loop through the opposing players weapons and determine advatage based off each
			for (int w = 0; w < players[other]->numEquippedWeapons(); w++)
			{
				if (w > 0) cout << "/";
				
				if (players[other]->getOption() != 3)
					advantage = players[phase]->getWeapon(i)->weaponAdvantage(true, players[other]->getWeapon(w));
				else
					advantage = players[phase]->getWeapon(i)->getDodgeModifier();

				advantages.push_back(advantage);
				Write::redGreen(advantage);
			}
		}


		//display attack bonus
		if (wep->getAttackModBonus() + players[phase]->getModBonus(0) != 0 && getAdvanced())
		{
			cout << " | (";
			Write::redGreen(wep->getAttackModBonus() + players[phase]->getModBonus(0));
			cout << "%)";
		}

		//loop through attacks
		for (int j = 0; j < wep->numberAttacks(); j++)
		{

			if ((wep->wepAttack(j).name == "Feint" && phase == 1) || (wep->wepAttack(j).name != "Feint")) //if the attack is a feint in phase 1 or not a feint
			{
				cout << endl;
				cout << "(" << j + (i * 4) + 1 << ") " << setw(attackNamePadding) << wep->wepAttack(j).name; //display attack name

				//display attack info 
				if (wep->wepAttack(j).name == "Feint")
				{
					cout << setw(7) << to_string(players[phase]->getFullAttDefVal(true, 2, i, j, 0, target)) + (char)175;

				}
				else
				{
					string attOutput = to_string(players[phase]->getFullAttDefVal(true, players[phase]->isTwoHandedWielding(), i, j, advantages[0], target));
					if (advantages.size() == 2) if (advantages[0] != advantages[1]) attOutput += "/" + to_string(players[phase]->getFullAttDefVal(true, players[phase]->isTwoHandedWielding(), i, j, advantages[1], target)) + (char)175;
					else attOutput += (char)175;
					cout << setw(7) << attOutput;
					
					if (getAdvanced())
					{
						string retString = attackBreakdown(true, players[phase]->isTwoHandedWielding(), i, j, 0, target);
						cout << setw(retString.length() + 7) << retString;
					}

					//display damage info
					if (wep->getDamage(false, j, target->damageMultiplier) != wep->getDamage(true, j, target->damageMultiplier))
						cout << setw(12) << "[" + to_string(wep->getDamage(false, j, target->damageMultiplier)) + "-" + to_string(wep->getDamage(true, j, target->damageMultiplier)) + " dmg]";
					else
						cout << setw(12) << "[" + to_string(wep->getDamage(true, j, target->damageMultiplier)) + " dmg]";
					
					
				}

				//display bonuses penalties on weapons
				if (wep->getAttackModBonus(j) != 0 && getAdvanced())
				{
					cout << " (";
					Write::redGreen(wep->getAttackModBonus(j));
					cout << "%)";
				}
				
				if (players[other]->getOption() == 4 && phase == 1)//if counter
						cout << " - Guaranteed Hit!";
			
			
			}
			

		}
	}

	cout << endl;

}

void Duel::displayDefenseOptions()
{
	int other = (phase + 1) % 2;

	cout << endl;
	players[phase]->printName(false, 0);
	cout << " - Defense Options" << endl;

	int imax = players[phase]->numEquippedWeapons();

	for (int i = 0; i < imax; i++)
	{
		Weapon* wep = players[phase]->getWeapon(i);
		vector<int> advantages;


		cout << endl;
		wep->displayName();
	
		cout << "  ";

		//Weapon Advantage
		if (phase == 1 && players[0]->getWepSlotOption() >= 0)
		{//If going second and being attacked with a weapon
			int advantage = players[phase]->getWeapon(i)->weaponAdvantage(false, players[other]->getCurrentWeapon());
			
			if (advantage != 0) Write::redGreen(advantage);
			advantages.push_back(advantage);
		}
		else if (phase == 0 )
		{//if defending first

			int advantage;

			for (int w = 0; w < players[other]->numEquippedWeapons(); w++)
			{
				if (w > 0) cout << "/";
				advantage = players[phase]->getWeapon(i)->weaponAdvantage(false, players[other]->getWeapon(w));
				if (advantage != 0) Write::redGreen(advantage);
				advantages.push_back(advantage);

			}
		}
		else
		{
			advantages.push_back(500);
		}


		cout << endl;

		//add parry 
		cout << "(" << (i * 2) + 1 << ") Parry    ";
		int type = players[phase]->isTwoHandedWielding();
		cout << players[phase]->getFullAttDefVal(false, type, i, -1, advantages[0]);
		if (advantages.size() == 2) cout << "/" << players[phase]->getFullAttDefVal(false, type, i, -1, advantages[1]);

		if (getAdvanced()) cout << " " << defenseBreakdown(false, type, i, -1, 0);
		
		//parry bonus, if there
		if (wep->getParryModBonus() + players[phase]->getModBonus(1) != 0 && getAdvanced())
		{
			cout << " (";
			Write::redGreen(wep->getParryModBonus() + players[phase]->getModBonus(1));
			cout << "%)";
		}

		cout << endl;

		//add counterattack
		cout << "(" << (i * 2) + 2 << ") Counter  ";

		cout << players[phase]->getFullAttDefVal(false, 4, i, -1, advantages[0]);
		if (advantages.size() == 2) cout << "/" << players[phase]->getFullAttDefVal(false, 4, i, -1, advantages[1]);
		
		if (getAdvanced()) cout << " " << defenseBreakdown(false, 4, i, -1, 0);

		cout << endl;

	}


	vector<int> dodgeAdvantages;
	//dodge advantage
	if (phase == 1 && players[0]->getWepSlotOption() >= 0) //if being attacked with a weapon, and going second
	{
		cout << "  ";

		dodgeAdvantages.push_back(players[0]->getCurrentWeapon()->getDodgeModifier() * -1);

	}
	else if (phase == 0) //if going first
	{
		cout << "  ";
		for (int i = 0; i < players[1]->numEquippedWeapons(); i++) //loop through the other players weapons
		{
			if (i == 1) cout << "/";
			dodgeAdvantages.push_back(players[1]->getWeapon(i)->getDodgeModifier() * -1);

		}
	}
	else
		dodgeAdvantages.push_back(500);

	
	//dodge skill
	cout << endl << "(5) Dodge    " << players[phase]->getFullAttDefVal(false, 3, -1, -1, dodgeAdvantages[0]);
	if (dodgeAdvantages.size() == 2) cout << "/" << players[phase]->getFullAttDefVal(false, 3, -1, -1, dodgeAdvantages[1]);

	if (getAdvanced()) cout << " " << defenseBreakdown(false, 3, -1, -1, 0);

	if (players[phase]->getModBonus(2) != 0)
	{
		cout << " (";
		Write::redGreen(players[phase]->getModBonus(2));
		cout << "%)";
	}
	
	for (int i = 0; i < dodgeAdvantages.size(); i++)
	{
		Write::redGreen(dodgeAdvantages[i]);
		if (i == 0) cout << "/";
	}


}

/*
bool Duel::setAttackWepIndex(char attackerInput) //sets the weapon index and attack index from an entered. returns false if invalid
{
	int attNum = attackerInput - 48; //converts from char to int

	int attackIndex = (attNum - 1) % 4;

	//first determine the weapon used
	if (attNum > 0 && attNum < 5)
		_attackerWeaponIndex = 0;
	else if (attNum > 4 && attNum < 9)
		_attackerWeaponIndex = 1;
	else if (attNum == 0)
	{
		//doing nothing
		_attackerWeaponIndex = -1;
		_attackerAttackIndex = -1;
		return true;
	}
	else
	{
		//invalid key press
		return false;
	}

	//then if there is a valid weapon, find the attack
	if (players[phase]->numEquippedWeapons() >= _attackerWeaponIndex + 1)//if there is a weapon that exists
	{
		bool attackValid = players[phase]->getWeapon(_attackerWeaponIndex)->hasAttackInSlot(attackIndex);

		if (attackValid)
		{
			if (players[phase]->getWeapon(_attackerWeaponIndex)->wepAttack(attackIndex).name == "Feint" && phase == 0)
				return false; //feint invalid if going first
			else
			{
				_attackerAttackIndex = attackIndex;
				return true;
			}
		}
		else
			return false;
	}
	else
		return false; //that weapon doesn't exist, invalid option

}

bool Duel::setDefenseNumber(char defenderInput)
{
	int defNum = defenderInput - 48;

	if (defNum > 0 && defNum < 3)
		_defenderWeaponIndex = 0;
	else if (defNum > 2 && defNum < 5)
		_defenderWeaponIndex = 1;
	else if (defNum == 5)
	{
		//dodge
		_defenderWeaponIndex = -1;
		_defenderDefenseIndex = -1;
		return true;
	}
	else if (defNum == 0)
	{	
		//nothing
		_defenderWeaponIndex = -2;
		_defenderDefenseIndex = -1;
		return true;
	}
	else
	{
		//invalid key press
		return false;
	}


	//then if there is a valid weapon, the defense is just 0 or 1
	if (players[phase]->numEquippedWeapons() >= _defenderWeaponIndex + 1)//if there is a weapon that exists
	{
		_defenderDefenseIndex = (defNum - 1) % 2;
		return true;
	}
	else
		return false; //that weapon doesn't exist, invalid option
	
}
*/

//this takes a character input and determines if it is valid. If so, it fills in the attack type data
bool Duel::setupCharacterOptions(bool attacking, char charInput, Character* attacker, Character* defender)
{
	//char to int
	int numInput = charInput - 48;

	int option = -1;
	int wepSlot = -1;
	int attSlot = -1;

	const int MAX_ATTACKS = 4;
	const int DEF_OPTIONS = 2;

	if (attacking)
	{
		//gets a value (0-3) for the attack type
		int attackIndex = (numInput - 1) % MAX_ATTACKS;

		//if attacker does nothing, the defender can't touch them. But they still do something
		if (numInput == 0)
		{
			attacker->setWeaponSlotOption(-1);
			attacker->setOption(-1);
			attacker->setAttTypeOption(-1);

			return true;
		}
		else if (numInput > 0 && numInput < ((MAX_ATTACKS * 2) + 1)) //attack with weapon
		{
			int wepInd = 0;
			if (numInput > MAX_ATTACKS) wepInd = 1;

			if (attacker->numEquippedWeapons() < (wepInd + 1))
				return false;	//if the weapon doesn't exist, return false

			if (attacker->getWeapon(wepInd)->hasAttackInSlot(attackIndex))	//if that particular wepon has an attack in the selected slot
			{
				attacker->setWeaponSlotOption(wepInd); //set the weapon to be in certain slot

				if (phase == 0 && attacker->getWeapon(wepInd)->wepAttack(attackIndex).name != "Feint") //if going first, and they don't select feint
				{
					attacker->setOption(attacker->isTwoHandedWielding()); //set this option to a on or two handed attack
					attacker->setAttTypeOption(attackIndex);
					return true;
				}
				else if (phase == 1)//if you are going second
				{
					if (attacker->getWeapon(wepInd)->wepAttack(attackIndex).name == "Feint") //if a feint
					{
						//if the attacker is feinting, set the attack option to feint
						attacker->setOption(2);
						attacker->setAttTypeOption(attackIndex);

						//defender will always be doing a feint defense
						defender->setOption(2);
						return true;
					}
					else
					{
						if (defender->getOption() == 4) //if attacking against a counterattack
						{
							attacker->setOption(4);		//Set option to attack against a counter, which is a different attack than if it was a weapon 
							attacker->setAttTypeOption(attackIndex);  //This remains the same, because we'll need the attack type to determine damage. 
							return true;
						}
						else
						{
							attacker->setOption(attacker->isTwoHandedWielding());
							attacker->setAttTypeOption(attackIndex);
							return true;
						}
					}
				}
				else
					return false;
			}
			else
				return false; //if there isn't an attack in that slot

		}
		else
			return false;

	}
	else //if defending
	{
		int defIndex = (numInput - 1) % DEF_OPTIONS; //This will be a 0 or 1 for either a parry or counter with each weapon

		if (numInput == 0) //If defending and doing nothing. Also, this is pretty much always a bad idea in game
		{	//also, the attacker isn't affected by this. They can attack against a defender that isn't doing anything
			defender->setAttTypeOption(-1);
			defender->setOption(-1);
			defender->setWeaponSlotOption(-1);

			return true;
		}
		else if (numInput < ((DEF_OPTIONS * 2) + 1)) //for parries and counters
		{
			int wepInd = 0;
			if (numInput > DEF_OPTIONS) wepInd = 1;

			if (defender->numEquippedWeapons() < (wepInd + 1))
				return false;	//if the weapon doesn't exist, return false

			defender->setWeaponSlotOption(wepInd);

			if (defIndex == 0) //Parry
			{

				defender->setOption(defender->isTwoHandedWielding());
				defender->setAttTypeOption(-1); //no attack type
				return true;
			}
			else //counter
			{
				defender->setOption(4);
				defender->setAttTypeOption(-1); //no attack type

				if (phase == 1) //if going second
				{
					if (attacker->getOption() >= 0) //if the attacker isn't doing nothing (aka attacking)
					{//we need to switch their option to a counter defense
						attacker->setOption(4);
					}
				}
				return true;

			}
			return true;
		}
		else if (numInput == 5) //Dodge
		{
			defender->setWeaponSlotOption(-1);
			defender->setOption(3);
			defender->setAttTypeOption(-1); //no attack type
			return true;
		}
		else
			return false;

	}
}

/*
string Duel::getMove(bool attacker, int playerNum)
{
	if (attacker == true)
	{
		if (_attackerWeaponIndex >= 0)
		{
			players[playerNum]->getWeapon(_attackerWeaponIndex)->displayName();
			cout << " ";
			return players[playerNum]->getWeapon(_attackerWeaponIndex)->wepAttack(_attackerAttackIndex).name;
		}
		else
			return "Nothing";
	}
	else
	{
		if (_defenderWeaponIndex >= 0)
		{
			players[playerNum]->getWeapon(_defenderWeaponIndex)->displayName();

			if (_defenderDefenseIndex == 0)
				return " Parry";
			else
				return " Counter";
		}
		else if (_defenderWeaponIndex == -1)
			return "Dodge";
		else if (_defenderWeaponIndex == -2)
			return "Nothing";
	}
}
*/
bool Duel::changePosture(Character* character, int num)
{
	int currentPost = character->getPosture();

	if (currentPost + num < 2 && currentPost + num > -2)
	{
		if (character->getReact() > 0)
		{
			character->setPosture(currentPost + num);
			character->alterReaction(-1);
			return true;
		}
		

	}
	
	return false;
	
}

void Duel::changeTargetArea(Character* character, Character* other, int num)
{
	character->setAreaTargeted(character->getAreaTargeted() + num);

	if (character->getAreaTargeted() < 0) character->setAreaTargeted(other->getNumberofTargetAreas() - 1);
	if (character->getAreaTargeted() >= other->getNumberofTargetAreas()) character->setAreaTargeted(0);
}

string Duel::attackBreakdown(bool attack, int type, int wepSlot, int attackNum, int advantage, Character::targetArea* target)
{
	string returnString = "";
	Weapon* currentWeapon = players[phase]->getWeapon(wepSlot);

	//added values
	int baseValue = players[phase]->getBaseValue(attack, type);
	int wepSkill = players[phase]->getWepSkillAttack(attack, type, wepSlot);
	int defSkill = players[phase]->getDefSkillAttack(attack, type, wepSlot); //for an attack this is only no zero for dodging a counter
	int attTypeSkill = players[phase]->getAttSkillAttack(attack, type, wepSlot, attackNum);

	returnString += "(" + to_string(baseValue);
	if (wepSkill != 0) returnString += " " + to_string(wepSkill) + "a";
	if (defSkill != 0) returnString += " " + to_string(defSkill) + "d";
	if (attTypeSkill != 0) returnString += " " + to_string(attTypeSkill) + "t";

	returnString += " adv";

	//multiplied values
	int wepBonus = players[phase]->getWepBonus(attack, type, attackNum, currentWeapon); //percentage
	int postureBonus = players[phase]->getPostureBonus(attack, type);
	int targetBonus = players[phase]->getTargetBonus(attack, type, attackNum, currentWeapon, target);

	returnString += ")";
	
	if (wepBonus != 0) returnString += " " + to_string(wepBonus) + "%B";
	if (postureBonus != 0) returnString += " " + to_string(postureBonus) + "%P";
	if (targetBonus != 0) returnString += " " + to_string(targetBonus) + "%T";

	return returnString;

}

string Duel::defenseBreakdown(bool attack, int type, int wepSlot, int attackNum, int advantage)
{
	string returnString = "";
	Weapon* currentWeapon = players[phase]->getWeapon(wepSlot);

	//added values
	int baseValue = players[phase]->getBaseValue(attack, type);
	int wepSkill = players[phase]->getWepSkillAttack(attack, type, wepSlot);
	int defSkill = players[phase]->getDefSkillAttack(attack, type, wepSlot); //for an attack this is only no zero for dodging a counter
	int attTypeSkill = players[phase]->getAttSkillAttack(attack, type, wepSlot, attackNum);

	returnString += "(" + to_string(baseValue);
	if (wepSkill != 0) returnString += " " + to_string(wepSkill) + "a";
	if (defSkill != 0) returnString += " " + to_string(defSkill) + "d";
	if (attTypeSkill != 0) returnString += " " + to_string(attTypeSkill) + "t";

	returnString += " adv";

	//multiplied values
	int wepBonus = players[phase]->getWepBonus(attack, type, attackNum, currentWeapon); //percentage
	int postureBonus = players[phase]->getPostureBonus(attack, type);
	int parryBonus = 0;
	if (type != 3) parryBonus = (currentWeapon->getBalance() - 1) * 100.0;

	returnString += ")";

	if (parryBonus != 0) returnString += " " + to_string(parryBonus) + "%D";
	if (wepBonus != 0) returnString += " " + to_string(wepBonus) + "%B";
	if (postureBonus != 0) returnString += " " + to_string(postureBonus) + "%P";
	

	return returnString;
}