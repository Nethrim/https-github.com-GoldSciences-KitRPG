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
		SWeapon			Weapon		;	// Index, ModifierIndex, Level
		SArmor			Armor		;	// Index, ModifierIndex, Level
		SProfession		Profession	;	// Index, ModifierIndex, Level
		SVehicle		Vehicle		;	// Index, ModifierIndex, Level

		inline constexpr SCharacterEquip
			(	SWeapon			weapon		= {0,0,1}
			,	SArmor			armor		= {0,0,1}
			,	SProfession		profession	= {0,0,1}
			,	SVehicle		vehicle		= {0,0,1}
			)
			:Weapon		(weapon		)
			,Armor		(armor		)
			,Profession	(profession	)
			,Vehicle	(vehicle	)
		{};
	};

	struct SCharacterResearch
	{
		SResearchGroup				Weapon		= {};
		SResearchGroup				Armor		= {};
		SResearchGroup				Profession	= {};
		SResearchGroup				Vehicle		= {};
	};

	struct SCharacterInventory
	{
		SInventoryItems				Items		= {};
		SInventoryWeapons			Weapons		= {};
		SInventoryArmors			Armors		= {};
		SInventoryProfessions		Professions	= {};
		SInventoryVehicles			Vehicles	= {};
	};

	struct SCharacterTurnBonus
	{
		SCombatBonus				Points				= SCombatBonus		();
		SCombatStatus				Status				= SCombatStatus		();
	};

	struct SCharacter
	{
		CHARACTER_TYPE				Type				= CHARACTER_TYPE_UNKNOWN;
		
		SEntityPoints				Points				= {};	
		SEntityFlags				Flags				= {};	

		SCharacterTurnBonus			ActiveBonus			= {};

		SCharacterScore				Score				= SCharacterScore();	

		SCharacterEquip				CurrentEquip		= {{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
		SCharacterEquip				MaxEquip			= {{0,0,1},{0,0,1},{0,0,1},{0,0,1}};	// Max equip grades researched/available.

		SCharacterInventory			Inventory			= {};
		SCharacterResearch			Researched			= {};

		constexpr SCharacter() = default;
		constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SEntityEffect characterEffect, SEntityStatus characterStatus ) 
			:Type				(characterType)
			,Points				({{maxHP	}, {maxHP}, {hitChance, attack}, coins})
			,Flags				({characterEffect, characterStatus})
			,ActiveBonus		(SCharacterTurnBonus	())
			,Score				(SCharacterScore		())
			,CurrentEquip		({{0,0,1},{0,0,1},{0,0,1},{0,0,1}})
			,MaxEquip			({{0,0,1},{0,0,1},{0,0,1},{0,0,1}})
			,Inventory			({})
			,Researched			({})
		{};

		int32_t						Save(FILE* fp)	const;
		int32_t						Load(FILE* fp);

		void						UnequipWeapon		()	{	unequipEntity(Inventory.Weapons		, CurrentEquip.Weapon		);	};
		void						UnequipArmor		()	{	unequipEntity(Inventory.Armors		, CurrentEquip.Armor		);	};
		void						UnequipProfession	()	{	unequipEntity(Inventory.Professions	, CurrentEquip.Profession	);	};
		void						UnequipVehicle		()	{	unequipEntity(Inventory.Vehicles	, CurrentEquip.Vehicle		);	};

		void						EquipWeapon			(size_t slotIndex);
		void						EquipArmor			(size_t slotIndex);
		void						EquipProfession		(size_t slotIndex);
		void						EquipVehicle		(size_t slotIndex);

		bool						DidLoseTurn() {
			return 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_STUN		) 
				|| 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_SLEEP	) 
				|| 0 < ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_FROZEN	);
		};
	};

	SEntityPoints	calculateFinalPoints	(const SCharacter& character);
	SEntityFlags	calculateFinalFlags		(const SCharacter& character);
	void			rest					(SCharacter& adventurerPoints);	// Take a nap and recover Life Points up to Max Value.
#pragma pack(pop)

} // namespace

#endif // __GAMEBASE_H__98236498027346892734689273__
