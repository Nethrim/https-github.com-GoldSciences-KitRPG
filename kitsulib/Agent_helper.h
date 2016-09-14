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
		static const size_t LINE_SIZE = 30;
		printfToGrid(display.Screen, offsetY, offsetX, LEFT, "-- Agent #%u: %-14.14s", agentIndex, "Open position");
		uint16_t color = COLOR_GREEN;
		valueToGrid(display.TextAttributes, offsetY, offsetX, LEFT, &color, 1, 12);
		color = COLOR_DARKCYAN;
		valueToGrid(display.TextAttributes, offsetY, offsetX+13, LEFT, &color, 1, LINE_SIZE-14);
	}

	template <size_t _Width, size_t _Depth>
	void displayAgentSlot(SWeightedDisplay<_Width, _Depth>& display_, int32_t offsetY, int32_t offsetX, int32_t agentIndex, CCharacter& character, bool bAddFieldNames=false)
	{
		SGrid<char, _Width, _Depth>& display = display_.Screen;

		std::string nameAndLevelText;
		const SEntityPoints agentFinalPoints = calculateFinalPoints( character );
		uint16_t color = COLOR_GREEN;
		if( bAddFieldNames ) {
			static const size_t LINE_SIZE = 56;
			nameAndLevelText = character.Name												;	printfToGrid(display, offsetY	, offsetX, LEFT, "-- Agent #%u:" " %-34.34s Lv. %i", agentIndex			, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			valueToGrid(display_.TextAttributes, offsetY, offsetX, LEFT, &color, 1, 12);
			//color = COLOR_GREEN;
			valueToGrid(display_.TextAttributes, offsetY, offsetX+13, LEFT, &color, 1, LINE_SIZE-14);

			offsetY += 2;
			nameAndLevelText = getProfessionName	(character.CurrentEquip.Profession		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s Lv. %i"	, "Class"				, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			nameAndLevelText = getWeaponName		(character.CurrentEquip.Weapon			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s Lv. %i"	, "Weapon"				, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
			nameAndLevelText = getArmorName			(character.CurrentEquip.Armor			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s Lv. %i"	, "Armor"				, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
			nameAndLevelText = getAccessoryName		(character.CurrentEquip.Accessory		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s Lv. %i"	, "Accessory"			, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
			nameAndLevelText = getVehicleName		(character.CurrentEquip.Vehicle			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s Lv. %i"	, "Vehicle"				, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
			nameAndLevelText = getFacilityName		(character.CurrentEquip.Facility		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s Lv. %i"	, "Facility"			, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
			nameAndLevelText = getStagePropName		(character.CurrentEquip.StageProp		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s Lv. %i"	, "Stage prop"			, nameAndLevelText.c_str(), character.CurrentEquip.StageProp	.Level);
			
			nameAndLevelText = "- Final Points:";
			lineToGrid(display, ++offsetY, offsetX, LEFT, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX, LEFT, &(color = COLOR_RED), 1, (int32_t)nameAndLevelText.size());

			offsetY+=2;
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Health				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Health"					, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Shield				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Shield"					, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Mana					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Mana"					, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Hit					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Hit Chance"				, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Damage					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Damage"					, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Health	);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Health"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Shield	);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Shield"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Mana		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Mana"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Absorption				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Absorption"				, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Attack			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Attack Speed"			, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Movement			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Movement Speed"			, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Reflexes			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Reflexes"				, nameAndLevelText.c_str());

			const SEntityPoints& agentBasePoints = character.Points;
			nameAndLevelText = std::to_string		(agentBasePoints.Coins							);	printfToGrid(display, ++offsetY	, offsetX, LEFT, "%-21.21s: %-11.11s"	, "- Coins"		, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX+23, LEFT, &(color = COLOR_ORANGE), 1, 11);
		}
		else 
		{
			nameAndLevelText = character.Name												;	printfToGrid(display, offsetY, offsetX, LEFT, "-- %-31.31s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			valueToGrid(display_.TextAttributes, offsetY, offsetX, LEFT, &color, 1, 40);
			offsetY+=2;
			nameAndLevelText = getProfessionName	(character.CurrentEquip.Profession		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
			nameAndLevelText = getWeaponName		(character.CurrentEquip.Weapon			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
			nameAndLevelText = getArmorName			(character.CurrentEquip.Armor			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
			nameAndLevelText = getAccessoryName		(character.CurrentEquip.Accessory		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
			nameAndLevelText = getVehicleName		(character.CurrentEquip.Vehicle			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
			nameAndLevelText = getFacilityName		(character.CurrentEquip.Facility		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
			nameAndLevelText = getStagePropName		(character.CurrentEquip.StageProp		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s Lv. %i"	, nameAndLevelText.c_str(), character.CurrentEquip.StageProp	.Level);

			nameAndLevelText = "- Final Points:";
			lineToGrid(display, ++offsetY, offsetX, LEFT, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX, LEFT, &(color = COLOR_RED), 1, (int32_t)nameAndLevelText.size());

			offsetY+=2;
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Health				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Health"					, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Shield				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Shield"					, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeMax.Mana					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Mana"					, nameAndLevelText.c_str());
			++offsetY;
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeCurrent.Health			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Health per turn"			, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeCurrent.Shield			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Shield per turn"			, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.LifeCurrent.Mana				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Mana per turn"			, nameAndLevelText.c_str());
			++offsetY;
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Hit					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Hit Chance"				, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Damage					);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Damage"					, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Health	);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Health"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Shield	);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Shield"	, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.DirectDamage.Mana		);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Mana"		, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Absorption				);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Absorption"				, nameAndLevelText.c_str());
			++offsetY;
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Attack			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Attack Speed"			, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Movement			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Movement Speed"			, nameAndLevelText.c_str());
			nameAndLevelText = std::to_string		(agentFinalPoints.Attack.Speed.Reflexes			);	printfToGrid(display, offsetY++	, offsetX, LEFT, "%-21.21s: %-10.10s"	, "Reflexes"				, nameAndLevelText.c_str());

			const SEntityPoints& agentBasePoints = character.Points;
			nameAndLevelText = std::to_string		(agentBasePoints.Coins							);	printfToGrid(display, ++offsetY	, offsetX, LEFT, "%-21.21s: %-11.11s"	, "- Coins in wallet"		, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX+23, LEFT, &(color = COLOR_ORANGE), 1, 11);
			nameAndLevelText = std::to_string		(agentFinalPoints.Coins							);	printfToGrid(display, ++offsetY	, offsetX, LEFT, "%-21.21s: %-11.11s"	, "- Coins per turn"		, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX+23, LEFT, &(color = (agentFinalPoints.Coins >= 0) ? COLOR_ORANGE : COLOR_RED), 1, 11);
			nameAndLevelText = std::to_string		(agentBasePoints.CostMaintenance				);	printfToGrid(display, ++offsetY	, offsetX, LEFT, "%-21.21s: %-11.11s"	, "- Salary"				, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX+23, LEFT, &(color = COLOR_ORANGE), 1, 11);
			nameAndLevelText = std::to_string		(agentFinalPoints.CostMaintenance				);	printfToGrid(display, ++offsetY	, offsetX, LEFT, "%-21.21s: %-11.11s"	, "- Total Cost"			, nameAndLevelText.c_str());
			valueToGrid(display_.TextAttributes, offsetY, offsetX+23, LEFT, &(color = COLOR_ORANGE), 1, 11);
		}
	}
}

#endif // __AGENT_HELPER_H__91238648097234698723649872364923874__
