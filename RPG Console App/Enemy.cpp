#include"Enemy.h"

CEnemy::CEnemy(
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
{}


