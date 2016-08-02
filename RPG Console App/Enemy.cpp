#include"Enemy.h"

enemy::enemy(
	int initHp	, 
	int initAtk	, 
	int initHit	, 
	const std::string& name,
	int initDrop
)
	:enemHp		(initHp	)
	,enemAtk	(initAtk)
	,enemHit	(initHit)
	,enemyName	(name	)
	,enemDrop	(initDrop)
{}


