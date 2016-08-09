#pragma once

#include "Enemy.h"
#include "Player.h"

// This struct holds a counter for every action that we want to record mostly for score purposes.
struct SGameCounters {
	int BattlesWon			= 0;
	int TurnsPlayed			= 0;

	int EnemiesKilled		= 0;
	int DamageDealt			= 0;
	int DamageTaken			= 0;

	int EscapesSucceeded	= 0;
	int EscapesFailed		= 0;

	int MoneyEarned			= 0;
	int MoneySpent			= 0;

	int AttacksHit			= 0;
	int AttacksMissed		= 0;
	int AttacksReceived		= 0;
	int AttacksAvoided		= 0;

	int PotionsUsed			= 0;
	int GrenadesUsed		= 0;
};

class CGame
{
public:
	CPlayer			Adventurer = {CT_PLAYER, 200, 6, 50, 100, "Anonymous"}; //MaxHP, ATk, HitChance, Coins.
	CCharacter		CurrentEnemy;
	SGameCounters	Statisticts;
};

void tavern(CGame& gameInstance);							// Main loop of the game. From there the player can choose to go shopping, fighting or take a nap to recover HP.
void mercenaryJob(CGame& gameInstance);						// Displays the combat difficulty menu from where the player can start combat or go back to the tavern.
void bar(CGame& gameInstance);								// Displays the available items for buying along with the player money.
void displayScore(const CGame& gameInstance);				// Displays the player's character points and statistics.
void combat(CGame& gameInstance, CHARACTER_TYPE enemyType);	// Combat is executed from the mercenary job menu and executes the battle turns until one of the combatants is dead.

