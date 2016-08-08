#pragma once

#include "Item.h"

struct SInventorySlot
{
	std::string	Name;
	int			Count;
};

#define MAX_INVENTORY_SLOTS 3

class CPlayer
{
public:
	CPlayer(int initHp, int initAtk, int initHit, int initCoins)
		: playerHp		(initHp)
		, playerAtk		(initAtk)
		, playerHit		(initHit)
		, playerCoins	(initCoins)
	{};
	inline void	setPlayerAttack	(int value)	{ playerAtk		=	value; };
	inline void	setPlayerHp		(int value)	{ playerHp		=	value; };
	inline void	setPlayerChit	(int value)	{ playerHit		=	value; };
	inline void	setPlayerCoins	(int value)	{ playerCoins	=	value; };
	
	inline int	getPlayerAttack	()			{ return playerAtk; };
	inline int	getPlayerHp		()			{ return playerHp; };
	inline int	getPlayerChit	()			{ return playerHit; };
	inline int	getPlayerCoins	()			{ return playerCoins; };

	SInventorySlot	inventory[MAX_INVENTORY_SLOTS];
	int itemCount;

private:
	int playerHp, 
		playerAtk, 
		playerHit, 
		playerCoins;
};