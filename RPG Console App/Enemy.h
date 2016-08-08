#pragma once

#include <string>

enum ENEMY_TYPE
{	WOLF
,	RAIDER
,	SOLDIER
,	UNKNOWN
};

class CEnemy
{
	public:
		CEnemy(){};
		CEnemy(
			ENEMY_TYPE	enemyType_,
			int initHp	, 
			int initAtk	, 
			int initHit	, 
			const std::string& name,
			int initDrop
		)
			:enemyType	(enemyType_)
			,enemHp		(initHp	)
			,enemAtk	(initAtk)
			,enemHit	(initHit)
			,enemyName	(name	)
			,enemDrop	(initDrop)
		{};
		inline	void setEnemHp			(int val)					{ enemHp	= val; };
		inline	void setEnemAttack		(int val)					{ enemAtk	= val; };
		inline	void setEnemChit		(int val)					{ enemHit	= val; };
		inline	void setEnemName		(const std::string& val)	{ enemyName	= val; };
		inline  void setEnemDrop		(int val)					{ enemDrop  = val; };
		inline	int getEnemHp			() { return enemHp; };
		inline	int getEnemAttack		() { return enemAtk; };		
		inline	int getEnemChit			() { return enemHit; };		
		inline	std::string getEnemName	() { return enemyName; };
		inline int getEnemDrop() { return enemDrop; };
				
	private:
		ENEMY_TYPE enemyType;
		int 
			enemHp	, 
			enemAtk	, 
			enemDrop,
			enemHit	;
			std::string enemyName;
};

static CEnemy getEnemyDefinition(ENEMY_TYPE enemyType)
{
	static const CEnemy wolf		(WOLF, 100,  5, 70,   "Wolf", 25); //HP, Atk, HitChance, name, dropCoins.
	static const CEnemy raider		(RAIDER, 150,  10, 50,  "Raider", 50); //HP, Atk, HitChance, name, dropCoins.
	static const CEnemy soldier		(SOLDIER, 250,  8, 80,	"Soldier",100); //HP, Atk, HitChance, name, dropCoins.

	switch(enemyType) 
	{
	case WOLF:		return wolf;	
	case RAIDER:	return raider;	
	case SOLDIER:	return soldier; 
	default:
		return CEnemy(UNKNOWN, 250+(rand()%50), 8+(rand()%5), 80+(rand()%20), "Reptile (a character that spawns due to a bug).", 100+(rand()%20));
	}
}