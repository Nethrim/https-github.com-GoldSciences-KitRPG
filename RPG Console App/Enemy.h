	#pragma once

#include <string>

class CEnemy
{
	public:
		CEnemy(){};
		CEnemy(int hp, int atk, int hit, const std::string& name, int drop);
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
		int 
			enemHp	, 
			enemAtk	, 
			enemDrop,
			enemHit	;
			std::string enemyName;
			
};