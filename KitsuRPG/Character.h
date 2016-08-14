#pragma once

#include "Item.h"
#include "Weapon.h"
#include "Misc.h"
#include <algorithm>

#pragma pack(push, 1)

// This struct holds a counter for every action that we want to record mostly for score purposes.
struct SCharacterScore 
{
	uint64_t BattlesWon			= 0;
	uint64_t TurnsPlayed		= 0;

	uint64_t EnemiesKilled		= 0;
	uint64_t DamageDealt		= 0;
	uint64_t DamageTaken		= 0;

	uint64_t EscapesSucceeded	= 0;
	uint64_t EscapesFailed		= 0;

	uint64_t MoneyEarned		= 0;
	uint64_t MoneySpent			= 0;

	uint64_t AttacksHit			= 0;
	uint64_t AttacksMissed		= 0;
	uint64_t AttacksReceived	= 0;
	uint64_t AttacksAvoided		= 0;

	uint64_t PotionsUsed		= 0;
	uint64_t GrenadesUsed		= 0;
};

struct SInventorySlot
{
	uint32_t	ItemIndex;
	uint32_t	ItemCount;
};

enum CHARACTER_TYPE : uint32_t
{	CHARACTER_TYPE_UNKNOWN
,	CHARACTER_TYPE_NPC
,	CHARACTER_TYPE_PLAYER
,	CHARACTER_TYPE_ENEMY
,	CHARACTER_TYPE_ALLY
};

typedef SCharacterPoints SBonusTurns;

struct SCombatBonus
{
	SCharacterPoints	Points		= {0, 0, 0, 0, 0};	// these are points that are calculated during combat depending on equipment or item consumption.
	SBonusTurns			TurnsLeft	= {0, 0, 0, 0, 0};	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

	void				NextTurn() {
		if( 0 >= --TurnsLeft.MaxHP	)	TurnsLeft.MaxHP		=	Points.MaxHP	= 0;
		if( 0 >= --TurnsLeft.HP		)	TurnsLeft.HP		=	Points.HP		= 0;
		if( 0 >= --TurnsLeft.Attack	)	TurnsLeft.Attack	=	Points.Attack	= 0;
		if( 0 >= --TurnsLeft.Hit	)	TurnsLeft.Hit		=	Points.Hit		= 0;
		if( 0 >= --TurnsLeft.Coins	)	TurnsLeft.Coins		=	Points.Coins	= 0;
	};
};

#define MAX_COMBAT_STATUS	32

struct SCombatStatus
{
	STATUS_TYPE			Status	[MAX_COMBAT_STATUS]	= {};
	int					Counter	[MAX_COMBAT_STATUS]	= {};
};

#define MAX_INVENTORY_SLOTS 9

struct SCharacterInventory
{
	uint32_t			ItemCount					= 0;
	SInventorySlot		Slots[MAX_INVENTORY_SLOTS]	= {};
};

// POD to store all the character data that is not a string or object.
struct SCharacter
{
	CHARACTER_TYPE		Type			= CHARACTER_TYPE_UNKNOWN;
	SCharacterPoints	Points			= {10, 10, 50, 1, 10};	// These are the base character points.
	SCombatBonus		CombatBonus		= {};
	SCombatStatus		CombatStatus	= {};
	SCharacterInventory	Inventory		= {};
	SCharacterScore		Score			= {};	
	uint32_t			Weapon			= 0;

	constexpr SCharacter() = default;
	constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins) 
		:Type			(characterType)
		,Points			({maxHP, maxHP, hitChance, attack, coins})
		,CombatBonus	({})
		,CombatStatus	({})
		,Inventory		({})
		,Score			({})
		,Weapon			(0)
	{};

	int		Save(FILE* fp)	const
	{	
		if(0 == fp) {	
			printf("Cannot save to file!"	); 
			return -1; 
		}	
		if( 1 != fwrite	(this, sizeof(SCharacter), 1, fp) )				
		{ 
			printf( "Cannot save to file!"	); 
			return -1; 
		}; 
		return 0; 
	};

	int		Load(FILE* fp)	{	
		if(0 == fp) {	
			printf("Cannot load from file!"	); 
			return -1; 
		} 
		
		SCharacter loadedCharacter; 
		if( 1 != fread	(&loadedCharacter, sizeof(SCharacter), 1, fp) ) { 
			printf( "Cannot load from file!"	); 
			return -1; 
		}; 
		
		*this = loadedCharacter; 
		return 0; 
	};
};

#pragma pack(pop)

class CCharacter : public SCharacter
{
public:
	std::string			Name			= "Unnamed";

	CCharacter() = default;
	CCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, const std::string& name)
		:SCharacter	(characterType, maxHP, hitChance, attack, coins)
		,Name		(name)
		{};

	int		Save(FILE* fp)	const 
	{	
		SCharacter::Save(fp);
		uint8_t nameLen = (uint8_t)Name.size();
		if( 1 != fwrite(&nameLen, 1, 1, fp) )
		{
			printf("Failed to save character data!");
			return -1;
		}
		if(nameLen)
			if( 1 != fwrite(Name.c_str(), 1, nameLen, fp) )
			{
				printf("Failed to load character name!");
				return -1;
			}
		return 0; 
	};

	int		Load(FILE* fp)	{	
		SCharacter::Load(fp); 
		uint8_t nameLen = 0;
		if( 1 != fread(&nameLen, 1, 1, fp) )
		{
			printf("Failed to load character data!");
			return -1;
		}
		char name[128] = {0,};
		if(nameLen)
			if( 1 != fread(name, 1, std::min(nameLen, (uint8_t)127), fp) )
			{
				printf("Failed to load character name!");
				return -1;
			}
		Name = name;
		return 0; 
	};
};



static SCharacterPoints calculateFinalPoints(const SCharacterPoints& basePoints, const SCharacterPoints& weaponBonusPoints, const SCharacterPoints& turnBonusPoints)
{
	SCharacterPoints result;

	result.MaxHP	= basePoints.MaxHP			+	weaponBonusPoints.MaxHP		+turnBonusPoints.MaxHP	;
	result.HP		= weaponBonusPoints.HP		+	turnBonusPoints.HP;
	result.Hit		= basePoints.Hit			+	weaponBonusPoints.Hit		+turnBonusPoints.Hit	;
	result.Attack	= basePoints.Attack			+	weaponBonusPoints.Attack	+turnBonusPoints.Attack	;
	result.Coins	= weaponBonusPoints.Coins	+	turnBonusPoints.Coins;

	return result;
};

void rest(SCharacterPoints& adventurerPoints);	// Take a nap and recover HP up to MaxHP.
bool addItem(SCharacterInventory& adventurer, uint32_t itemIndex);
void showInventory(const SCharacter& adventurer);
