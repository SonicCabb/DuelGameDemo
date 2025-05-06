
#include "Character.h"

using namespace std;
Character::Character()
{

	_strength = 0;
	_agility = 0;
	_toughness = 0;
	_reflex = 0;
	_currentReaction = 0;
	attacking = true;
	_attDefOption = -1; //by default, set to doing nothing
	_attOption = -1;
	_wepSlotOption = -1;

	_currentPosture = 0;
	_defaultPosture = 0;

	_currentAttackRoll = 0;
	_currentDefenseRoll =0;
	_areaTargeted = 0;
	_lastAreaTargeted = -1;

	string _name = "noname";
	int _level = 0;
	int _maxHp = 0;
	int _currentHp = 0;
	_color = 5;
	_altcolor = 5;

	_altcolorNeeded = false;
	

	vector<int> colors;

	for (int i = 1; i < 15; i++)
		if (i != 7 && i != 8) colors.push_back(i);

	int ind;
	int ind2;

	ind = rand() % colors.size();
	ind2 = ind;
	while (ind == ind2) ind2 = rand() % colors.size();

	_color = colors[ind];
	_altcolor = colors[ind2];


	Weapon wp;

	equipWeapon(wp, 0, true, false);
	equipWeapon(wp, 1, true, false);

	for (int i = 0; i < numWepSkills; i++)
	{
		wepSkills[i] = 0;
	}
	for (int i = 0; i < numDefSkills; i++)
	{
		defSkills[i] = 0;
	}
	for (int i = 0; i < numAttSkills; i++)
	{
		attSkills[i] = 0;
	}

	setupTargetAreas();

	setAI(false);
}


Character::~Character()
{
}

void Character::setupTargetAreas()
{
	targetArea temp;

	//Torso
	temp.name = "Torso";
	for (int i = 0; i < 3; i++)
	{
		temp.damageMultiplier[i] = 1.0;
		temp.attackMultiplier[i] = 1.0;
	}
	temp.reactMultiplier = 1.0;
	temp.attackPenalty = 0;
	temp.defPenalty = 0;
	temp.canHaveArmor = true;
	temp.movementPenalty = 0;
	_targetAreas.push_back(temp);


	//Head
	temp.name = "Head";
	for (int i = 0; i < 3; i++)
	{
		temp.damageMultiplier[i] = 2.0;
	}

	temp.attackMultiplier[0] = 1.0;
	temp.attackMultiplier[1] = 0.2;
	temp.attackMultiplier[2] = 0.5;

	temp.reactMultiplier = 2.0;
	temp.attackPenalty = 0;
	temp.defPenalty = 0;
	temp.canHaveArmor = true;
	temp.movementPenalty = 0;
	_targetAreas.push_back(temp);

	//Arms
	temp.name = "Arms";

	temp.damageMultiplier[0] = 0.7;
	temp.damageMultiplier[1] = 0.5;
	temp.damageMultiplier[2] = 1.0;

	temp.attackMultiplier[0] = 1.5;
	temp.attackMultiplier[1] = 0.5;
	temp.attackMultiplier[2] = 1.0;

	temp.reactMultiplier = 1.0;
	temp.attackPenalty = -50;
	temp.defPenalty = -50;
	temp.canHaveArmor = true;
	temp.movementPenalty = 0;
	_targetAreas.push_back(temp);

	//Legs
	temp.name = "Legs";
	temp.damageMultiplier[0] = 1.0;
	temp.damageMultiplier[1] = 0.5;
	temp.damageMultiplier[2] = 0.7;

	temp.attackMultiplier[0] = 0.9;
	temp.attackMultiplier[1] = 0.9;
	temp.attackMultiplier[2] = 0.9;

	temp.reactMultiplier = 1.0;
	temp.attackPenalty = 0;
	temp.defPenalty = 0;
	temp.canHaveArmor = true;
	temp.movementPenalty = -50;
	_targetAreas.push_back(temp);

}

float Character::getSkill(int skillType, int skillIndex)
{
	switch (skillType)
	{
	case 0:
		if (skillIndex < arraySize(wepSkills)) return wepSkills[skillIndex];
		else return -1;
		break;
	case 1:
		if (skillIndex < arraySize(defSkills)) return defSkills[skillIndex];
		else return -1;
		break;
	case 2:
		if (skillIndex < arraySize(attSkills)) return attSkills[skillIndex];
		else return -1;
		break;
	default:
		return -1;
		break;
	}
}



void Character::alterHP(int amount)
{
	_currentHp += amount;

	if (_currentHp < 0) _currentHp = 0;

	if (_currentHp > _maxHp) _currentHp = _maxHp;
}

int Character::alterReaction(int value)
{
	if (getReact() + value > getMaxReaction())
	{
		int val = getMaxReaction() - _currentReaction;
		_currentReaction = getMaxReaction();
		return val;
	}
	else if (getReact() + value < getMinReaction())
	{
		int val = getMinReaction() - _currentReaction;
		_currentReaction = getMinReaction();
		return val;
	}
	else 
	{
		_currentReaction += value;
		return value;
	}
}

//this determines what the skill is based on the skill number
string Character::skillName(int type, int skll)
{

	switch (type)
	{
	case 0:
		switch (skll)
		{
		case 0:
			return "Sword";
			break;
		case 1:
			return "Axe";
			break;
		case 2:
			return "Polearm";
			break;
		case 3:
			return "Club";
			break;
		case 4:
			return "Knife";
			break;
		case 5:
			return "Unarmed";
			break;
		default:
			return "Undefined Weapon Skill";
			break;
		}

		break;
	case 1:
		switch (skll)
		{
		case 0:
			return "Shield";
			break;
		case 1:
			return "Parry";
			break;
		case 2:
			return "Dodge";
			break;
		default:
			return "Undefined Defense Skill";
			break;
		}
		break;
	case 2:
		switch (skll)
		{
		case 0:
			return "Slashing";
			break;
		case 1:
			return "Piercing";
			break;
		case 2:
			return "Crushing";
			break;
		case 3:
			return "Feinting";
			break;
		default:
			return "Undefined Attack Skill";
			break;
		}
		break;
	default:
		return "Undefined Category";
		break;
	}
}



void Character::skillUp(int type, int index, int times)
{
	switch (type)
	{
	case 0:
		for (int i = 0; i < times; i++) wepSkills[index] += (1 / ((wepSkills[index] + 1) * skillMultiplier));
		break;
	case 1:
		for (int i = 0; i < times; i++) defSkills[index] += (1 / ((defSkills[index] + 1) * skillMultiplier));
		break;
	case 2:
		for (int i = 0; i < times; i++) attSkills[index] += (1 / ((attSkills[index] + 1) * skillMultiplier));
		break;
	default:
		break;
	}
}

int Character::classToWepSkill(string clss)
{
	if (clss == "Sword") return 0;
	else if (clss == "Axe") return 1;
	else if (clss == "Polearm") return 2;
	else if (clss == "Club") return 3;
	else if (clss == "Knife") return 4;
	else if (clss == "Unarmed") return 5;
	else if (clss == "Shield") return -10;
	else if (clss == "Dodge") return -15;
	else if (clss == "Still") return -20;
	else return -1;
}



//generates this current character
void Character::generateRandom(int power, int gender)
{
	//number of attributes
	const int numStats = 4;

	//modifiers
	int skillNum = 100;		//number of skills per level to add. Exponential
	int skillbuyin = power * ((power + 2) / 3) * skillNum;

	int focus = skillNum * .5; //modifier to determine how focused random characters will be. Higher values give more focus

	//flip is a modifier to show the likelyhood of each character focusing on one particular weapon skill
	int flip;
	//the higher the flip modifier the more likely the character is to focus on certain skills
	int flipMod = (rand() % ((focus * (power + 1)) / 3)) + 1;
	
	//the random skill/stat index used throughout
	int randIndex;

	//get random name
	setName(randomName(gender));


	//then determine stats (These aren't weighted at the moment)
	int buyin = power + 4;
	for (int i = 0; i < buyin; i++)
	{
		float modi[4] = { getStrength(), getAgility(), getToughness(), getReflex() };
		if (i % 2 == 0)randIndex = rand() % numStats;
		else randIndex = weightedIndex(modi, 4);

		switch (randIndex)
		{
		case 0:
			setStrength(getStrength() + 1);
			break;

		case 1:
			setAgility(getAgility() + 1);
			break;

		case 2:
			setToughness(getToughness() + 1);
				break;
		case 3:
			setReflex(getReflex() + 1);
			break;

		}

	}

	//weapon skills (Helps determine if they can be wielded)
	for (int i = 0; i < skillbuyin; i++)	//add skills for all buyin points
	{
		//determine flip modifier, if 0 skill is random, if not it isn't
		flip = rand() % flipMod;

		if (flip == 0 || i == 0)	//if the flip is 0 or this is the first time, the skill to change is random
		{
			randIndex = rand() % (numWepSkills - (rand() % 2));	//this extra bit is to temporarily make fists less likely
			skillUp(0, randIndex, 1);
		}
		else
			//if not, the skill to change is still random, just weighted based on the skills already chosen.
			skillUp(0, weightedIndex(wepSkills, arraySize(wepSkills)), 1);

	}

	//generate righthand weapon
	generateWeapon(true);

	float seed[3] = { 1, 1, 1 };
	if (_equippedWeapons[0].getTwoHanded())//if two handed weapon equipped, make it more unlikely to get shield skill
		seed[0] = 0;

	//get defensive skills
	for (int i = 0; i < skillbuyin; i++)
	{
		int randIndex = rand() % numDefSkills;
		flip = rand() % flipMod;

		int skillChoice;
		//if the first time, seed with the special seed
		if (flip == 0 && i != 0)
			skillUp(1, randIndex, 1); //if flip = 0, increase a random skill
		else if (i == 0)
			skillUp(1, weightedIndex(seed, arraySize(seed)), 3);	//first time through, always seed based on the two handed weapon thing
		else
			skillUp(1, weightedIndex(defSkills, arraySize(defSkills)), 1);				//other times, add skills based on what has already been picked

	}

	//generate lefthand weapon
	if (_equippedWeapons[0].getTwoHanded() == false)
		generateWeapon(false);


	//attack skills based on the weapons chosen
	for (int i = 0; i < skillbuyin; i++)
	{
		int randIndex = rand() % numAttSkills;
		flip = rand() % flipMod;

		int skillChoice;

		if (flip == 0 || i == 0)
			skillUp(2, randIndex, 1);
		else
		{
			//determine attack stats based on wielded weapons
			float wepAttacks[numAttSkills];

			

			wepAttacks[0] = getWeapon(0)->getSlashing();
			wepAttacks[1] = getWeapon(0)->getPiercing(); 
			wepAttacks[2] = getWeapon(0)->getCrushing();

			if (getWeapon(1) != nullptr)
			{
				wepAttacks[0] += (getWeapon(1)->getSlashing() / 3.0);
				wepAttacks[1] += (getWeapon(1)->getPiercing() / 3.0);
				wepAttacks[2] += (getWeapon(1)->getCrushing() / 3.0);
			}


			wepAttacks[3] = _agility / 3.5;

			int num = weightedIndex(wepAttacks, arraySize(wepAttacks)); 

			skillUp(2, num, 1);
		}
			

	}

	//then other stats

	setDefPosture((rand() % 3) - 1);
	setPosture(getDefPosture());

	setMaxHp(10);

	setMaxHp(getmaxHp() + (3 * _toughness));
	for (int i = 0; i < power; i++)
	{
		setMaxHp(getmaxHp() + (rand() % 5) + (3 * _toughness));
	}

	setHp(getmaxHp());

	setLevel(power);

}

//handy function that returns a random index based on the weighted float vector
int Character::weightedIndex(float weights[], int length)
{
	int weightSum = 0;

	int randomIndex = 0;

	//determine weightsum
	for (int i = 0; i < length; i++)
		weightSum += (weights[i] * 100);

	//if all the weights are 0, just give a random index
	if (weightSum == 0) return (rand() % length);
	else
	{
		//determine random index based on the weight sum
		randomIndex = rand() % weightSum;

		//loop through each element, seeing if the random number is found in each
		for (int i = 0; i < length; i++)
		{
			if (randomIndex < (weights[i] * 100)) return i;	//if it is, return it
			//if not, decreas the random index by the weight
			randomIndex -= (weights[i] * 100);
		}

	}

	return 0;

}
int Character::weightedIndex(float weights[], int length, float extra)
{
	int weightSum = 0;

	int randomIndex = 0;

	//determine weightsum
	for (int i = 0; i < length; i++) weightSum += (weights[i] * (100 + (weights[i] * weights[i] * extra)));

	//if all the weights are 0, just give a random index
	if (weightSum == 0) return (rand() % length);
	else
	{
		//determine random index based on the weight sum
		randomIndex = rand() % weightSum;

		//loop through each element, seeing if the random number is found in each
		for (int i = 0; i < length; i++)
		{
			if (randomIndex < (weights[i] * (100 + (weights[i]  * weights[i] * extra)))) return i;	//if it is, return it
			//if not, decreas the random index by the weight
			randomIndex -= (weights[i] * (100 + (weights[i] * weights[i] * extra)));
		}

	}

	return 0;

}

int Character::weightedIndexInts(int weights[], int length)
{
	int weightSum = 0;

	int randomIndex = 0;

	//determine weightsum
	for (int i = 0; i < length; i++)
		weightSum += (weights[i]);

	//if all the weights are 0, just give a random index
	if (weightSum == 0) return (rand() % length);
	else
	{
		//determine random index based on the weight sum
		randomIndex = rand() % weightSum;

		//loop through each element, seeing if the random number is found in each
		for (int i = 0; i < length; i++)
		{
			if (randomIndex < (weights[i])) return i;	//if it is, return it
			//if not, decreas the random index by the weight
			randomIndex -= (weights[i]);
		}

	}

	return 0;
}

//loads in a random name from a file
string Character::randomName(int gender)
{
	fstream fin;
	string nameTemp;
	vector<string> firstNames;
	vector<string> lastNames;

	//get first names
	if (gender == 0)
	{
		fin.open("FirstNames.txt");
		if (fin.fail()) perror("FirstNames.txt");
		while (getline(fin, nameTemp)) firstNames.push_back(nameTemp);

		fin.close();
	}
	else
	{
		fin.open("FemNames.txt");
		if (fin.fail()) perror("FemNames.txt");
		while (getline(fin, nameTemp)) firstNames.push_back(nameTemp);

		fin.close();
	}

	//get last names

	fin.open("LastNames.txt");
	if (fin.fail()) perror("LastNames.txt");
	while (getline(fin, nameTemp)) lastNames.push_back(nameTemp);

	fin.close();

	//get random name
	return firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];

}

//character chooser function...currentlyt returns whatever character was selected
Character Character::characterChooser(int level, string title)
{
	Write w;
	const int numChoices = 5;
	char input;
	Character champions[numChoices];
	int index = 0;

	vector<int> colors;
	vector<int> altColors;

	for (int i = 1; i < 15; i++)
	{
		if (i != 7 && i != 8)
		{
			colors.push_back(i);
			altColors.push_back(i);
		}
	}
	int ind;
	int ind2;

	for (int i = 0; i < numChoices; i++)
	{
		champions[i].generateRandom(level, rand() % 2);
		ind = rand() % colors.size();
		ind2 = ind;
		while (ind2 == ind) ind2 = rand() % altColors.size();
		champions[i]._color = colors[ind];
		champions[i]._altcolor = altColors[ind2];

		//get rid of that color option
		colors[ind] = colors.back();
		colors.pop_back();
	}

	bool running = true;

	while (running)
	{
		system("CLS");

		w.wc(title, 0 , champions[index]._color);
		cout << " Choose your hero. (z to save character)" << endl;

		for (int i = 0; i < numChoices; i++)
		{
			if (index != i)
			{
				champions[i].printName(false, 0);
				cout << endl;
				
			}
			else
			{
				w.wc(champions[i].getName(), champions[i]._color, 0);
				cout << endl;
			}
		}
		cout << "-----------------------------------------------------------------------"<< endl;
		champions[index].display();

		input = _getch();
		//determine action
		switch (input)
		{

		case 'w':
			if (index > 0) index--;
			break;

		case 's':
			if (index < numChoices - 1) index++;
			break;

		case 'e':
			return champions[index];
			break;

		case 'z':
			champions[index].saveCharacter();
			break;
		case 'q':
		{
			Character temp;
			temp.setName("choosing--");
			return temp;
		}	break;

		}
	}



	system("PAUSE");
}

Character Character::savedCharacters() //displays saved characters
{
	char input;
	int index = 0;
	vector<Character> characters;

	ifstream fin;
	string line;
	fin.open(INDEX_FILE); //should probably add error checking
	
	while (!fin.eof()) { //load all the names of saved characters
		getline(fin, line);
		if (line != "")
		{
			Character temp;
			temp.loadCharacter(line);
			characters.push_back(temp);
		}
	}
	fin.close();


	bool running = true;

	while (running)
	{
		system("CLS");

		Write::wc("Saved Characters: (x to delete) (c to view character)", 0, 7);
		cout << endl;

		for (int i = 0; i < characters.size(); i++)
		{
			if (index != i)
			{
				Write::wc(characters[i].getName() + " - Level " + to_string(characters[i].getLevel()), 0, characters[i].getColor());
				cout << endl;

			}
			else
			{
				Write::wc(characters[i].getName() + " - Level " + to_string(characters[i].getLevel()), characters[i].getColor(), 0);
				cout << endl;
			}
		}


		input = _getch();
		//determine action
		switch (input)
		{

		case 'w':
			if (index > 0) index--;
			break;

		case 's':
			if (index < characters.size() - 1) index++;
			break;

		case 'e':
			return characters[index];
			break;
		case 'x':
		{
			if (deleteCharacter(characters[index].getName()) == 0)
			{
				
				characters[index] = characters.back();
				characters.pop_back();
				if (index > 0) index--;
			}
		}	break;
		
		case 'c':
			system("CLS");
			characters[index].display();
			_getch();
			break;

		case 'q':
		{
			Character temp;
			temp.setName("choosing--");
			return temp;
		}	break;

		}
	}



	system("PAUSE");
}
//displays character profile with weapons
void Character::display()
{
	stringstream ss;
	
	ss.precision(0);
	ios:ss.setf(ios::fixed, ios::floatfield);

	//this holds color informaotion for each stat. Highest stats are a differnt color
	int wepColors[numWepSkills];
	int defColors[numDefSkills];
	int attColors[numAttSkills];

	for (int i = 0; i < numWepSkills; i++)
		wepColors[i] = 7;
	for (int i = 0; i < numDefSkills; i++)
		defColors[i] = 7;
	for (int i = 0; i < numAttSkills; i++)
		attColors[i] = 7;


	int wepMaxInd = 0;
	int defMaxInd = 0;
	int attMaxInd = 0;


	//determine the highest stat for each category
	for (int i = 0; i < numWepSkills; i++)
	{	
		if (getWepSkill(i) >= getWepSkill(wepMaxInd))
			wepMaxInd = i;
	}
	for (int i = 0; i < numDefSkills; i++)
	{
		if (getDefSkill(i) >= getDefSkill(defMaxInd))
			defMaxInd = i;
	}
	for (int i = 0; i < numAttSkills; i++)
	{
		if (getAttSkill(i) >= getAttSkill(attMaxInd))
			attMaxInd = i;
	}
	//set the colors for each stat
	wepColors[wepMaxInd] = getColor();
	defColors[defMaxInd] = getColor();
	attColors[attMaxInd] = getColor();


	//cout << setw(24) << left;

	cout << "Lvl " << getLevel() << " | ";

	//display title
	Write::wc(getName(), 0, getColor());
	
	cout << " | " << getmaxHp() << " HP";

	//display posture

	cout << " | Posture: " << getPostureString();

	//display reaction min/max/starting

	cout << " | Reaction: " << getMaxReaction() << "/" << getStartingReact() << "/" << getMinReaction();

	//display basic stats
	cout << endl << endl;
	Write::wc("Ability Indicators:", 0, getColor());

	cout << endl << "Strength " << getStrength() << "   Agility " << getAgility() << "   Toughness " << getToughness() << "   Reflex " << getReflex() << endl << endl;

	Write::wc("Acquired Skills: ", 0, getColor());
	cout << endl;

	//display skills

	//go through each of the weapon skills, checking for other skills after
	for (int i = 0; i < numWepSkills; i++)
	{
		//three categories of skills
		for (int j = 0; j < 3; j++)
		{
			float sk = getSkill(j, i);

			if (sk >= 0)
			{
				cout << setw(10) << left << skillName(j, i) << setw(6) << left;

				int color;

				if (j == 0) color = wepColors[i];
				else if (j == 1) color = defColors[i];
				else color = attColors[i];
				//get the nicely rounded skill info
				ss << sk * 100;
				Write::wc(ss.str(), 0, color);

				//clear the string stream
				ss.str("");
				ss.clear();
			}
			else
			{
				cout << setw(10) << left << " " << setw(6) << left << " ";
			}

			if (j == 2) cout << endl;
		}

	}


	//display weapons
	cout << endl;
	Write::wc("Equipped Weapons:", 0, getColor());
	cout << endl;
	for (int i = 0; i < _equippedWeapons.size(); i++)
	{
		_equippedWeapons[i].display();
		cout << endl;
	}
}

void Character::printName(bool pad, int widthPadding)
{
	if (pad)
		cout << setw(widthPadding);

	Write::wc(getName(), 0, getColor());
}

float Character::getWepSkill(int skillIndex)
{
	if (skillIndex >= 0)
		return wepSkills[skillIndex];
	else if (skillIndex == -10)
		return defSkills[0];
	else if (skillIndex == -15)
		return defSkills[2];
	else
		return 10000;
}

string Character::getPostureString()
{
	if (_currentPosture == -1) return "Defensive";
	else if (_currentPosture == 1) return "Aggressive";
	else return "Neutral";
}

void Character::changePosture()
{
	int currentCost = 0;

	int menuPlace = _currentPosture + 2;

	string title = "Change Posture. Current Reaction: " + to_string(getReact());
	bool finished = false;

	int costs[3];

	costs[0] = _currentPosture + 1;
	costs[1] = abs(_currentPosture);
	costs[2] = abs(_currentPosture - 1);

	string line1 = "Defensive (" + to_string(costs[0]) + " Reaction)";
	string line2 = "Neutral (" + to_string(costs[1]) + " Reaction)";
	string line3 = "Aggressive (" + to_string(costs[2]) + " Reaction)";
	
	string disc1 = "Increases defense by 50%. Reduces defensive penalties. Reduces attacks, and attack bonuses.";
	string disc2 = "This posture gives no bonuses or penalties to anything.";
	string disc3 = "Increases attack by 50%. Reduces attack penalties. Reduces defense, and defensive bonuses.";
	
	
	
	string menuLines[] = { line1, line2, line3 };
	string descripLines[] = { disc1, disc2, disc3 };


	while (finished == false)
	{

		int selection = Write::menu(title, menuLines, descripLines, 3, menuPlace, "Blue");

		menuPlace = selection;

		if (selection == -1)//if exit the menu without choosing, exit posture menu
		{
			finished = true;
		}

		selection -= 1;

		if ((getReact() - costs[selection] >= 0) && !finished)
		{//if valid
			alterReaction(costs[selection] * -1);

			_currentPosture = selection - 1;

			finished = true;
		}


	}


}

int Character::getMaxReaction()
{
	return 3 + getLevel() + (getReflex() * 2);
}

int Character::getMinReaction()
{
	int val = -10 + getToughness() + getLevel();

	if (val < 0) return val;
	else return 0;
}

int Character::getStartingReact()
{
	return (getReflex()) + getLevel();
}

void Character::setReaction(int value)
{
	if (value > getMaxReaction()) _currentReaction = getMaxReaction();
	else if (value < getMinReaction()) _currentReaction = getMinReaction();
	else _currentReaction = value;
}

void Character::changeReaction(int mode, int extra)
{
	if (mode == 0) //this mode just alters it by the extra
	{
		int num = alterReaction(extra);
		if (num > 0)
		{
			printName(false, 0);
			cout << " gains " << num << " reaction." << endl;
		}
		else if (num < 0)
		{
			printName(false, 0);
			cout << " loses " << num << " reaction." << endl;
		}
	}
	else if (mode == 1) //mental increase mode increases their reaction by the standard amount, multiplied by extra
	{
		int num = alterReaction((getReflex() + 1) * extra);

		if (num != 0)
		{
			printName(false, 0);
			cout << " gains " << num << " reaction." << endl;
		}
	}
	else if (mode == -1) //decrease based on toughness. This is when they take damage. Extra is the damage taken
	{
		int num = alterReaction(((extra / (getToughness() + 4)) + 1) * -1);
		if (num != 0)
		{
			printName(false, 0);
			cout << " loses " << num * -1 << " reaction." << endl;
		}
	}
	else if (mode == -2) //decrease based on reflex (mental toughness basically). Extra is value lost by level 0 reflex person
	{
		int num = alterReaction(((extra / (getReflex() + 1)) + 1) * -1);
		
		if (num != 0) 
		{
			printName(false, 0);
			cout << " loses " << num * -1 << " reaction." << endl;
		}
	}

}

void Character::generateWeapon(bool righthand) 
{
	Weapon generatedWeapon;
	int wieldType;
	int weaponType;
	int skill;
	if (righthand == true)
	{
		//need to determine if wielding a one or two handed weapon
		float modi[2] = { getStrength() + 5, getAgility() + 5};			//weighted string. Higher agility, more likely to have two handed weapon, and more strength, more likely to have onehanded

		wieldType = weightedIndex(modi, arraySize(modi));
		//0 is one handed, 1 is two handed

		weaponType = weightedIndex(wepSkills, arraySize(wepSkills), 100);

		skill = wepSkills[weaponType];
		if (wieldType == 0) skill += getStrength();
		else skill += getAgility();

		if (generatedWeapon.generateWeaponToSkill(weaponType, wieldType, skill, 0) != true)	//try to generate a weapon via the wield type. If that fails, do the other wield typee
		{
			if (wieldType == 0) wieldType = 1;
			else wieldType = 0;
			generatedWeapon.generateWeaponToSkill(weaponType, wieldType, skill, 0);
		}

		//equip the weapon
		equipWeapon(generatedWeapon, 0, false, wieldType);
	}
	else
	{
		wieldType = 0;

		//to determine if shield or weapon
		float modi[2] = { defSkills[0], defSkills[1] };

		int isShield = weightedIndex(modi, arraySize(modi));

		if (isShield == 0)
		{
			weaponType = 6;
			skill = defSkills[0] + getStrength();
		}
		else
		{	//if not a shield
			weaponType = weightedIndex(wepSkills, arraySize(wepSkills), 100);
			skill = wepSkills[weaponType] + getStrength();
		}

		generatedWeapon.generateWeaponToSkill(weaponType, 0, skill, 0);
		
		//equip the weapon
		equipWeapon(generatedWeapon, 1, false, false);
	}
}

bool Character::equipWeapon(Weapon wep, int slot, bool overwrite, bool twohanded)
{
	int size = _equippedWeapons.size();

	if (size == slot)
	{
		if (slot == 1 && twohanded == true) //can't put two handed weapons in the second slot
			return false;

		if (slot == 1 && _equippedWeapons[0].getTwoHanded() == true) //calso can't equip if there is a two handed weapon in the first slot
			return false;

		//if no weapons equipped, add one.
		_equippedWeapons.push_back(wep);
		return true;
	}
	else if (slot < size) //if the slot exists
	{

		if (slot == 0)
		{
			if (overwrite == true || _equippedWeapons[slot].getName() == "Fist") //if permission is given to overwrite or weapon is a fist
			{
				_equippedWeapons[slot] = wep;
				if (twohanded)
					removeWeapon(1);//if a two handed weapon, remove the weapon in slot 2
				return true; //we were successful
			}
		}
		else
		{//only one handed in this slot
			if (twohanded == false)
				if (overwrite == true || _equippedWeapons[slot].getName() == "Fist")
				{
					//we shouldn't be able to get to this point if there is a two handed sword in the first slot

					_equippedWeapons[slot] = wep;
					return true;
				}
		}
		

	}
	else //the desired slot is the second one, and their isn't anything equipped yet. Can't do that yet
	{

	}

	return false;
}

bool Character::removeWeapon(int slot)
{
	int size = _equippedWeapons.size();

	if (slot < size) //if that slot exists
	{
		_equippedWeapons[slot] = _equippedWeapons.back();

		_equippedWeapons.pop_back();
		
		return true;
	}
	return false;
}

bool Character::addFist(int slot)
{
	int size = _equippedWeapons.size();

	if (slot < size)
	{
		Weapon fist;
		_equippedWeapons[slot] = fist;

		return true;
	}
	return false;
}


Weapon* Character::getWeapon(int slot)
{ 
	//if the weapon doesn't exist, nullptr
	if (slot >= _equippedWeapons.size()) return nullptr;
	return &_equippedWeapons[slot];
}

bool Character::isTwoHandedWielding()
{
	if (numEquippedWeapons() == 1)
	{
		if (getWeapon(0)->getTwoHanded())
			return true;
	}
	return false;
}

int Character::aiChoice(Character* other, int phase)
{

	//First, determine if posture should be changed

	const int CHANCE_TO_KEEP_POSTURE = 50; //higher this number, higher chance to not change posture
	bool desireToChangePosture = false;
	bool desireToGainReaction = false;

	//the starting desire to change posture is random
	if (rand() % (100 + abs(getPosture() * 50)) > CHANCE_TO_KEEP_POSTURE) //randomly decide if there is a desire to change posture. Higher chance if poosture is extreme
		desireToChangePosture = true;

	//if reaction is lower than the enemies minus the reflex difference (potential gain of reaction)
	if (getReact() <= other->getReact() - (getReflex() - other->getReflex()))
	{
		desireToChangePosture = false; //we don't want to go that much lower than our enemy
		desireToGainReaction = true; //if we are lower than our enemies reaction level, we want to gain some back
	}

	if (getAttacking() && getPosture() == 1 || !getAttacking() && getPosture() == -1) desireToChangePosture = false; //if aleady the posture that would be the most helpful, we don't need to change

	if (getReact() > 0 && desireToChangePosture) //if possible to change it, and you want to
	{
		int desiredPosture = 0;
		if (getAttacking())
			desiredPosture = 1;
		else
			desiredPosture = -1;

		if (abs(getPosture() - desiredPosture) >= getReact()) //if possible to spend reaction to this posture change
		{
			//change posture
			setPosture(desiredPosture);
			alterReaction(abs(getPosture() - desiredPosture) * -1);
		}
		else //if not possible, can still set posture as neutral
		{
			setPosture(0);
			alterReaction(-1);
		}

		desireToChangePosture = false;
	}

	if (getReact() <= 0) desireToGainReaction = true; //this will be used later


	//now we can form our weighted array to determine what the ai does.
	const int NUM_OF_OPTIONS = 10;
	const int MAX_WEP_ATTACKS = 4;
	//option number is the keyboard input
	vector<int> optionWeights;	//each element of the vector is an options array for a specific target

	int highestWeight = 0;
	int sumOfWeights = 0;



	//add weights. Attacking values are much different.
	if (getAttacking())
	{

		//Initialize data
		for (int i = 0; i < other->getNumberofTargetAreas(); i++) 
		{

			for (int j = 0; j < NUM_OF_OPTIONS; j++)
			{
				optionWeights.push_back(0);
			}
		}

		//loop through each target area
		for (int i = 0; i < other->getNumberofTargetAreas(); i++)
		{
			//loop through weapons
			for (int j = 0; j < numEquippedWeapons(); j++)
			{
				//advantage calculation
				int advantage = 0;
				if (phase == 1)
				{
					//if against a dodge, advantage is different
					if (other->getOption() != 3)
						advantage = getWeapon(j)->weaponAdvantage(true, other->getCurrentWeapon());
					else
						advantage = getWeapon(j)->getDodgeModifier();
				}
				else
				{//if we don't know the defending weapon, we will just determine the lowest advantage
					advantage = getWeapon(j)->weaponAdvantage(true, other->getWeapon(0));

					for (int wep = 0; wep < other->numEquippedWeapons(); wep++)
					{
						if (getWeapon(j)->weaponAdvantage(true, other->getWeapon(wep)) < advantage)
							advantage = getWeapon(j)->weaponAdvantage(true, other->getWeapon(wep));
					}
				}


				//loop through each attack
				for (int k = 0; k < getWeapon(j)->numberAttacks(); k++)
				{
					int slotNum = k + (j * MAX_WEP_ATTACKS) + 1;
					int optionNum = slotNum + (i * NUM_OF_OPTIONS);

					//the weight should be the attack value of the attack times the damage of the attack
					optionWeights[optionNum] = getFullAttDefVal(true, isTwoHandedWielding(), j, k, advantage, other->getTargetArea(i)) * (getWeapon(j)->getDamage(true, k, other->getTargetArea(i)->damageMultiplier + 1));

					if (getWeapon(j)->wepAttack(k).name == "Feint" && phase == 0) optionWeights[slotNum + (i * NUM_OF_OPTIONS)] = 0; //can't feint in phase 0
					else if (getWeapon(j)->wepAttack(k).name == "Feint" && phase == 1)
					{//if the feint option, multiply by itself...because doesn't have damage
						optionWeights[optionNum] *= optionWeights[optionNum];

						if (phase == 1 && (other->getOption() == 0 || other->getOption() == 1) && other->getCurrentWeapon()->getDef() > 0)
						{//if in phase 1, and enemy parrying with a weapon that has a defensive bonus
							optionWeights[optionNum] *= 4;
						}

						//higher chance of feinting, if you want to gain reaction 
						if (desireToGainReaction)
							optionWeights[optionNum] *= 4;
					}

					if (optionWeights[optionNum] > highestWeight)
						highestWeight = optionWeights[optionNum];

					sumOfWeights += optionWeights[optionNum];

				}

			}

			//determine chance of doing nothing
			if (desireToGainReaction)
			{
				//if going first, or if the enemy isn't doing nothing.
				if (phase == 0 || (phase == 1 && other->getOption() != -1))
				{
					optionWeights[0 + (i * NUM_OF_OPTIONS)] = sumOfWeights;
					sumOfWeights += optionWeights[0 + (i * NUM_OF_OPTIONS)];
					highestWeight = optionWeights[0 + (i * NUM_OF_OPTIONS)];
				}
			}
			else
			{
				optionWeights[0 + (i * NUM_OF_OPTIONS)] = 0;
			}
		}
	}
	else //if defending
	{

		for (int i = 0; i < NUM_OF_OPTIONS; i++)
		{
			optionWeights.push_back(0);
		}

		for (int i = 0; i < numEquippedWeapons(); i++)
		{
			//advantage calculation
			int advantage = 0;
			if (phase == 1) advantage = getWeapon(i)->weaponAdvantage(false, other->getCurrentWeapon());
			else
			{//if we don't know the defending weapon, we will just determine the lowest advantage
				advantage = getWeapon(i)->weaponAdvantage(false, other->getWeapon(0));

				for (int wep = 0; wep < other->numEquippedWeapons(); wep++)
				{
					if (getWeapon(i)->weaponAdvantage(false, other->getWeapon(wep)) < advantage)
						advantage = getWeapon(i)->weaponAdvantage(false, other->getWeapon(wep));
				}
			}

			//parry
			optionWeights[(i * 2) + 1] = getFullAttDefVal(false, isTwoHandedWielding(), i, -1, advantage);

			if (optionWeights[(i * 2) + 1] > highestWeight)
				highestWeight = optionWeights[(i * 2) + 1];

			sumOfWeights += optionWeights[(i * 2) + 1];


			//counter
			optionWeights[(i * 2) + 2] = getFullAttDefVal(false, 4, i, -1, advantage);

			if (optionWeights[(i * 2) + 2] > highestWeight)
				highestWeight = optionWeights[(i * 2) + 2];

			sumOfWeights += optionWeights[(i * 2) + 2];

		}

		//dodge
		int dodgeAdvantage = 0;
		if (phase == 1) dodgeAdvantage = other->getCurrentWeapon()->getDodgeModifier() * -1;
		else
		{//if we don't know the defending weapon, we will just determine the lowest advantage
			dodgeAdvantage = other->getWeapon(0)->getDodgeModifier() * -1;

			for (int wep = 0; wep < other->numEquippedWeapons(); wep++)
			{
				if (other->getWeapon(wep)->getDodgeModifier() < dodgeAdvantage)
					dodgeAdvantage = other->getWeapon(wep)->getDodgeModifier();
			}
		}

		optionWeights[5] = getFullAttDefVal(false, 3, -1, -1, dodgeAdvantage);

		if (optionWeights[5] > highestWeight)
			highestWeight = optionWeights[5];

		sumOfWeights += optionWeights[5];

		//if we know the attacker is doing nothing, also do nothing
		if (phase == 1)
		{
			if (other->getOption() != -1)
			{
				optionWeights[0] = 0;
			}
			else
			{
				for (int i = 0; i < NUM_OF_OPTIONS; i++)
					optionWeights[i] = 0;

				optionWeights[0] = 100;
			}
		}

	}

	//Now we can determine the decision


	//the higher the difficulty, the less random, and the more chance the ai will pick the "best" decision
	int difficulty = getLevel() + 1;
	
	for (int i = 0; i < optionWeights.size() ; i++)
		if (optionWeights[i] < highestWeight)
			optionWeights[i] /= difficulty * difficulty;
		

	//use the weighted index function from the weapon class...it has vectors
	int choice = Weapon::weightedIndex(optionWeights);

	int finalChoice = choice % NUM_OF_OPTIONS;

	int target = (choice - finalChoice) / NUM_OF_OPTIONS;

	setAreaTargeted(target);

	return finalChoice;

}

int Character::aiDecision(Character * opposer, int phase, string opposingDecision, int opposserWepIndex)
{
	int randomness = 5;
	float weight = 1.3;
	int damageMult = 10;
	int wepskillMult = 100;
	int attskillMult = 100;
	int defSkillMult = 100;
	float modSkillMult = 1.1;
	int options[10]; //each slot corresponds to the key presses on the keyboard

	if (opposserWepIndex < 0) opposserWepIndex = 0;

	//initialize options array

	for (int i = 0; i < 10; i++)
		options[i] = 0;

	//if valid, add randomness number as a baseline
	if (getAttacking())
	{
		for (int i = 0; i < numEquippedWeapons(); i++)//loop through weapons
		{
			for (int j = 0; j < getWeapon(i)->numberAttacks(); j++) //and attacks
			{
				if (getWeapon(i)->hasAttackInSlot(j)) //if there is a valid attack, give its slot the randomness value
				{
					int slotNum = j + (i * 4) + 1;
					double mult[3] = { 1.0, 1.0, 1.0 };
					options[slotNum] = (getWeapon(i)->getDamage(true, j, mult) * damageMult) + (getWepSkill(classToWepSkill(getWeapon(i)->getClass())) * wepskillMult) + (getAttSkill(getWeapon(i)->wepAttack(j).type) * attskillMult); //add facot of damage done by the attack, and skill of that that weapon type, and sklill with that weapon type

					if (opposer->getWeapon(opposserWepIndex)->getClass() == "Polearm" && opposingDecision == "Parry" && getWeapon(i)->getClass() == "Unarmed") options[slotNum] *= 2.0; //if the parryign weapon is a polearm, and the attacking weapon is a fist, double the chance of firing
					if (opposer->getWeapon(opposserWepIndex)->getClass() == "Unarmed" && opposingDecision == "Parry" && getWeapon(i)->getClass() == "Polearm") options[slotNum] /= 2.0; //if the parrying weapon is a fist, and they have a polearm, half the chances of firing


					options[slotNum] += options[slotNum] * ((((float)getWeapon(i)->getAttackModBonus(j) + (float)getWeapon(i)->getAttackModBonus()) * modSkillMult) / 100.0); //Attack bonuses affect the chances
				
					options[slotNum] += randomness;

					if (getWeapon(i)->wepAttack(j).name == "Feint" && phase == 0) options[slotNum] = 0; //can't feint in phase 0
					else if (getWeapon(i)->wepAttack(j).name == "Feint" && phase == 1)
					{
						options[slotNum] += 20 + ((getAgility() + getReflex()) * 10);
						if (opposingDecision == "Parry" && opposer->getWeapon(opposserWepIndex)->getClass() == "Shield")//if they are trying to parry with a shield, try and feint if you can
						{
							options[slotNum] *= 4;
						}
					}

				}

			}
		}

		options[0] = randomness;

		//now look at the other player to determine some things

		if (getReflex() > opposer->getReflex() && getReact() < opposer->getReact())//if trailing reaction, and has good reaction, stading still can be a good idea, unless they are already standing still
		{
			options[0] = randomness + (getReflex() * 10);
		}


		if (phase == 1)
		{
			//determine still chances
			if (opposingDecision == "Still")
			{
				options[0] = randomness;
			}
		}





	}
	else //if the ai is defending
	{
		for (int i = 0; i < numEquippedWeapons(); i++)//loop through weapons
		{
			for (int j = 0; j < 2; j++) //and defenses
			{

				int slotNum = j + (i * 2) + 1;

				

				if (j == 0) //parry
				{
					options[slotNum] = ((getDefSkill(1) * defSkillMult) + (getWepSkill(classToWepSkill(getWeapon(i)->getClass())) * wepskillMult)); //add defense skill and weapon skill
					options[slotNum] += (getWeapon(i)->getDef() * 10); //shield bonus
					options[slotNum] *= getWeapon(i)->getBalance(); //balance bonus
					options[slotNum] += options[slotNum] * (((float)getWeapon(i)->getParryModBonus() * (float)modSkillMult) / 100.0); //Parry bonuses affect the chances
					if (opposer->getWeapon(opposserWepIndex)->getClass() == "Polearm" && opposingDecision == "Attack" && getWeapon(i)->getClass() == "Unarmed") options[slotNum] /= 2; //if the parryign weapon is a polearm, and the attacking weapon is a fist, double the chance of firing
					if (opposer->getWeapon(opposserWepIndex)->getClass() == "Unarmed" && opposingDecision == "Attack" && getWeapon(i)->getClass() == "Polearm") options[slotNum] *= 2; //if the parrying weapon is a fist, and they have a polearm, half the chances of firing
				
				}
				else //counter
				{
					options[slotNum] = ((getCurrentHp() - opposer->getCurrentHp()) * 5);
					if (options[slotNum] < 0) options[slotNum] = 0;

					if (getPosture() == -1) options[slotNum] /= 2.0; //decrease chances of counter if defensive
					else if (getPosture() == 1) options[slotNum] *= 4.0;
				}


				options[slotNum] += randomness;



			}
		}


		options[5] = (getDefSkill(2) * defSkillMult) + (getAgility() + getReflex()) * 10;
		options[0] = 0;

		if (phase == 1)
		{
			if (opposingDecision == "Still")
			{
				for (int i = 0; i < 10; i++)
					options[i] = 0;

				options[0] = 100; //guarantee the defender will stand still if the attacker does.
			}
			else
			{
				options[0] = 0; //don't stand still if you know the attacker won't
			}
		}
	}

	int decision = weightedIndexInts(options, 10);
	
	return decision;
}

int Character::saveCharacter()
{
	ifstream fin;

	fin.open(INDEX_FILE);
	if (fin.fail())
	{
		system("CLS");
		cout << ("Error loading index file");
		return 2;
	}

	string readLine;
	bool alreadyExist = false;


	while (!fin.eof())
	{
		getline(fin, readLine);

		if (readLine == getName()) alreadyExist = true;
	}

	fin.close();

	if (alreadyExist)
	{
		system("CLS");
		cout << "Save for " << getName() << " exists, overwrite? y/n";
		
		if (Write::yorn() == false) return 1;
	}

	ofstream fout;

	string outputFile = "SavedCharacters/" + getName() + ".txt";

	
	fout.open(outputFile);

	if (fout.fail())
	{
		system("CLS");
		cout << ("Error loading output file");
		return 3;
	}
	//now output data to file

	//output various stats
	fout << _color << endl;
	fout << _level << endl;
	fout << _maxHp << endl;
	fout << _currentHp << endl;
	fout << _currentReaction << endl;
	fout << _currentPosture << endl;
	fout << _defaultPosture << endl;
	fout << _strength << endl;
	fout << _agility << endl;
	fout << _toughness << endl;
	fout << _reflex << endl;
	fout << _isAi << endl;
	fout << attacking << endl;

	//output skills
	for (int i = 0; i < numWepSkills; i++)
		fout << wepSkills[i] << endl;
	for (int i = 0; i < numDefSkills; i++)
		fout << defSkills[i] << endl;
	for (int i = 0; i < numAttSkills; i++)
		fout << attSkills[i] << endl;

	//output weapon and inventory info

	fout << _equippedWeapons.size() << endl;

	//ouput each weapon
	for (int i = 0; i < _equippedWeapons.size(); i++)
	{
		fout << _equippedWeapons[i].getName() << endl;
		fout << _equippedWeapons[i].getClass() << endl;
		fout << _equippedWeapons[i].getDef() << endl;
		fout << _equippedWeapons[i].getArcDataDamageMin() << endl;
		fout << _equippedWeapons[i].getArcDataDamageMax() << endl;
		fout << _equippedWeapons[i].getSpeed() << endl;
		fout << _equippedWeapons[i].getOneHand() << endl;
		fout << _equippedWeapons[i].getTwoHand() << endl;
		fout << _equippedWeapons[i].getSlashing() << endl;
		fout << _equippedWeapons[i].getPiercing() << endl;
		fout << _equippedWeapons[i].getCrushing() << endl;
		fout << _equippedWeapons[i].gerArcDataReach() << endl;
		fout << _equippedWeapons[i].getArcDataRarity() << endl;
		fout << _equippedWeapons[i].getArcDataValue() << endl;
		fout << _equippedWeapons[i].getArcDataBalance() << endl;

		//don't store attack data...can be reinitialized (i think)

		fout << _equippedWeapons[i].getLevel() << endl;
		fout << _equippedWeapons[i].getSharpness() << endl;
		fout << _equippedWeapons[i].getSize() << endl;
		fout << _equippedWeapons[i].getTwoHanded() << endl;
		fout << _equippedWeapons[i].getBalanceMod() << endl;
	}


	fout.close();
	//if a new character, we need to add it to the list
	if (alreadyExist == false)
	{
		fout.open(INDEX_FILE, std::ios_base::app);
		if (fout.fail())
		{
			system("CLS");
			cout << ("Error loading output index file");
			return 4;
		}
		fout  << getName() << std::endl;
		fout.close();
	}
	
	return 0;
}

int Character::loadCharacter(string characterName)
{

	string fileName = "SavedCharacters/" + characterName + ".txt";

	ifstream fin;

	fin.open(fileName);
	if (fin.fail())
	{
		system("CLS");
		cout << ("Error loading character file");
		return 2;
	}

	_name = characterName;

	string temp;
	const int NUMLINES = 13;
	string lines[NUMLINES];
	for (int i = 0; i < NUMLINES; i++)
		getline(fin, lines[i]);

	_color = stoi(lines[0]);
	_level = stoi(lines[1]);
	_maxHp = stoi(lines[2]);
	_currentHp = stoi(lines[3]);
	_currentReaction = stoi(lines[4]);
	_currentPosture = stoi(lines[5]);
	_defaultPosture = stoi(lines[6]);
	_strength = stoi(lines[7]);
	_agility = stoi(lines[8]);
	_toughness = stoi(lines[9]);
	_reflex = stoi(lines[10]);
	_isAi = stoi(lines[11]);
	attacking = stoi(lines[12]);

	//temporary loading alt color
	_altcolor = 3;

	//skills
	for (int i = 0; i < numWepSkills; i++)
	{
		getline(fin, temp);
		wepSkills[i] = stof(temp);
	}
	for (int i = 0; i < numDefSkills; i++)
	{
		getline(fin, temp);
		defSkills[i] = stof(temp);
	}
	for (int i = 0; i < numAttSkills; i++)
	{
		getline(fin, temp);
		attSkills[i] = stof(temp);
	}

	//weapons
	getline(fin, temp);
	int numNewWeapons = stoi(temp);

	//remove old weapons
	for (int i = 0; i < numEquippedWeapons(); i++)
		removeWeapon(i);

	//create new weapons
	for (int i = 0; i < numNewWeapons; i++)
	{
		Weapon newWep;
		const int NUMWEPLINES = 20;
		string wepLines[NUMWEPLINES];

		for (int i = 0; i < NUMWEPLINES; i++)
			getline(fin, wepLines[i]);

		newWep.setName(wepLines[0]);
		newWep.setClass(wepLines[1]);
		newWep.setDef(stoi(wepLines[2]));
		newWep.setDamage(stoi(wepLines[4]), stoi(wepLines[3]));
		newWep.setSpeed(stof(wepLines[5]));
		newWep.setOneHand(stoi(wepLines[6]));
		newWep.setTwoHand(stoi(wepLines[7]));

		newWep.setSlashing(stof(wepLines[8]));
		newWep.setPiercing(stof(wepLines[9]));
		newWep.setCrushing(stof(wepLines[10]));

		newWep.setReach(stof(wepLines[11]));
		newWep.setRarity(stoi(wepLines[12]));
		newWep.setValue(stoi(wepLines[13]));
		newWep.setBalance(stof(wepLines[14]));

		newWep.setLevel(stoi(wepLines[15]));
		newWep.setSharpness(stoi(wepLines[16]));
		newWep.setSize(stoi(wepLines[17]));
		newWep.setTwoHanded(stoi(wepLines[18]));
		newWep.setBalanceMod(stoi(wepLines[19]));

		newWep.setupAttacks();
		equipWeapon(newWep, i, true, newWep.getTwoHanded());
	}

	fin.close();
}

int Character::deleteCharacter(string characterName)
{
	ifstream fin;
	fin.open(INDEX_FILE); //no error checking atm

	string readLine;
	bool exist = false;

	while (!fin.eof())
	{
		getline(fin, readLine);

		if (readLine == characterName) exist = true;
	}

	fin.close();

	if (exist)
	{
		system("CLS");
		cout << "Are you sure you want to delete " << characterName << "? y/n";
		
		if (Write::yorn())
		{
			string fileToRemove = "SavedCharacters/" + characterName + ".txt";
			remove(fileToRemove.c_str());

			ofstream fout;
			fout.open("SavedCharacters/temp.txt");

			fin.open(INDEX_FILE);

			while (getline(fin, readLine))
			{
				if (readLine != characterName)
					fout << readLine << endl;
			}

			fin.close(); fout.close();
			remove(INDEX_FILE.c_str());
			rename("SavedCharacters/temp.txt", INDEX_FILE.c_str());
			return 0;
		}
	}
	return 1;

}

Character Character::characterCreator()
{
	//choose name
	//then color
	//then assign stats
	Character temp;
	int currentStep = 1;

	string input;
	int abilityPoints = 4;

	while (currentStep > 0 && currentStep < 5)
	{
		system("CLS");
		if (currentStep == 1)
		{
			cout << "Enter your name: ";

			cin >> input;
			temp.setName(input);
			currentStep = 2;
		}
		else if (currentStep == 2)
		{
			cout << temp.getName() << endl << endl;

			cout << "Choose your player color: (press the corresponding letter)\n\n";
			
			char index = 'a';
			for (int i = 1; i < 16; i++)
			{
				cout << index << ") ";
				Write::wc("          ", i, 0);
				cout << endl;
				index++;
			}
			char selection = 0;

			while (selection < 'a' || selection >= 'p')
			{
				selection = _getch();
				if (selection == 'q')
				{	
					currentStep--;
					break;
				}
			}

			if (selection != 'q')
			{
				temp.setColor(selection - 'a' + 1);
				currentStep++;
			}
		}
		else if (currentStep == 3)
		{

			bool running = true;
			char input;
			int selection = 1;
			int numLines = 4;

			string desc1 = "Measure of physical strength. Increases damage done. Increases attack and defense for one handed weapons.";
			string desc2 = "Measure of quickness and flexibility. Increases attack and defense for two handed weapons. Increases dodge chance.";
			string desc3 = "Measure of ability to take a hit. Increases starting HP. Decreases reaction taken, and increases min reaction.";
			string desc4 = "Measure of mental quickness. Increases reaction gain, and max reaction. Increases ability to feint and dodge.";

			string desc[] = { desc1, desc2, desc3, desc4 };
			while(running)
			{
				

				string strLine = "Strength - " + to_string(temp.getStrength());
				string agiLine = "Agility - " + to_string(temp.getAgility());
				string touLine = "Toughness - " + to_string(temp.getToughness());
				string refLine = "Reflex - " + to_string(temp.getReflex());

				string lines[] = { strLine, agiLine, touLine, refLine, "Continue..." };


				


				system("CLS");
				Write::wc(temp.getName(), 0, temp.getColor());
				cout << " level " << temp.getLevel() << " | " << abilityPoints << " ability points left to spend. (e to increase, f to decrease) (c to continue)" << endl;
				cout << temp.generateLevOneMaxHp() << " HP | Reaction: " << temp.getMaxReaction() << "/" << temp.getStartingReact() << "/" << temp.getMinReaction();
				cout << " | Base Attack/Defense Values: One-Handed " << temp.getBaseValue(true, 0) << "/" << temp.getBaseValue(false, 0);
				cout << "   Two-Handed " << temp.getBaseValue(true, 1) << "/" << temp.getBaseValue(false, 1);
				cout << "   Mental " << temp.getBaseValue(true, 2) << endl << endl;

				for (int i = 0; i < numLines; i++)
				{
					if (i + 1 == selection) Write::wc(lines[i], 7, 0);
					else Write::wc(lines[i], 0, 7);
					cout << endl;
				}

				cout << desc[selection - 1];

				input = _getch();

				int num = 0;

				switch (input)
				{
				case 'w':
					if (selection > 1) selection--;
					break;

				case 's':
					if (selection < numLines) selection++;
					break;

				case 'e':
				{
					num = 1;
				}
					break;

				case 'f':
				{
					num = -1;
				}
				break;

				case 'q':
					currentStep--;
					break;

				case 'c':
					currentStep++;
					break;
				default:
					break;
				}

				if (num == 1)
				{
					if (abilityPoints > 0)
					{
						switch (selection)
						{
						case 1:
							temp.setStrength(temp.getStrength() + 1);
							break;
						case 2:
							temp.setAgility(temp.getAgility() + 1);
							break;
						case 3:
							temp.setToughness(temp.getToughness() + 1);
							break;
						case 4:
						{
							temp.setReflex(temp.getReflex() + 1);
							temp.setReaction(temp.getStartingReact());
						}	
						break;

						default:
							break;
						}
						abilityPoints--;
					}
				}
				else if (num == -1)
				{
					switch (selection)
					{
					case 1:
						if (temp.getStrength() > 0)
						{
							temp.setStrength(temp.getStrength() - 1);
							abilityPoints++;
						}
							break;
					case 2:
						if (temp.getAgility() > 0)
						{
							temp.setAgility(temp.getAgility() - 1);
							abilityPoints++;
						}
						break;
					case 3:
						if (temp.getToughness() > 0)
						{
							temp.setToughness(temp.getToughness() - 1);
							abilityPoints++;
						}
						break;
					case 4:
						if (temp.getReflex() > 0)
						{
							temp.setReflex(temp.getReflex() - 1);
							abilityPoints++;
							temp.setReaction(temp.getStartingReact());
						}
						break;

					default:
						break;
					}
				}
				
				if (currentStep != 3) break;
			}

		}
		else if (currentStep == 4)
		{
			//finilize things

			temp.setMaxHp(temp.generateLevOneMaxHp());
			temp.setHp(temp.getmaxHp());



			return temp;
 
		}

	}

	return temp;


}

int Character::getBaseValue(bool attack, int type)
{//returns the base attack or defense value. 0 is one handed, 1 is two handed, 3 is feint, 4 is dodge, 5 is counter
	int value = 0;		//base base value

	int minValue = 3; //smallest the base value can be

	switch (type)
	{
	case 0: //one hand
		if (attack)
			value += (getStrength() * 1.5) + getAgility();
		else
			value += (getStrength() * 1.5) + getReflex();
		break;

	case 1: //two hand
		if (attack)
			value += (getAgility() * 1.5) + getAgility();
		else
			value += (getAgility() * 1.5) + getReflex();
		break;

	case 2: //feint
		if (attack)
			value += (getReflex() * 1.5) + getAgility();
		else
			value += (getReflex() * 1.5) + getReflex();
		break;

	case 3://dodge
	{
		value += (getAgility() * 2) + (getReflex() * 1.5);

		value += (getReact() * 1.5); //easier to dodge the higher reaction you are

	}	break;

	case 4://counter. Attacker here is the true attacker. They are actually "defending against an attack" with a dodge
		if (attack)
			value += (getAgility() * 2) + (getReflex() * 1.5) + (getReact() * 1.5);
		else //counter attack pertends they are attacking with a weapon
			if (isTwoHandedWielding())
				value += (getAgility() * 1.5) + getAgility();
			else
				value += (getStrength() * 1.5) + getAgility();
		break;

	default://return just the scalar based on level
		return value;
		break;
	}


	if (value < 0) value = 0; //can't be negative
	
	value += getLevel() + minValue; // Add level and base number to it
	
	return value;
}

int Character::getFullAttDefVal(bool attack, int type, int wepSlot, int attacNum, int advantage, targetArea* target)
{
	//attackNum is the attack slot on the particular weapon
	Weapon* currentWeapon = getWeapon(wepSlot);

	//Base Value of attack or defense
	float value = getBaseValue(attack, type);

	//Add to the attack/def value skill bonuses
	value += getWepSkillAttack(attack, type, wepSlot);			//Bonus from Weapon skills
	value += getDefSkillAttack(attack, type, wepSlot);			//Bonus from Defensive SKill
	value += getAttSkillAttack(attack, type, wepSlot, attacNum);//Bonus from Attack Skills

	//Weapon Advantage (This has to be passed through)
	if (advantage > 0) value += advantage; //negative advantage is positive to the defender

	//Parry Values
	if (!attack && (type == 0 || type == 1)) //if parrying take into account balance
		value = (value * currentWeapon->getBalance());


	//Weapon Bonuses
	value += value * ((double)getWepBonus(attack, type, attacNum, currentWeapon) / 100.0);

	//Posture Bonuses
	value += value * ((double)getPostureBonus(attack, type) / 100.0);

	//Target bonus
	value += value * ((double)getTargetBonus(attack, type, attacNum, currentWeapon, target) / 100.0);

	if (value < 0) value = 0;

	return (int)value;

}
int Character::getFullAttDefVal(bool attack, int type, int wepSlot, int attackNum, int advantage)
{
	targetArea temp;
	temp.name = "Fake";
	for (int i = 0; i < 3; i++)
	{
		temp.damageMultiplier[i] = 1.0;
		temp.attackMultiplier[i] = 1.0;
	}
	temp.reactMultiplier = 1.0;
	temp.attackPenalty = 0;
	temp.defPenalty = 0;
	temp.canHaveArmor = true;
	temp.movementPenalty = 0;
	
	return getFullAttDefVal(attack, type, wepSlot, attackNum, advantage, &temp);
}

int Character::getWepSkillAttack(bool attack, int type, int wepSlot)
{
	int skillWeight = 5;
	int value = 0;

	if (wepSlot != -1)
	{
		//Weapon Skill Bonuses (Applied to both attacking and defending weapons)        //Wep skill is added to feint. And added to a defending counter
		if ((type != 3 && type != 4 && attack) || (!attack && type != 2 && type != 3))	//if attacking and not a dodge or a counter (an attacking counter is actually a dodge)														  
		{																				//or if defending and isn't a feint or a dodge
			//should be just getting weapon skill at this point
			if (getWeapon(wepSlot)->getClass() == "Shield") //wepSlot being zero is bad here
			{//shields skill is technically a defensive skill
				value += (defSkills[0] * skillWeight);
			}
			else //any other weapon class
			{
				//add corresponding wep skill
				value += (wepSkills[getWeapon(wepSlot)->getClassNum()] * skillWeight);
			}
		}
	}
	return value;
}

int Character::getDefSkillAttack(bool attack, int type, int wepSlot)
{
	int skillWeight = 5;
	int value = 0;
	//Def Skill Bonuses
	if ((!attack && type != 2 && type != 4) || (attack && type == 4)) //if defending and not a feint or counter. Or if attacking and trying to dodge a counter
	{
		if (type == 0 || type == 1) //if a weapon, use parry skill
		{
			if (getWeapon(wepSlot)->getClass() == "Shield")//if the weapon is a shield, and it is defending, gets the shield bonus again, not the parry one
				value += (defSkills[0] * skillWeight);
			else
				value += (defSkills[1] * skillWeight);
		}
		else if (type == 3 || type == 4)
		{//dodge defensive bonus (also for counter defense)
			value += (defSkills[2] * skillWeight);
		}
	}
	return value;
}

int Character::getAttSkillAttack(bool attack, int type, int wepSlot, int attNum)
{
	int skillWeight = 10;
	int value = 0;

																	//You get your feint skill whether attacking or defending against a feint
	if ((attack && type != 3 && type != 4) || (!attack && type == 2))//if attacking with a weapon or defending against a feint 
	{
		if (type == 0 || type == 1) //if a weapon
		{
			float tot = 0;
			for (int i = 0; i < 3; i++) //for each damage type, add skill based on how much damage the attack does
				tot += getWeapon(wepSlot)->wepAttack(attNum).damMax[i];

			for (int i = 0; i < 3; i++)
			{
				//multiply the proportion of the damage of the attack by the skill weight and the attack skill. This is usually one. (if not one, it means the attack uses multiple damage types. Your skills for each type are applied to each part seperatly.
				value += ((getWeapon(wepSlot)->wepAttack(attNum).damMax[i] / tot) * skillWeight * attSkills[i]);
			}

		}
		else if (type == 2)
		{//if defending or performiung a feint, add feint skill
			value += (attSkills[3] * skillWeight);
		}
	}
	return value;
}

int Character::getWepBonus(bool attack, int type, int attNum, Weapon* wep)
{
	int stalePenalty = 0;
	double value = 0;

	if (getLastAreaTargeted() == getAreaTargeted())
	{
		stalePenalty = -50; //penalty for attacking the same place twice
	}


	//Weapon Bonuses
	if (attack)
	{
		if (type == 0 || type == 1) //if a weapon (feints don't currently get bonuses or penalties (I think)) Also, attacking into a counter doen't need a bonus for the weapon
			value += (double)wep->getAttackModBonus() + stalePenalty + (double)getModBonus(0) + (double)wep->getAttackModBonus(attNum); //add the weapon bonus, plus any specific attack bonus
	}
	else
	{//defensive bonuses
		if (type == 0 || type == 1) //
			value += (double)wep->getParryModBonus() + (double)getModBonus(1); //add weapon poarry bonuses, plus those assosiated with the character
		else if (type == 4)//if defenfing with a counterattack, you get the attack bonuses/negatives assosiated with that weapon
			value += (double)wep->getAttackModBonus() + (double)wep->getAttackModBonus(attNum);
		else if (type == 3)//dodge bonus
			value += (double)getModBonus(2);

	}

	return value;
}

int Character::getPostureBonus(bool attack, int type)
{
	const int POSTURE_BONUS = 50;
	double value = 0;

	if (attack)
	{
		if (type == 4) //if attacking as countering, you are dodging, which gives you the defensive posture bonuses
			value = POSTURE_BONUS  * getPosture() * -1;
		else
			value = POSTURE_BONUS * getPosture();
	}
	else
	{
		if (type == 4) //defending with a counterattack, gives bonuses from aggressive posture
			value = POSTURE_BONUS * getPosture();
		else
			value = POSTURE_BONUS * getPosture() * -1;
	}
	return value;
}

int Character::getTargetBonus(bool attack, int type, int attackNum, Weapon* wep, targetArea* target)
{
	double value = 0;
	if (target->name != "Fake" && attack && (type == 0 || type == 1))
	{

		double attMult = 0;
		double total = 0;
		

		for (int i = 0; i < 3; i++)
			total += wep->wepAttack(attackNum).damMax[i];

		for (int i = 0; i < 3; i++)
		{
			//this determines the attack multiplier based on target. Each part of the attack gets it's multiplier multiplied to it. (rare). It is uaually only one attack type
			attMult += (wep->wepAttack(attackNum).damMax[i] / total) * target->attackMultiplier[i];
		}

		//if an attack with a weapon, use the multiplier for each target
		if (attack && (type == 0 || type == 1))
			value = (attMult - 1) * 100;
	}
	return value;
}

Weapon* Character::getCurrentWeapon()
{
	if (getWepSlotOption() >= 0)
		return getWeapon(getWepSlotOption());
	else
		return nullptr;
}

int Character::printOptionName(bool preview)
{
	int num = 0;


	if (getOption() == -1)
	{
		if (!preview) cout << "Nothing";
		num += 7;
	}
	else if (getOption() == 0 || getOption() == 1)//if a weapon
	{
		if (!preview) Write::wc( getWeapon(getWepSlotOption())->getName(), 0, getWeapon(getWepSlotOption())->getColor());
		num += getWeapon(getWepSlotOption())->getName().size();
		if (getAttacking()) //if they are the attacker
		{
			
			if (getAttTypeOption() != -1) //if attacking with an attack
			{
				if (!preview) cout << " " << getWeapon(getWepSlotOption())->wepAttack(getAttTypeOption()).name;
				num += getWeapon(getWepSlotOption())->wepAttack(getAttTypeOption()).name.size() + 1;
			}

		}
		else
		{
			if (!preview) cout << " Parry";
			num += 6;
		}
	}
	else if (getOption() == 2)//if feint
	{
		if (getAttacking())
		{
			if (!preview) Write::wc(getWeapon(getWepSlotOption())->getName(), 0, getWeapon(getWepSlotOption())->getColor());
			
			if (!preview) cout << " Feint";
			num += getWeapon(getWepSlotOption())->getName().size() + 6;
		}
		else
		{
			if (!preview) cout << "Feint Defense";
			num += 13;
		}
	}
	else if (getOption() == 3)
	{
		if (!preview) cout << "Dodge";
		num += 5;
	}
	else if (getOption() == 4)
	{
		if (getAttacking())//true attacker is dodging
		{
			if (!preview) cout << "Counter Dodge";
			num += 13;
		}
		else
		{
			if (!preview) Write::wc(getWeapon(getWepSlotOption())->getName(), 0, getWeapon(getWepSlotOption())->getColor());
			if (!preview) cout << " Counterattack";
			num += getWeapon(getWepSlotOption())->getName().size() + 14;
		}
	}

	return num;
}

void Character::viewModifiers()
{
	system("CLS");

	if (_attackMods.size() + _parryMods.size() + _dodgeMods.size() > 0)
	{
		printName(false, 0);
		cout << "'s modifiers." << endl << endl;

		for (int i = 0; i < _attackMods.size(); i++)
		{
			cout << _attackMods[i].name << ": ";

			Write::redGreen(_attackMods[i].value);

			cout << "% " << (char)175 << " for " << ((_attackMods[i].rounds - 1) / 2) + 1 << " rounds." << endl;

		}
		for (int i = 0; i < _parryMods.size(); i++)
		{
			cout << _parryMods[i].name << ": ";

			Write::redGreen(_parryMods[i].value);

			cout << "% parry mod for " << ((_parryMods[i].rounds - 1) / 2) + 1 << " rounds." << endl;

		}
		for (int i = 0; i < _dodgeMods.size(); i++)
		{
			cout << _dodgeMods[i].name << ": ";

			Write::redGreen(_dodgeMods[i].value);

			cout << "% dodge mod for " << ((_dodgeMods[i].rounds - 1) / 2) + 1 << " rounds." << endl;

		}

		cout << endl;
	}
	

	printName(false, 0);
	cout << "'s weapon modifiers." << endl;

	//loop through each weapon
	for (int i = 0; i < numEquippedWeapons(); i++)
	{
		getWeapon(i)->displayMods();


		cout << endl;
	}




	_getch();
}

void Character::newMod(int type, string name, int value, int rounds)
{
	Weapon::attackMod newMod;
	newMod.rounds = (rounds * 2) + 1;
	newMod.value = value;
	newMod.name = name;

	if (type == 0)
		_attackMods.push_back(newMod);
	else if (type == 1)
		_parryMods.push_back(newMod);
	else if (type == 2)
		_dodgeMods.push_back(newMod);
}
void Character::removeMod(int type, int modSlot)
{
	if (type == 0)
	{
		if (modSlot < _attackMods.size()) //if that mod exists
		{
			//delete it
			_attackMods[modSlot] = _attackMods.back();
			_attackMods.pop_back();
		}
	}
	else if (type == 1)
	{
		if (modSlot < _parryMods.size()) //if that mod exists
		{
			//delete it
			_parryMods[modSlot] = _parryMods.back();
			_parryMods.pop_back();
		}
	}
	else if (type == 2)
	{
		if (modSlot < _dodgeMods.size()) //if that mod exists
		{
			//delete it
			_dodgeMods[modSlot] = _dodgeMods.back();
			_dodgeMods.pop_back();
		}
	}
	
}
int Character::getModBonus(int type)
{
	if (type == 0)
	{
		int size = _attackMods.size();

		int mod = 0;

		for (int i = 0; i < size; i++)
		{
			//loop through each modifier
			mod += _attackMods[i].value;
		}
		return mod;
	}
	else if (type == 1)
	{
		int size = _parryMods.size();

		int mod = 0;

		for (int i = 0; i < size; i++)
		{
			//loop through each modifier
			mod += _parryMods[i].value;
		}
		return mod;
	}
	else if (type == 2)
	{
		int size = _dodgeMods.size();

		int mod = 0;

		for (int i = 0; i < size; i++)
		{
			//loop through each modifier
			mod += _dodgeMods[i].value;
		}
		return mod;
	}
	
}
void Character::cycleModRounds()
{

	//loops through weapon mods
	for (int j = 0; j < _attackMods.size(); j++) //loop through each modifier
	{
		_attackMods[j].rounds--;

		//if the timer has run out, remove the modifier
		if (_attackMods[j].rounds <= 0)
			removeMod(0, j);

		//if a mod is removed, it should change the modifier vector size, decreasing the time the for loop goes.
	}
	//loops through parry mods
	for (int j = 0; j < _parryMods.size(); j++) //loop through each modifier
	{
		_parryMods[j].rounds--;

		//if the timer has run out, remove the modifier
		if (_parryMods[j].rounds <= 0)
			removeMod(1, j);

		//if a mod is removed, it should change the modifier vector size, decreasing the time the for loop goes.
	}
	//loops through parry mods
	for (int j = 0; j < _dodgeMods.size(); j++) //loop through each modifier
	{
		_dodgeMods[j].rounds--;

		//if the timer has run out, remove the modifier
		if (_dodgeMods[j].rounds <= 0)
			removeMod(2, j);

		//if a mod is removed, it should change the modifier vector size, decreasing the time the for loop goes.
	}

	
}