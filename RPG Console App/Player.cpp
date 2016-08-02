#include "Player.h"

player::player(int initHp, int initAtk, int initHit, int initCoins)
{
	setPlayerHp(initHp);
	setPlayerAttack(initAtk);
	setPlayerChit(initHit);
	setPlayerCoins(initCoins);
}

void player::setPlayerHp(int initHp)
{
	playerHp = initHp;
}
void player::setPlayerAttack(int initAtk)
{
	playerAtk = initAtk;
}
void player::setPlayerChit(int initHit)
{
	playerHit = initHit;
}

void player::setPlayerCoins(int initCoins)
{
	playerCoins = initCoins;
}

int player::getPlayerHp()
{
	return playerHp;
}

int player::getPlayerAttack()
{
	return playerAtk;
}

int player::getPlayerChit()
{
	return playerHit;
}

int player::getPlayerCoins()
{
	return playerCoins;
}

