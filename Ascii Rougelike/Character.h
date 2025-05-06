#include <string>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <math.h>

#include "Weapon.h"
#include "Write.h"

#pragma once

#define arraySize(a) (sizeof(a) / sizeof(float))

using namespace std;

const int numWepSkills = 6;
const int numDefSkills = 3;
const int numAttSkills = 4;
const int numSkills = numWepSkills + numDefSkills + numAttSkills;
const int skillMultiplier = 90; //the higher this is, the slower skills are earned

const int attackOp = 9;
const int defOp = 5;

const string INDEX_FILE = "SavedCharacters/savedCharacterIndex.txt";

class Character
{


public:
	Character();
	~Character();


	struct targetArea {
		string name;
		double damageMultiplier[3];	//one for each damage type
		double attackMultiplier[3];	//one for each damage type
		double reactMultiplier;		//higher does more reaction damage

		int attackPenalty;			//A successful hit to this area adds a penalty worth this
		int defPenalty;				//Same
		int movementPenalty;


		bool canHaveArmor;			//if this can be an armor slot

		void operator=(const targetArea& w) {
			name = w.name;


			for (int i = 0; i < 3; i++)
			{
				damageMultiplier[i] = w.damageMultiplier[i];
				attackMultiplier[i] = w.attackMultiplier[i];
			}

			reactMultiplier = w.reactMultiplier;
			attackPenalty = w.attackPenalty;
			defPenalty = w.defPenalty;
			movementPenalty = w.movementPenalty;

			canHaveArmor = w.canHaveArmor;
		}

	};

	

	//Getters
	string getName() { return _name; }
	int getLevel() { return _level; }
	int getmaxHp() { return _maxHp; }
	int generateLevOneMaxHp() { return 10 + (3 * _toughness); }
	int getCurrentHp() { return _currentHp; }
	int getStrength() { return _strength; }
	int getAgility() { return _agility; }
	int getToughness() { return _toughness; }
	int getReflex() { return _reflex; }
	int getColor() {
		if (isAltColorNeeded()) return _altcolor;
		else return _color;
	}

	int getReact() { return _currentReaction; }
	int getMaxReaction();
	int getMinReaction();
	int getStartingReact();

	int getPosture() { return _currentPosture; }
	int getDefPosture() { return _defaultPosture; }
	string getPostureString();

	float getSkill(int skillType, int skillIndex);
	float getWepSkill(int skillIndex);
	float getDefSkill(int skillIndex) { return defSkills[skillIndex]; }
	float getAttSkill(int skillIndex) { return attSkills[skillIndex]; }
	int classToWepSkill(string clss);
	string skillName(int type, int skll); //returns the name of the skill for the certain type

	bool getAttacking() { return attacking; }

	Weapon* getWeapon(int slot);
	Weapon* getCurrentWeapon();

	int numEquippedWeapons() { return _equippedWeapons.size(); }
	bool isTwoHandedWielding();

	bool isAI() { return _isAi; }


	int getBaseValue(bool attack, int type); //returns the base attack or defense value. 0 is one handed, 1 is two handed, 2 is feint, 3 is dodge, 4 is counter
	int getWepSkillAttack(bool attack, int type, int wepSlot);				//These functions return the added bonus to attack or defense from the different categories of skills
	int getDefSkillAttack(bool attack, int type, int wepSlot);
	int getAttSkillAttack(bool attack, int type, int wepSlot, int attNum);
	int getWepBonus(bool attack, int type, int attNum, Weapon* wep); //returns the percentage value of the weapon bonus
	int getPostureBonus(bool attack, int type); //returns the bonus due to posture
	int getTargetBonus(bool attack, int type, int attackNum, Weapon *wep, targetArea* target);


	int getFullAttDefVal(bool attack, int type, int wepSlot, int attackNum, int advantage, targetArea* target);
	int getFullAttDefVal(bool attack, int type, int wepSlot, int attackNum, int advantage);

	int getOption() { return _attDefOption; }		//gets the option for the attack/defense
	int getAttTypeOption() { return _attOption; }	//gets the option for the weapon attack
	int getWepSlotOption() { return _wepSlotOption; }

	int getCurrentAttackRoll() {return _currentAttackRoll;}
	int getCurrentDefenseRoll() { return _currentDefenseRoll; }

	bool isAltColorNeeded() { return _altcolorNeeded; }

	targetArea* getTargetArea(int slot) { return &_targetAreas[slot]; }
	int getNumberofTargetAreas() { return _targetAreas.size(); }
	int getAreaTargeted() { return _areaTargeted; }

	int getLastAreaTargeted() { return _lastAreaTargeted; }

	//Setters
	void setName(string name) { _name = name; }
	void setLevel(int level) { _level = level; } 
	void setMaxHp(int hp) { _maxHp = hp; } 
	void setHp(int hp) { _currentHp = hp; } 
	void setStrength(int str) { _strength = str; } 
	void setAgility(int ag) { _agility = ag; } 
	void setToughness(int to) { _toughness = to; }
	void setReflex(int to) { _reflex = to; }

	void setColor(int color) { _color = color; }

	void setReaction(int value);
	void changeReaction(int mode, int extra);
	int alterReaction(int value);// returns value raise by
	
	void alterHP(int amount);

	void setAttacking(bool value) { attacking = value; }
	void cycleAttacking(){ attacking = !attacking; }
	
	void setPosture(int posture) { _currentPosture = posture; }
	void setDefPosture(int posture) { _defaultPosture = posture; }
	
	void skillUp(int type, int index, int times);

	bool equipWeapon(Weapon wep, int slot, bool overwrite, bool twohanded);
	bool removeWeapon(int slot);
	bool addFist(int slot);

	void setAI(bool isAI) { _isAi = isAI; }


	void setOption(int option) { _attDefOption = option; }
	void setAttTypeOption(int attTypeOption) { _attOption = attTypeOption; }
	void setWeaponSlotOption(int wepSlotOption) { _wepSlotOption = wepSlotOption; }

	void setAttRoll(int value) {_currentAttackRoll = value; }
	void setDefRoll(int value) {_currentDefenseRoll = value; }

	void setAltColorNeeded(bool value) { _altcolorNeeded = value; }

	void setAreaTargeted(int value) { _areaTargeted = value; }
	void setLastAreaTargeted(int value) { _lastAreaTargeted = value; }


	//Display Functions
	void display();
	void printName(bool pad, int widthPadding); //prints name to the screen in the players color
	int printOptionName(bool preview); //returns the number of characters


	//Character generation
	void generateRandom(int power, int gender);
	string randomName(int gender);

	void generateWeapon(bool righthand);

	int saveCharacter(); //saves charater to file. Checks if character exists
	int loadCharacter(string characterName); //loads character data from file

	void setupTargetAreas();


	static int deleteCharacter(string characterName);


	//Utility Functions
	int weightedIndex(float weights[], int length);
	int weightedIndex(float weights[], int length, float extra);
	int weightedIndexInts(int weights[], int length);
	
	

	//Menus
	void changePosture();
	static Character characterChooser(int level, string title);
	static Character savedCharacters();
	static Character characterCreator();
	
	void viewModifiers(); //displays in a menu, all of the characters weapons and their attack modifiers

	//AI
	int aiDecision(Character * opposer, int phase, string opposingDecision, int opposserWepIndex); //generates a decision from the ai (0-9) based on thir skills, and the opposers
	int aiChoice(Character* other, int phase); //The new ai function


	//Modifiers
	void newMod(int type, string name, int value, int rounds);
	void removeMod(int type, int modSlot);
	int getModBonus(int type);
	void cycleModRounds();

private:
	string _name;
	int _color;
	int _altcolor;
	int _level;
	int _maxHp;
	int _currentHp;
	int _currentReaction;
	
	int _currentPosture; //-1 through 1 (-1 defensive, +1 aggresive
	int _defaultPosture; 


	vector<Weapon> _equippedWeapons;

	vector<targetArea> _targetAreas;	//different areas that can be targeted

	vector<Weapon::attackMod> _attackMods;		//modifiers tied to the character
	vector<Weapon::attackMod> _parryMods;
	vector<Weapon::attackMod> _dodgeMods;

	int _areaTargeted; //area targeted in the enemy. 0 by defualt. 
	int _lastAreaTargeted; //this area will have a penalty to attacking

	//ability scores
	int _strength;
	int _agility;
	int _toughness;
	int _reflex;

	//skills
	float wepSkills[numWepSkills];
	float defSkills[numDefSkills];
	float attSkills[numAttSkills];
	

	
	bool _isAi;
	bool attacking;
	int _attDefOption; //-1 do nothing, 0 one hand weapon att/parry, 1 two hand, 2 feint, 3 dodge, 4 counter
	int _wepSlotOption; //-1 for no weapon, 0+ for weapon in that slot
	int _attOption; //-1 if no attack slot, 0+ for that attack slot

	int _currentAttackRoll;  //holds current attack roll
	int _currentDefenseRoll; //holds current defense roll

	bool _altcolorNeeded;

};

