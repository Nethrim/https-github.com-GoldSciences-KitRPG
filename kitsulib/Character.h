#include "Entity.h"
#include "EntityPoints.h"

#include "CharacterPoints.h"

#include <vector>

#ifndef __GAMEBASE_H__98236498027346892734689273__
#define __GAMEBASE_H__98236498027346892734689273__

namespace klib
{
#pragma pack(push, 1)
	struct SCharacterEquip
	{
		SWeapon					Weapon		= {0,0,1};	// Index, ModifierIndex, Level
		SAccessory				Accessory	= {0,0,1};	// Index, ModifierIndex, Level
		SProfession				Profession	= {0,0,1};	// Index, ModifierIndex, Level
		SArmor					Armor		= {0,0,1};	// Index, ModifierIndex, Level
		SVehicle				Vehicle		= {0,0,1};	// Index, ModifierIndex, Level
		SFacility				Facility	= {0,0,1};	// Index, ModifierIndex, Level
		SStageProp				StageProp	= {0,0,1};	// Index, ModifierIndex, Level
	};

	struct SCharacterResearch
	{
		SResearchGroup<SWeapon		>	Weapon		= {};
		SResearchGroup<SAccessory	>	Accessory	= {};
		SResearchGroup<SProfession	>	Profession	= {};
		SResearchGroup<SArmor		>	Armor		= {};
		SResearchGroup<SVehicle		>	Vehicle		= {};
		SResearchGroup<SFacility	>	Facility	= {};
		SResearchGroup<SStageProp	>	StageProp	= {};
	};

	struct SCharacterInventory
	{
		SInventoryProfession	Profession		= {};
		SInventoryAccessory		Accessory		= {};
		SInventoryArmor			Armor			= {};
		SInventoryWeapon		Weapon			= {};
		SInventoryVehicle		Vehicle			= {};
		SInventoryFacility		Facility		= {};
		SInventoryStageProp		StageProp		= {};
		SInventoryItems			Items			= {};
	};

	struct SCharacterTurnBonus
	{
		SCombatBonus			Points			= SCombatBonus	();
		SCombatStatus			Status			= SCombatStatus	();
	};


	template<typename _EntityType, CHARACTER_TYPE _CharacterType>
	struct SEquip
	{
		CHARACTER_TYPE			Type			= _CharacterType;
		_EntityType				Entity			= {0,0,1};

		SEntityPoints			Points			= SEntityPoints();	
		SEntityFlags			Flags			= SEntityFlags();	
		SCharacterTurnBonus		ActiveBonus		= SCharacterTurnBonus();

		SCharacterInventory		Inventory		= SCharacterInventory();
	};

	//template<typename _EntityType>
	struct SCharacter	//: public SEquip<_EntityType>
	{
		CHARACTER_TYPE			Type			= CHARACTER_TYPE_UNKNOWN;
		//_EntityType				Entity			= {0,0,1};

		SEntityPoints			Points			= SEntityPoints();	
		SEntityFlags			Flags			= SEntityFlags();	

		SCharacterTurnBonus		ActiveBonus		= SCharacterTurnBonus();

		SCharacterScore			Score			= SCharacterScore();	

		SCharacterEquip			CurrentEquip	= SCharacterEquip();
		SCharacterEquip			MaxEquip		= SCharacterEquip();	// Max equip grades researched/available.
		SCharacterInventory		Inventory		= SCharacterInventory();

		SCharacterResearch		Researched		= SCharacterResearch();

		constexpr SCharacter() = default;
		constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SSpeedPoints speed, SEntityEffect characterEffect, SEntityStatus characterStatus ) 
			:Type				(characterType)
			,Points				({{maxHP}, {maxHP}, {hitChance, attack, {}, 0, speed}, coins, coins, coins/10})
			,Flags				({characterEffect, characterStatus})
			,ActiveBonus		(SCharacterTurnBonus	())
			,Score				(SCharacterScore		())
			,CurrentEquip		(SCharacterEquip		())
			,MaxEquip			(SCharacterEquip		())
			,Inventory			(SCharacterInventory	())
			,Researched			(SCharacterResearch		())
		{};

		bool					DidLoseTurn			() {
			return 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_SLEEP	) 
				|| 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_STUN		) 
				|| 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_PETRIFY	) 
				|| 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_FROZEN	);
		};
	};

	SEntityPoints				calculateFinalPoints			(const SCharacter& character);
	SEntityFlags				calculateFinalFlags				(const SCharacter& character);

	static inline void			rest(SCharacter& character) {
		const SEntityPoints finalPoints	= calculateFinalPoints(character);
		character.Points.LifeCurrent	= finalPoints.LifeMax	;
	}

	struct SCellCoord
	{
		int32_t x;
		int32_t y;
		int32_t z;

		constexpr bool			operator == (const SCellCoord& other) const { return x == other.x && y == other.y && z == other.z; }
		inline constexpr bool	operator != (const SCellCoord& other) const { return !operator==(other); }
	};

	class CCharacter : public SCharacter
	{
	public:
		SCellCoord				Position	= {};
		std::string				Name		= "Unnamed";

		CCharacter() = default;
		CCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SSpeedPoints speed, SEntityEffect characterEffect, SEntityStatus characterStatus, const std::string& name)
			:SCharacter	(characterType, maxHP, hitChance, attack, coins, speed, characterEffect, characterStatus)
			,Position	({0,0})
			,Name		(name)
			{};
	};

	class CCharacterManager
	{
		const CCharacter*	m_WorldCharacters[256] = {0};
		uint32_t			Count = 0;
	public:
		~CCharacterManager() {
			for( uint32_t i=0; i<Count; ++i)
				delete(m_WorldCharacters[i]);
		};

		void CreateCharacter(CCharacter*& createdCharacter)  {
			if(createdCharacter = new CCharacter())
				m_WorldCharacters[Count++] = createdCharacter;
		};
	};
#pragma pack(pop)

} // namespace

#endif // __GAMEBASE_H__98236498027346892734689273__
