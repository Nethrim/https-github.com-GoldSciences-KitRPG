#include "Player.h"

CPlayer::CPlayer(int initHp, int initAtk, int initHit, int initCoins)
{
	setPlayerHp(initHp);
	setPlayerAttack(initAtk);
	setPlayerChit(initHit);
	setPlayerCoins(initCoins);
}

void CPlayer::setPlayerHp(int initHp)
{
	playerHp = initHp;
}
void CPlayer::setPlayerAttack(int initAtk)
{
	playerAtk = initAtk;
}
void CPlayer::setPlayerChit(int initHit)
{
	playerHit = initHit;
}

void CPlayer::setPlayerCoins(int initCoins)
{
	playerCoins = initCoins;
}

int CPlayer::getPlayerHp()
{
	return playerHp;
}

int CPlayer::getPlayerAttack()
{
	return playerAtk;
}

int CPlayer::getPlayerChit()
{
	return playerHit;
}

int CPlayer::getPlayerCoins()
{
	return playerCoins;
}

