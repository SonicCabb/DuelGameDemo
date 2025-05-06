#include "Weapon.h"
#include "Write.h"

using namespace std;
Weapon::Weapon()
{
	arcData.def = 0;
	arcData.damageMax = 2;
	arcData.damageMin = 3;
	arcData .name= "Fist";
	_level = 0;


	arcData.speed = 2;

	arcData.oneHand = 0;
	arcData.twoHand = 1;

	//new attack type idea
	attack fist;
	fist.damMax[2] = 3; fist.damMax[0] = 0; fist.damMax[1] = 0;
	fist.damMin[2] = 1; fist.damMin[0] = 0; fist.damMin[1] = 0;
	fist.name = "Punch";
	fist.type = 2; //crushing


	_attacks.push_back(fist);


	//add feint attack
	attack feint;
	feint.name = "Feint";
	feint.type = 3;

	for (int j = 0; j < 3; j++)
	{
		feint.damMax[j] = 0;
		feint.damMin[j] = 0;
	}
	_attacks.push_back(feint);


	arcData.damType[0] = arcData.damType[1] = 0;
	arcData.damType[2] = 1;

	arcData.reach = .5;

	arcData.clss = "Unarmed";

	arcData.rarity = 10000;
	arcData.value = 0;

	arcData.balance = 1;

	_sharpness = 0;
	_size = 0;
	_balanceMod = 0;

	_twoHanded = false;
	_defenseCanceled = false;


	setupAttacks();
}


Weapon::~Weapon()
{
}

string Weapon::getRarityString()
{

	if (arcData.rarity > 2000) return "Common";				//Dark Grey
	else if (arcData.rarity > 500) return "Uncommon";		//Green
	else if (arcData.rarity > 100) return "Unique";			//Blue
	else if (arcData.rarity > 20) return "Rare";			//Purple
	else if (arcData.rarity > 5) return "Legendary";		//Gold
	else return "Iconic";									//Cyan

}


//checks if the weapon is best held one handed, or two handed
bool Weapon::oneHanded()
{
	if (arcData.oneHand > arcData.twoHand) return false;
	else return true;
}

//sets weapon damage
void Weapon::setDamage(int max, int min)
{
		arcData.damageMax = max;
		arcData.damageMin = min;

}


int Weapon::getClassNum()
{
	if (getClass() == "Sword") return 0;
	else if (getClass() == "Axe") return 1;
	else if (getClass() == "Polearm") return 2;
	else if (getClass() == "Club") return 3;
	else if (getClass() == "Knife") return 4;
	else if (getClass() == "Unarmed") return 5;
	else return 6;
}
int Weapon::getClassNum(string clss)
{
	if (clss == "Sword") return 0;
	else if (clss == "Axe") return 1;
	else if (clss == "Polearm") return 2;
	else if (clss == "Club") return 3;
	else if (clss == "Knife") return 4;
	else if (clss == "Unarmed") return 5;
	else return 6;
}


//Displays weapon info
void Weapon::display()
{
	displayName();

	cout << " - Level ";
	if (getOneHand() < getTwoHand()) cout << getOneHand() << " ";
	else cout << getTwoHand() << " ";

	cout << getClass() << " (";
	displayRarity();
	cout << ")";
	
	cout << endl << "Speed " << getSpeed() << "   Reach " << getReach() << "   Balance " << getBalance();
	cout << "   Difficulty " << getOneHand() << "/" << getTwoHand();
	if (getDef() != 0) cout << "   Defensive Bonus " << getDef();

	cout << endl;
	displayAttacks();

	if (getTwoHanded()) cout << "Two-Handed Grip" << endl;

}


//function to load weapon archeotype data. Weapon vector reference is passed in
int Weapon::loadArcheotypes(vector<wep>& weplist)
{

	ifstream fin;		//input file stream
	string input;		//temperary string for loading in data
	int counter = 0;	//used to keep track of the number of archeotypes
	wep tempWep;		//used to input weapon


	//error checking
	fin.open(ARCFILE);
	if (fin.fail()) {
		perror(ARCFILE.c_str());
		return 0;	//if there isn't any file we can abort the load
	}

	//while we haven't reached the end of the document, keep reading the lines
	while (getline(fin, input))
	{

		//get name
		tempWep.name = input;

		//input Weapon class
		getline(fin, input);
		tempWep.clss = input;

		//Input Damage info
		getline(fin, input);
		tempWep.damageMin = stoi(input);

		getline(fin, input);
		tempWep.damageMax = stoi(input);

		//Input Slashing
		getline(fin, input);
		tempWep.damType[0] = stof(input);

		//Input Piercing
		getline(fin, input);
		tempWep.damType[1] = stof(input);

		//Input Crushing
		getline(fin, input);
		tempWep.damType[2] = stof(input);

		//Input Reach
		getline(fin, input);
		tempWep.reach = stof(input);

		//Input Speed
		getline(fin, input);
		tempWep.speed = stof(input);

		//Input Defensive Bonus
		getline(fin, input);
		tempWep.def = stoi(input);

		//Input OneHand Difficulty
		getline(fin, input);
		tempWep.oneHand = stof(input);

		//Input TwoHand Difficulty
		getline(fin, input);
		tempWep.twoHand= stof(input);

		//Input Rarity
		getline(fin, input);
		tempWep.rarity = stoi(input);

		//Input Value
		getline(fin, input);
		tempWep.value = stoi(input);

		//Input Value
		getline(fin, input);
		tempWep.balance = stof(input);

		//put weapon on vector
		weplist.push_back(tempWep);

		//get ~ sign
		getline(fin, input);

		//increase counter each time.
		counter++;

	}
	//when it hits the end of the document, the loading is done

	//close the file
	fin.close();
	return counter;

}
int Weapon::loadArcheotypes(vector<wep>& weplist, int wepClass, int wieldType, int skillLimit, int rarityMin)
{
	//load all the archeotypes in
	vector<wep> loadedWeapons;
	loadArcheotypes(loadedWeapons);
	int counter = 0;

	//go through each weapon, checking for skil, wield type, rarity andclass
	for (int i = 0; i < loadedWeapons.size(); i++)
	{
		//figure out the wield type of the laoded weapon, and the skill associated with it
		int wield; int skill;
		if (loadedWeapons[i].oneHand < loadedWeapons[i].twoHand) { wield = 0; skill = loadedWeapons[i].oneHand; }
		else { wield = 1; skill = loadedWeapons[i].twoHand; }

		//then add the wepaon to the vector options, if it indeed fits the criteria
		if (getClassNum(loadedWeapons[i].clss) == wepClass && wield == wieldType && loadedWeapons[i].rarity >= rarityMin && skill <= skillLimit)
		{
			weplist.push_back(loadedWeapons[i]);
			counter++;
		}
	}
	return counter;

}
int Weapon::loadArcheotypes(vector<wep>& weplist, int skillLimit, int rarityMin)
{

	//load all the archeotypes in
	vector<wep> loadedWeapons;
	loadArcheotypes(loadedWeapons);
	int counter = 0;

	//go through each weapon, checking for skil, wield type, rarity andclass
	for (int i = 0; i < loadedWeapons.size(); i++)
	{
		//figure out the skill associated with it
		int skill;
		if (loadedWeapons[i].oneHand < loadedWeapons[i].twoHand) { skill = loadedWeapons[i].oneHand; }
		else {skill = loadedWeapons[i].twoHand; }

		//then add the wepaon to the vector options, if it indeed fits the criteria
		if (loadedWeapons[i].rarity >= rarityMin && skill <= skillLimit)
		{
			weplist.push_back(loadedWeapons[i]);
			counter++;
		}
	}
	return counter;

}

bool Weapon::generateWeaponToSkill(int weaponClass, int wieldType, int skill, int rarityLimit)
{
	vector<wep> possibleWeapons;

	loadArcheotypes(possibleWeapons, weaponClass, wieldType, skill, rarityLimit);

	if (possibleWeapons.size() == 0) return false;
	else
	{
		vector<int> weights;
		for (int i = 0; i < possibleWeapons.size(); i++)
		{
			weights.push_back(possibleWeapons[i].rarity);
		}

		int wepChoice = weightedIndex(weights);

		arcData = possibleWeapons[wepChoice];

		//generate sharpness
		vector<int> sharpWeight = { 200, 500, 3000, 500, 200 };
		
		if (getPiercing() != 0 || getSlashing() != 0)
			_sharpness = weightedIndex(sharpWeight) - 2;


		//Generate BalanceMod
		vector<int> balanceWeight = { 200, 500, 3000, 500, 200 };
		_balanceMod = weightedIndex(balanceWeight) - 2;



		if (wieldType == 1) _twoHanded = true;
		
		setupAttacks();

		return true;
	}
}

int Weapon::weightedIndex(vector<int>& weights)
{
	int weightSum = 0;

	int randomIndex = 0;

	//determine weightsum
	for (int i = 0; i < weights.size(); i++)
	{
		weightSum += (weights[i] );
	}

	//if all the weights are 0, just give a random index
	if (weightSum == 0) return (rand() % weights.size());
	else
	{
		//determine random index based on the weight sum
		randomIndex = rand() % weightSum;

		//loop through each element, seeing if the random number is found in each
		for (int i = 0; i < weights.size(); i++)
		{
			if (randomIndex < (weights[i])) return i;	//if it is, return it
			//if not, decreas the random index by the weight
			randomIndex -= (weights[i]);
		}

	}

	return 0;

}

void Weapon::displayName()
{
	Write::setColor(0, getColor());
	if (_balanceMod != 0) cout << getBalanceString() << " ";
	if (getSharpness() != 0) cout << getSharpnessString() << " ";
	cout << getName();
	Write::setColor(0, 7);
}

void Weapon::displayRarity()
{
	Write::wc(getRarityString(), 0, getColor());
}

int Weapon::getColor()
{
	if (getRarityString() == "Common") return 8;
	else if (getRarityString() == "Uncommon") return 2;
	else if (getRarityString() == "Unique") return 9;
	else if (getRarityString() == "Rare") return 5;
	else if (getRarityString() == "Legendary") return 6;
	else return 11;
}

string Weapon::getSharpnessString()
{
	switch (_sharpness)
	{
	case -2:
		return "Blunt";
		break;
	case -1:
		return "Dull";
		break;
	case 0:
		return "Normal";
		break;
	case 1:
		return "Honed";
		break;
	case 2:
		return "Keen";
		break;
	default:
		return "Invalid";
		break;
	}
}

string Weapon::getBalanceString()
{
	switch (_balanceMod)
	{
	case -2:
		return "Lopsided";
		break;
	case -1:
		return "Unstable";
		break;
	case 0:
		return "Normal";
		break;
	case 1:
		return "Tuned";
		break;
	case 2:
		return "Fine Tuned";
		break;
	default:
		return "Invalid";
		break;
	}
}

int Weapon::getRarity()
{
	float val = 0;
	if (_sharpness > 0)
		val = arcData.rarity * ((_sharpness * 10) / 100.0);

	return arcData.rarity - val;
}

float Weapon::getReach()
{
	float reach = arcData.reach;
	if (oneHanded() && _twoHanded == true) reach -= (reach * .5); //one handed weapon wielded in two hands gives a huge penalty to reach (paired with double damage)
	if (!oneHanded() && _twoHanded == false) reach += (reach * .5); //two handed weapon wielded in one hand gives a huge reach bonus
	return reach;
}


void Weapon::displayAttacks()
{
	int num = _attacks.size();

	double multipliers[3] = { 1.0, 1.0, 1.0 };

	cout << "Attacks: ";
	for (int i = 0; i < num; i++)
	{
		if (_attacks[i].name != "Feint")	//don't display feint attacks
		{
			cout << _attacks[i].name << " [";

			if (getDamage(false, i , multipliers) != getDamage(true, i, multipliers))
				cout << getDamage(false, i, multipliers) << "-";
				
			cout << getDamage(true, i, multipliers) << " dmg]  ";
		}
	}
	cout << endl;
}
void Weapon::setupAttacks()
{
	_attacks.clear();

	int i = 0;
	if (arcData.clss == "Club") i = 2; //if a club, only do the crushing attack 
	
	//loop through each of the attacks, and add it to the attacks vector
	for (; i < 3; i++)
	{
		attack atk;


		if (arcData.damType[i] > 0) //if the attack exists
		{
			atk.name = getAttackName(i, arcData.clss);
			atk.type = i;


			for (int j = 0; j < 3; j++)
			{
				atk.damMax[j] = 0;
				atk.damMin[j] = 0;
			}
			//if the weapon is a club, add the other attack damage values
			if (arcData.clss == "Club" || arcData.clss == "Shield")
			{
				//add extra slashing and piercing
				atk.damMax[0] += arcData.damageMax * arcData.damType[0];
				atk.damMax[1] += arcData.damageMax * arcData.damType[1];
				atk.damMin[0] += arcData.damageMax * arcData.damType[0];
				atk.damMin[1] += arcData.damageMax * arcData.damType[1];

			}
			else if (arcData.clss == "Axe")
			{
				//add extra crushing damage
				if (i == 0)
				{
					atk.damMax[2] += arcData.damageMax * arcData.damType[2];
					atk.damMin[2] += arcData.damageMax * arcData.damType[2];
				}
			}

	
			//the damage max and min values are split 
			atk.damMax[i] += arcData.damageMax * arcData.damType[i];
			atk.damMin[i] += arcData.damageMin * arcData.damType[i];

			_attacks.push_back(atk);
		}

	}


	//add feint attack
	attack feint;
	feint.name = "Feint";
	feint.type = 3;

	for (int j = 0; j < 3; j++)
	{
		feint.damMax[j] = 0;
		feint.damMin[j] = 0;
	}
	_attacks.push_back(feint);
}
string Weapon::getAttackName(int type, string clss)
{
	string name;

	//default names of attacks
	if (type == 0) name = "Slash";
	else if (type == 1) name = "Pierce";
	else name = "Bash";


	if (clss == "Axe")
	{
		if (type == 0) name = "Chop";
	}

	else if (clss == "Unarmed")
	{
		if (type == 2) name = "Punch";
	}
	else if (clss == "Sword" || clss == "Polearm" || clss == "Knife")
	{
		if (type == 2) name = "Smack";
	}
	else if (clss == "Club")
	{
		if (type == 2) name = "Smash";
	}

	return name;
}


int Weapon::getAttackType(int attackSlot)
{
	if (hasAttackInSlot(attackSlot))
		return wepAttack(attackSlot).type;
	else
		return -1;
}

int Weapon::hasAttackOfType(int attackType)
{
	for (int i = 0; i < numberAttacks(); i++)
	{
		if (wepAttack(i).type == attackType)
			return i;
	}
	return -1;
}

int Weapon::getDamage(bool max, int attackIndex, double multipliers[3])
{
	int damVal = 0;
	int tempDamVal = 0;

	bool allzero = true;

	//loop through possible damage source types. Most weapons will only add damage on one i value
	for (int i = 0; i < 3; i++)
	{
		tempDamVal = 0;

		if (arcData.clss == "Axe")
		{
			int p = 8;
		}


		if (max)
			tempDamVal += _attacks[attackIndex].damMax[i];
		else
			tempDamVal += _attacks[attackIndex].damMin[i];

		if (tempDamVal != 0) allzero = false;
		

		//Sharpness added to slashing and piercing damage. If those values are 0, nothing will be added. If there is damage to add sharpness to.
		if (tempDamVal != 0)
			if (i == 0)
				tempDamVal += getSharpness() + (tempDamVal * ((8.0 * getSharpness()) / 100.0)); //slashing is effected more by sharpness
			else if (i == 1)
				tempDamVal += (tempDamVal * ((4.0 * getSharpness()) / 100.0));

		damVal += (tempDamVal * multipliers[i]); //add in any multipliers (these would be from attacking different target areas
	}
	
	if (allzero == true) return 0;//This means this attack natively has no damage value, so don't bother to continue

	if (damVal <= 0)
		damVal = 1;

	return damVal;
}

int Weapon::randomDamage(int attackSlot, double multipliers[3])
{
	int range = 0;

	if (hasAttackInSlot(attackSlot))
	{
		range = getDamage(true, attackSlot, multipliers) - getDamage(false, attackSlot, multipliers);

		if (range != 0)
			return getDamage(false, attackSlot, multipliers) + (rand() % range);
		else
			return getDamage(false, attackSlot, multipliers);
	}
	else //if invalid slot, just use 1st attack
	{
		range = getDamage(true, 0, multipliers) - getDamage(false, 0, multipliers);
		return getDamage(false, 0, multipliers) + (rand() % range);
	}
}


int Weapon::getAttackModBonus(int attackSlot)
{
	if (attackSlot >= _attacks.size()) //check to see if that slot exists. I don't think this should ever come up
		return 0;

	int size = wepAttack(attackSlot).attackModifiers.size();

	int mod = 0;

	for (int i = 0; i < size; i++)
	{	
		//loop through each modifier
		mod += wepAttack(attackSlot).attackModifiers[i].value;
	}
	return mod;
}
void Weapon::newAttackMod(string name, int attackSlot, int value, int rounds)
{
	if (attackSlot < _attacks.size()) //if that attack exists
	{
		if (_attacks[attackSlot].name != "Feint") //can't add a mod to a feint attack currently may change
		{
			attackMod newMod;
			newMod.rounds = (rounds * 2) + 1;
			newMod.value = value;
			newMod.name = name;

			_attacks[attackSlot].attackModifiers.push_back(newMod);
		}
	}

}
void Weapon::removeAttackMod(int attackSlot, int modSlot)
{
	if (attackSlot < _attacks.size())
	{
		if (modSlot < _attacks[attackSlot].attackModifiers.size()) //if that mod exists
		{
			//delete it
			_attacks[attackSlot].attackModifiers[modSlot] = _attacks[attackSlot].attackModifiers.back();
			_attacks[attackSlot].attackModifiers.pop_back();
		}
	}
}

int Weapon::getParryModBonus()
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
void Weapon::newParryMod(string name, int value, int rounds)
{
	attackMod newMod;
	newMod.rounds = (rounds * 2) + 1;
	newMod.value = value;
	newMod.name = name;

	_parryMods.push_back(newMod);
}
void Weapon::removeParryMod(int modSlot)
{
	if (modSlot < _parryMods.size()) //if that mod exists
	{
		//delete it
		_parryMods[modSlot] = _parryMods.back();
		_parryMods.pop_back();
	}
}


int Weapon::getAttackModBonus()
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
void Weapon::newAttackMod(string name, int value, int rounds)
{
	attackMod newMod;
	newMod.rounds = (rounds * 2) + 1;
	newMod.value = value;
	newMod.name = name;

	_attackMods.push_back(newMod);
}
void Weapon::removeAttackMod(int modSlot)
{
	if (modSlot < _attackMods.size()) //if that mod exists
	{
		//delete it
		_attackMods[modSlot] = _attackMods.back();
		_attackMods.pop_back();
	}
}


void Weapon::cycleModRounds()
{
	//loops through each attack
	for (int i = 0; i < _attacks.size(); i++)
	{
		for (int j = 0; j < _attacks[i].attackModifiers.size(); j++) //loop through each modifier
		{
			_attacks[i].attackModifiers[j].rounds--;

			//if the timer has run out, remove the modifier
			if (_attacks[i].attackModifiers[j].rounds <= 0)
				removeAttackMod(i, j);

			//if a mod is removed, it should change the modifier vector size, decreasing the time the for loop goes.
		}


	}

	//loops through parry mods
	for (int j = 0; j < _parryMods.size(); j++) //loop through each modifier
	{
		_parryMods[j].rounds--;

		//if the timer has run out, remove the modifier
		if (_parryMods[j].rounds <= 0)
			removeParryMod(j);

		//if a mod is removed, it should change the modifier vector size, decreasing the time the for loop goes.
	}

	//loops through weapon mods
	for (int j = 0; j < _attackMods.size(); j++) //loop through each modifier
	{
		_attackMods[j].rounds--;

		//if the timer has run out, remove the modifier
		if (_attackMods[j].rounds <= 0)
			removeAttackMod(j);

		//if a mod is removed, it should change the modifier vector size, decreasing the time the for loop goes.
	}


}

float Weapon::getBalance()
{
	return arcData.balance + (arcData.balance * ((10.0 * _balanceMod) / 100.0));
}

int Weapon::weaponAdvantage(int thisAttacking, Weapon* otherWeapon)
{
	//final advantage value for this weapon based on the other weapon
	int advantage;

	//extra advantage bonuses based on special weapons 
	int extra = 0;

	//default weights for reach and speed. (reach matters more)
	float reachWeight = 10;
	float speedWeight = 4;

	//if this is a shield or attacking into a shield, reach doesn't matter much at all
	if ((!thisAttacking && getClass() == "Shield") || (thisAttacking && otherWeapon->getClass() == "Shield")) //when a shield is defending, reach doesn't matter as much
		reachWeight = 1;

	//if the current weapon is a polearm attacking a fist, fist gets an advantage. The opposite is also true
	if (getClass() == "Polearm" && otherWeapon->getClass() == "Unarmed" && thisAttacking)
		extra = getReach() * -20;	//Attacking unarmed with a polearm gives a penalty based on your reach
	else if (getClass() == "Unarmed" && otherWeapon->getClass() == "Polearm" && !thisAttacking)
		extra = getReach() * 20;	//you get the bonus if defending while unarmed

	//no advantage to punching a spear

	//determing the advantage values based on reach and speed
	int reachAd = ((getReach() - otherWeapon->getReach()) * reachWeight);
	int speedAd = ((getSpeed() - otherWeapon->getSpeed()) * speedWeight);
	
	//defensive advantage
	int defensive = 0;

	//if attacking, defender gets their bonus. If defending, you get it
	if (thisAttacking)
	{
		if (!otherWeapon->isDefenseCanceled()) defensive -= otherWeapon->getDef();
	}
	else
	{
		if (!isDefenseCanceled()) defensive += getDef();
	}
	//add these all up
	advantage = reachAd + speedAd + defensive + extra;

	return advantage; //if returned is positive, the advantage goes tothe current weapon. If negative, advantage goes to the other weapon
}

void Weapon::displayMods()
{
	if (getNumberOfAttackMods() > 0)
	{
		displayName();
		cout << endl;

		//loop through all the specific weapon attack mods
		for (int i = 0; i < getNumberOfAttackMods(); i++)
		{
			cout << _attackMods[i].name << ": ";

			Write::redGreen(_attackMods[i].value);

			cout << "% " << (char)175 << " for " << ((_attackMods[i].rounds  - 1 ) / 2) + 1 << " rounds." << endl;



		}
	}

	
	//loop through each attack and give it's specific bonuses
	for (int i = 0; i < numberAttacks(); i++)
	{
		if (i == 0) cout << endl;

		if (_attacks[i].attackModifiers.size() > 0)
		{
			cout << "    ";
			displayName();

			cout << " " << _attacks[i].name << endl;
			
			//loop through each bonus
			for (int j = 0; j < _attacks[i].attackModifiers.size(); j++)
			{
				cout << "    " << _attacks[i].attackModifiers[j].name << ": ";

				Write::redGreen(_attacks[i].attackModifiers[j].value);

				cout << "% " << (char)175 << " for " << ((_attacks[i].attackModifiers[j].rounds - 1) / 2) + 1 << " rounds." << endl;
				if (j == _attacks[i].attackModifiers.size() - 1) cout << endl;
			}
		}
	}
	

	//loop through parry bonuses

	if (_parryMods.size() > 0)
	{
		cout << "    ";
		displayName();
		cout << " Parry" << endl;

		//loop through all the specific weapon attack mods
		for (int i = 0; i < _parryMods.size(); i++)
		{
			
			cout << "    " << _parryMods[i].name << ": ";

			Write::redGreen(_parryMods[i].value);

			cout << "% " << (char)175 << " for " << ((_parryMods[i].rounds - 1) / 2) + 1 << " rounds." << endl;



		}
	}

}