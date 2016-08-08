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
		int initDrop,
		const std::string& name
	)
		:enemyType	(enemyType_)
		,enemHp		(initHp	)
		,enemAtk	(initAtk)
		,enemHit	(initHit)
		,enemDrop	(initDrop)
		,enemyName	(name	)
	{};
	inline void	setEnemHp			(int val)					{ enemHp	= val; };
	inline void	setEnemAttack		(int val)					{ enemAtk	= val; };
	inline void	setEnemChit			(int val)					{ enemHit	= val; };
	inline void	setEnemDrop			(int val)					{ enemDrop  = val; };
	inline void	setEnemName			(const std::string& val)	{ enemyName	= val; };
		
	inline int	getEnemHp			() const { return enemHp; };
	inline int	getEnemAttack		() const { return enemAtk; };		
	inline int	getEnemChit			() const { return enemHit; };		
	inline int	getEnemDrop			() const { return enemDrop; };
	inline std::string getEnemName	() const { return enemyName; };
				
private:
	ENEMY_TYPE enemyType;
	int 
		enemHp	, 
		enemAtk	, 
		enemHit	,
		enemDrop;
	std::string enemyName;
};

static CEnemy getEnemyDefinition(ENEMY_TYPE enemyType)
{
	static const CEnemy wolf		(WOLF,		100,	5,	70,	25,		"Wolf"		); //type, HP, Atk, HitChance, dropCoins, name.
	static const CEnemy raider		(RAIDER,	150,	10, 50,	50,		"Raider"	); //type, HP, Atk, HitChance, dropCoins, name.
	static const CEnemy soldier		(SOLDIER,	250,	8,	80,	100,	"Soldier"	); //type, HP, Atk, HitChance, dropCoins, name.

	switch(enemyType) 
	{
	case WOLF:		return wolf;	
	case RAIDER:	return raider;	
	case SOLDIER:	return soldier; 
	default:
		return CEnemy(UNKNOWN, 250+(rand()%50), 8+(rand()%5), 80+(rand()%20), 100+(rand()%20), "Reptile (a character that spawns due to a bug).");
	}
}