#pragma once

#include "Item.h"

#include <vector>

struct SInventorySlot
{
	std::string	Name;
	int			Count;
};

#define MAX_INVENTORY_SLOTS 3

class CPlayer
{
public:
	CPlayer(int initHp, int initAtk, int initHit, int initCoins);
	void setPlayerHp(int);
	void setPlayerAttack(int);
	void setPlayerChit(int);
	void setPlayerCoins(int);
	int getPlayerHp();
	int getPlayerAttack();
	int getPlayerChit();
	int getPlayerCoins();

	SInventorySlot	inventory[MAX_INVENTORY_SLOTS];
	int itemCount;

private:
	int playerAtk, playerHp, playerHit, playerCoins;
};