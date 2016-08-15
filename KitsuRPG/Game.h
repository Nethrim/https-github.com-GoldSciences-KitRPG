#pragma once

#include "Enemy.h"

class CGame
{
public:
								//CharacterType			MaxHP	HitChance,	ATk, Coins,		Name.
	CCharacter		Adventurer; // = {CHARACTER_TYPE_PLAYER,	10,		50,		1,		100,	"Anonymous"}; 
};

void tavern(CCharacter& adventurer);							// Main loop of the game. From there the player can choose to go shopping, fighting or take a nap to recover HP.
void mercenaryJob(CCharacter& adventurer);						// Displays the combat difficulty menu from where the player can start combat or go back to the tavern.
void bar(CCharacter& adventurer);								// Displays the available items for buying along with the player money.
void displayScore(const CCharacter& adventurer);				// Displays the player's character points and statistics.
void combat(CCharacter& adventurer, uint32_t enemyType);	// Combat is executed from the mercenary job menu and executes the battle turns until one of the combatants is dead.
