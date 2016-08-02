#pragma once

class player
{
public:
	player(int, int, int, int);
	void setPlayerHp(int);
	void setPlayerAttack(int);
	void setPlayerChit(int);
	void setPlayerCoins(int);
	int getPlayerHp();
	int getPlayerAttack();
	int getPlayerChit();
	int getPlayerCoins();

private:
	int playerAtk, playerHp, playerHit, playerCoins;
};