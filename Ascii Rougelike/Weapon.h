#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <sstream>
#include <cmath>
#include <math.h>
#include <iostream>

#pragma once

using namespace std;
class Weapon
{
public:
	Weapon();
	~Weapon();

	//Data struct for loading in weapon data
	struct wep {
		string name;
		string clss;
		int def;
		int damageMin;
		int damageMax;
		float  speed;
		int oneHand, twoHand;
		float damType[3];
		float reach;
		int rarity;
		int value;
		float balance;
	};

	struct attackMod
	{
		string name;
		int rounds;
		int value;
	}; //modifier struct

	struct attack
	{
		int type;
		string name;

		//damage is split into three components. Only certain attacks have values in each
		int damMax[3];
		int damMin[3];

		vector<attackMod> attackModifiers;
	};

	void operator=(const Weapon& w) {
		_level = w._level;
		

		arcData = w.arcData;

		_sharpness = w._sharpness;

		_attacks = w._attacks;
		_twoHanded = w._twoHanded;
		_balanceMod = w._balanceMod;
		_defenseCanceled = w._defenseCanceled;
	}
	
	
	
	//file for archeotypes
	const string ARCFILE = "WeaponArcheotypes.txt";




	//getters
	float getReach(); float gerArcDataReach() { return arcData.reach; }
	float getSpeed() { return arcData.speed; }
	int getOneHand() { return arcData.oneHand; }
	int getTwoHand() { return arcData.twoHand; }
	int getDef() { return arcData.def;}
	float getSlashing() { return arcData.damType[0]; }
	float getPiercing() { return arcData.damType[1]; }
	float getCrushing() { return arcData.damType[2]; }
	
	float getBalance(); float getArcDataBalance() { return arcData.balance; }
	string getBalanceString();

	int getBalanceMod() { return _balanceMod; }

	int getSharpness() { return _sharpness; }
	string getSharpnessString();
	
	int getSize() { return _size; }

	string getName() { return arcData.name; }
	string getClass() { return arcData.clss; }
	
	int getLevel() { return _level; }
	string getRarityString();
	int getRarity(); int getArcDataRarity() { return arcData.rarity; }
	int getColor();

	int getArcDataValue() { return arcData.value; }
	
	int getClassNum(); //converst a class string to the class index
	int getClassNum(string clss);

	bool oneHanded();
	bool getTwoHanded() {return _twoHanded;}

	int getDamage(bool max, int attackIndex, double multipliers[3]); //get either min or max damage. This includes sharpness and such
	int randomDamage(int attackSlot, double multipliers[3]);
	
	int numberAttacks() { return _attacks.size(); }
	attack wepAttack(int value) {
		if (hasAttackInSlot(value)) { return _attacks[value]; }
		else { std::cout << "No attack in this slot"; system("PAUSE"); }
		return _attacks[0];
	}//returns the attack struct located at the specified value
	bool hasAttackInSlot(int attackIndex) { if (attackIndex < numberAttacks()) return true; else return false; } //returns if there is an attack in the specified slot
	int hasAttackOfType(int attackType); //returns the index of an attack of the desired type. -1 if it can't be found
	int getAttackType(int attackSlot); //returns 0-2 for slashing-piercing. This is the primary damage type for the weapon

	int getArcDataDamageMax() { return arcData.damageMax; }
	int getArcDataDamageMin() { return arcData.damageMin; }

	int weaponAdvantage(int thisAttacking, Weapon* otherWeapon);

	int getDodgeModifier() { return (arcData.speed - 2) * 10; } //this shows the advantage the weapon has agaist someone dodging. Used instead of normal weapon advantage

	bool isDefenseCanceled() { return _defenseCanceled; }


	//setters
	void setDamage(int max, int min);
	void setName(string name) { arcData.name = name; }
	void setClass(string clss) { arcData.clss = clss; }
	void setDef(int defenseBonus) { arcData.def = defenseBonus; }
	void setSpeed(float speed) { arcData.speed = speed;  }
	void setOneHand(int strength) { arcData.oneHand = strength; }
	void setTwoHand(int agility) { arcData.twoHand = agility; }
	void setSlashing(float slashing) { arcData.damType[0] = slashing; }
	void setPiercing(float piercing) { arcData.damType[1] = piercing; }
	void setCrushing(float crushing) { arcData.damType[2] = crushing; }
	void setReach(float reach) { arcData.reach = reach; }

	void setRarity(int rarity) { arcData.rarity = rarity; }
	void setValue(int value) { arcData.value = value; }

	void setBalance(float balance) { arcData.balance = balance; }

	void setLevel(int level) { _level = level; }
	void setSharpness(int sharpness) { _sharpness = sharpness; }
	void setSize(int size) { _size = size; }
	void setTwoHanded(bool istwohanded) { _twoHanded = istwohanded; }
	void setBalanceMod(int balanceMod) { _balanceMod = balanceMod; }

	void cancelDefense() { _defenseCanceled = true; }
	void restoreDefense() { _defenseCanceled = false; }

	//Display Functions
	void display(); //display weapon stats
	void displayAttacks(); //display weapon attacks
	void displayName(); //display name
	void displayRarity(); //display rarity
	
	void displayMods(); //displays weapon and attack modifiers



	//Weapon Generation
	bool generateWeaponToSkill(int weaponClass, int wieldType, int skill, int rarityLimit); //returns true if a success

	int loadArcheotypes(vector<wep> & weplist); //load all archeotypes
	int loadArcheotypes(vector<wep>& weplist, int wepClass, int wieldType, int skillLimit, int rarityMin); //this version only loads in archeotypes that fit the requirements
	int loadArcheotypes(vector<wep>& weplist, int skillLimit, int rarityMin);
	
	void setupAttacks(); //sets up the initial attacks for the weapon during generation
	string getAttackName(int type, string clss);  //gets unique attack names




	//Utility Functions
	static int weightedIndex(vector<int>& weights); //weigted index

	


	//Weapon Modifiers
	int getAttackModBonus(int attackSlot); //Returns the total % modifier for the specified attack 
	int getAttackModBonus();
	int getParryModBonus();
	int getNumberOfAttackMods() { return _attackMods.size(); }

	void newAttackMod(string name, int attackSlot, int value, int rounds); //creates a new attack mod on the specified attack
	void newAttackMod(string name, int value, int rounds); //creates a new attack mod for each of the weapons attacks
	void newParryMod(string name, int value, int rounds);

	void removeAttackMod(int attackSlot, int modSlot);
	void removeAttackMod(int modSlot);
	void removeParryMod(int modSlot);

	void cycleModRounds();

	
	


private:

	//weapon stats stored here
	wep arcData;

	
	vector<attack> _attacks;		//list of attacks (mods stored within
	vector<attackMod> _parryMods;	//weapon parry modifiers
	vector<attackMod> _attackMods;	//modifiers for specific attacks

	//Weapon Level. Weapons scale from 1 onward
	int _level;

	int _sharpness; // -2 Blunt, -1 Dull, 0 Normal, 1 Honed, 2 Keen  (sharpness gives bonus to damage)

	int _size; // -2 Small, -1 Undersized, 0 Normal, 1 Oversized, 2 Huge (Being small gives bonus to speed, penalty to reach, penalty to damage, defensive bonus)

	bool _twoHanded; //stores if the weapon is currenlty being wieldied by one or two hands

	int _balanceMod; //-2 Awkward, -1 Unbalanced, 0 Normal, 1 Balanced, 2 Finetuned

	bool _defenseCanceled; //This is temporarily true when successfully feinted against.

};
