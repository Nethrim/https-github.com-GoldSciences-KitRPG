#include "Tile.h"
#include "Game.h"
#include "misc.h"

#include "Profession.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "Facility.h"
#include "Vehicle.h"
#include "StageProp.h"

#ifndef __AGENT_HELPER_H__91238648097234698723649872364923874__
#define __AGENT_HELPER_H__91238648097234698723649872364923874__

namespace klib
{
	template <size_t _Width, size_t _Depth>
	void displayEmptySlot(SWeightedDisplay<_Width, _Depth>& display, int32_t offsetY, int32_t offsetX, int32_t agentIndex) {
		static const size_t LINE_SIZE = 35;
		printfToGrid(display.Screen, offsetY, offsetX, LEFT, "-- Agent #%u: %-36.36s", agentIndex, "Open position");
		uint16_t color = COLOR_DARKCYAN;
		valueToGrid(display.TextAttributes, offsetY, offsetX, LEFT, &color, 1, LINE_SIZE);
	}

	template <size_t _Width, size_t _Depth>
	void displayAgentSlot(SWeightedDisplay<_Width, _Depth>& display_, int32_t offsetY, int32_t offsetX, int32_t agentIndex, CCharacter& character, bool bAddFieldNames=false)
	{
		SGrid<char, _Width, _Depth>& display = display_.Screen;

		std::string nameAndLevelText;
		const SEntityPoints agentFinalPoints = calculateFinalPoints( character );
		if( bAddFieldNames ) {
			static const size_t LINE_SIZE = 56;
			uint16_t color = COLOR_GREEN;
			nameAndLevelText = character.Name												;	printfToGrid(display, offsetY	, offsetX, LEFT, "-- Agent #%u: %-34.34s (Lv. %i)", agentIndex			, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			valueToGrid(display_.TextAttributes, offsetY, offsetX, LEFT, &color, 1, LINE_SIZE);

			offsetY += 2;
			nameAndLevelText = getProfessionName	(character.CurrentEquip.Profession		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Class"				, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			nameAndLevelText = getWeaponName		(character.CurrentEquip.Weapon			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Weapon"				, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
			nameAndLevelText = getArmorName			(character.CurrentEquip.Armor			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Armor"				, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
			nameAndLevelText = getAccessoryName		(character.CurrentEquip.Accessory		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Accessory"			, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
			nameAndLevelText = getVehicleName		(character.CurrentEquip.Vehicle			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Vehicle"				, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
			nameAndLevelText = getFacilityName		(character.CurrentEquip.Facility		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Facility"			, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
			nameAndLevelText = getStagePropName		(character.CurrentEquip.StageProp		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Stage prop"			, nameAndLevelText.c_str(), character.CurrentEquip.StageProp	.Level);
			
			nameAndLevelText = "- Final Points:";
			lineToGrid(display, ++offsetY, offsetX, LEFT, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX, LEFT, &(color = COLOR_RED), 1, (int32_t)nameAndLevelText.size());

			offsetY+=2;
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Health				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Health"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Shield				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Shield"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Mana					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Mana"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Hit					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Hit Chance"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Damage					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Damage"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Health	);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "DD. Health"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Shield	);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "DD. Shield"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Mana		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "DD. Mana"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Absorption				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Absorption"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Attack			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Atk. Speed"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Movement			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Mov. Speed"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Reflexes			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Reflexes"	, nameAndLevelText.c_str());

			const SEntityPoints& agentBasePoints = character.Points;
			nameAndLevelText = std::to_string		(agentBasePoints.Coins							);	printfToGrid(display, ++offsetY	, offsetX, LEFT, "%-10.10s: %-11.11s"	, "- Coins"		, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX+11, LEFT, &(color = COLOR_ORANGE), 1, 11);
		}
		else {
			nameAndLevelText = character.Name												;	printfToGrid(display, offsetY++	, offsetX, LEFT, "-- %-21.21s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			nameAndLevelText = getProfessionName	(character.CurrentEquip.Profession		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			nameAndLevelText = getWeaponName		(character.CurrentEquip.Weapon			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
			nameAndLevelText = getArmorName			(character.CurrentEquip.Armor			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
			nameAndLevelText = getAccessoryName		(character.CurrentEquip.Accessory		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
			nameAndLevelText = getVehicleName		(character.CurrentEquip.Vehicle			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
			nameAndLevelText = getFacilityName		(character.CurrentEquip.Facility		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
			nameAndLevelText = getFacilityName		(character.CurrentEquip.Facility		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
			offsetY+=1;
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Health		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Health"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Shield		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Shield"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Mana			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Mana"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Hit			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Hit Chance"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Damage			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Damage"		, nameAndLevelText.c_str());
		}
	}
}

#endif // __AGENT_HELPER_H__91238648097234698723649872364923874__
