#pragma once

#include "Character.h"
#include "Write.h"

const int width = 60;
class Duel
{
public:
	Duel();
	~Duel();

	int run(Character *p1, Character *p2);

	void display(bool justTop);
	void healthBars();
	void reaction();
	void posture();

	bool getAdvanced() { return _advanced; }
	void cycleAdvanced() { _advanced = (_advanced + 1) % 2; }

	void displayAttackOptions();
	void displayDefenseOptions();

	void nextRound();

	void characterClash();

	//bool setAttackWepIndex(char attackerInput); //sets the attack and wep index for the attacker -1 if invalid attack, 0-3 for first weapons attacks, 4-7 for second weapons attacks, 9 for nothing    this is inteneded to be the index of the weapon attack, not the attack type. Also sets the wepUsed to weapon index, or -1 if doing nothing
	//bool setDefenseNumber(char defenderInput); //returns false if invalid defense, 0-1 for first weapon parry or counterattack, 2-3 for second weapon, 4 for dodge, and 9 for nothing

	bool setupCharacterOptions(bool attacking, char charInput, Character* attacker, Character* defender); //takes a char input and sets up the attackers attacks to it

	
	//string getMove(bool attacker, int playerNum);

	bool attacking;

private:
	Character *player1;
	Character *player2;

	//vector<string>

	int move;

	int advantage; //1 if player 1, 2 if player 2

	Character * attPlayer;
	Character * defPlayer;

	Character* gofirstplayer;
	Character* gosecondPlayer;


	Character* players[2];

	void setAdvantage();

	bool changePosture(Character* character, int num);
	void changeTargetArea(Character* character, Character* other, int num);

	int round;

	int phase;

	char attOp;
	char defOp;

	int _attackerWeaponIndex;		//-1 for doing nothing, 0 for weapon 1, 1 for weapon 2
	int _attackerAttackIndex;		//-1 for nothing, 0-3 for different attacks

	int _defenderWeaponIndex;
	int _defenderDefenseIndex;

	bool _postureLock;
	bool _advanced; //if true displays more advanced data

	bool success(int desired, int opposing, int& roll, int &roll2);

	//string attackChoice(Character* attacker);
	//string defendChoice();

	string attackBreakdown(bool attack, int type, int wepSlot, int attackNum, int advantage, Character::targetArea* target); //returns a string with the breakdown of the attack value
	string defenseBreakdown(bool attack, int type, int wepSlot, int attackNum, int advantage);

	int damageChar(Character* characterAttacking, Character* characterDamaged, int attackType, int padding); //returns damage done
	int damageChar(Character* characterDamaged, int damage, int padding);


	void determineModifiers(bool display, Character* attacker, Character* defender, bool success, int displayPadding);

};

