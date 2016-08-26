#include "Entity.h"
#include "EntityPoints.h"

#include "CharacterPoints.h"

#ifndef __GAMEBASE_H__98236498027346892734689273__
#define __GAMEBASE_H__98236498027346892734689273__

namespace klib
{
#pragma pack(push, 1)
	struct SCharacterEquip
	{
		SWeapon					Weapon		= {0,0,1};	// Index, ModifierIndex, Level
		SArmor					Armor		= {0,0,1};	// Index, ModifierIndex, Level
		SProfession				Profession	= {0,0,1};	// Index, ModifierIndex, Level
		SVehicle				Vehicle		= {0,0,1};	// Index, ModifierIndex, Level
	};

	struct SCharacterResearch
	{
		SResearchGroup			Weapon		= {};
		SResearchGroup			Armor		= {};
		SResearchGroup			Profession	= {};
		SResearchGroup			Vehicle		= {};
	};

	// We don't need to create complex classes from SItem
	class CArmor;
	class CWeapon;
	class CProfession;
	class CVehicle;
	class CStageProp;

	typedef SInventoryItems											SCharacterItems			;
	typedef SEntityContainer<SWeapon*		, MAX_INVENTORY_SLOTS>	SCharacterWeapons		;
	typedef SEntityContainer<SArmor*		, MAX_INVENTORY_SLOTS>	SCharacterArmors		;
	typedef SEntityContainer<SProfession*	, MAX_INVENTORY_SLOTS>	SCharacterProfessions	;
	typedef SEntityContainer<SVehicle*		, MAX_INVENTORY_SLOTS>	SCharacterVehicles		;
	typedef SEntityContainer<SStageProp*	, MAX_INVENTORY_SLOTS>	SCharacterStageProps	;

	struct SCharacterInventory
	{
		SCharacterItems			Items		= {};
		SInventoryWeapons		Weapons		= {};
		SInventoryArmors		Armors		= {};
		SInventoryProfessions	Professions	= {};
		SInventoryVehicles		Vehicles	= {};
	};

	struct SCharacterTurnBonus
	{
		SCombatBonus			Points			= SCombatBonus	();
		SCombatStatus			Status			= SCombatStatus	();
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

		int32_t					Save(FILE* fp)	const;
		int32_t					Load(FILE* fp);

		void					UnequipWeapon		()	{	unequipEntity(Inventory.Weapons		, CurrentEquip.Weapon		);	};
		void					UnequipArmor		()	{	unequipEntity(Inventory.Armors		, CurrentEquip.Armor		);	};
		void					UnequipProfession	()	{	unequipEntity(Inventory.Professions	, CurrentEquip.Profession	);	};
		void					UnequipVehicle		()	{	unequipEntity(Inventory.Vehicles	, CurrentEquip.Vehicle		);	};

		void					EquipWeapon			(size_t inventorySlotIndex);
		void					EquipArmor			(size_t inventorySlotIndex);
		void					EquipProfession		(size_t inventorySlotIndex);
		void					EquipVehicle		(size_t inventorySlotIndex);

		bool					DidLoseTurn			() {
			return 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_STUN		) 
				|| 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_SLEEP	) 
				|| 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_FROZEN	);
		};
	};

	SEntityPoints	calculateFinalPoints			(const SCharacter& character);
	SEntityFlags	calculateFinalFlags				(const SCharacter& character);
	void			rest							(SCharacter& adventurerPoints);	// Take a nap and recover Life Points up to Max Value.

	class CCharacter : public SCharacter
	{
	public:
		std::string				Name			= "Unnamed";

		CCharacter() = default;
		CCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SEntityEffect characterEffect, SEntityStatus characterStatus, const std::string& name)
			:SCharacter	(characterType, maxHP, hitChance, attack, coins, characterEffect, characterStatus)
			,Name		(name)
			{};

		int		Save(FILE* fp)	const;
		int		Load(FILE* fp);
	};

#pragma pack(pop)

} // namespace

#endif // __GAMEBASE_H__98236498027346892734689273__
