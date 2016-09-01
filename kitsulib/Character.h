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
	};

	struct SCharacterResearch
	{
		SResearchGroup			Weapon		= {};
		SResearchGroup			Accessory	= {};
		SResearchGroup			Profession	= {};
		SResearchGroup			Armor		= {};
		SResearchGroup			Vehicle		= {};
		SResearchGroup			Facility	= {};
		SResearchGroup			StageProp	= {};
	};

	// We don't need to create complex classes from SItem
	class CWeapon;
	class CAccessory;
	class CArmor;
	class CProfession;
	class CVehicle;
	class CFacility;
	class CStageProp;

	typedef SEntityContainer<SWeapon*		, MAX_INVENTORY_SLOTS>	SCharacterWeapons		;
	typedef SEntityContainer<SAccessory*	, MAX_INVENTORY_SLOTS>	SCharacterAccessories	;
	typedef SEntityContainer<SArmor*		, MAX_INVENTORY_SLOTS>	SCharacterArmors		;
	typedef SEntityContainer<SProfession*	, MAX_INVENTORY_SLOTS>	SCharacterProfessions	;
	typedef SEntityContainer<SVehicle*		, MAX_INVENTORY_SLOTS>	SCharacterVehicles		;
	typedef SEntityContainer<SStageProp*	, MAX_INVENTORY_SLOTS>	SCharacterStageProps	;
	typedef SEntityContainer<SFacility*		, MAX_INVENTORY_SLOTS>	SCharacterFacilities	;
	typedef SInventoryItems											SCharacterItems			;

	struct SCharacterInventory
	{
		SInventoryProfessions	Professions	= {};
		SInventoryAccessories	Accessories	= {};
		SInventoryArmors		Armors		= {};
		SInventoryWeapons		Weapons		= {};
		SInventoryVehicles		Vehicles	= {};
		SInventoryFacilities	Facilities	= {};
		SInventoryStageProps	StageProps	= {};
		SInventoryItems			Items		= {};
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
		constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SEntityEffect characterEffect, SEntityStatus characterStatus ) 
			:Type				(characterType)
			,Points				({{maxHP}, {maxHP}, {hitChance, attack}, coins})
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
	void						rest							(SCharacter& adventurerPoints);	// Take a nap and recover Life Points up to Max Value.


	struct SCellCoord
	{
		int32_t x;
		int32_t z;
	};

	class CCharacter : public SCharacter
	{
	public:
		SCellCoord				Position	= {};
		std::string				Name		= "Unnamed";

		CCharacter() = default;
		CCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SEntityEffect characterEffect, SEntityStatus characterStatus, const std::string& name)
			:SCharacter	(characterType, maxHP, hitChance, attack, coins, characterEffect, characterStatus)
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
