#include "draw.h"

#include "Profession.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "Facility.h"
#include "Vehicle.h"
#include "StageProp.h"

using namespace klib;

template <typename _TCell, size_t _Width, size_t _Depth>
void displayEmptySlot(SGrid<_TCell, _Width, _Depth>& display, int32_t offsetY, int32_t offsetX, int32_t agentIndex) {
	lineToGrid(display, offsetY		, offsetX, LEFT, "-- Agent #%u: %-36.36s", agentIndex, "Open position");
}

template <typename _TCell, size_t _Width, size_t _Depth>
void displayAgentSlot(SGrid<_TCell, _Width, _Depth>& display, int32_t offsetY, int32_t offsetX, int32_t agentIndex, CCharacter& character, bool bAddFieldNames=false)
{
	std::string nameAndLevelText;
	SEntityPoints agentPoints = calculateFinalPoints( character );
	if( bAddFieldNames ) {
		nameAndLevelText = character.Name												;	lineToGrid(display, offsetY++	, offsetX, LEFT, "-- Agent #%u: %-34.34s (Lv. %i)", agentIndex			, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		++offsetY;
		nameAndLevelText = getProfessionName	(character.CurrentEquip.Profession		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Class"				, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = getWeaponName		(character.CurrentEquip.Weapon			);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Weapon"				, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = getArmorName			(character.CurrentEquip.Armor			);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Armor"				, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = getAccessoryName		(character.CurrentEquip.Accessory		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Accessory"			, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = getVehicleName		(character.CurrentEquip.Vehicle			);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Vehicle"				, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
		nameAndLevelText = getFacilityName		(character.CurrentEquip.Facility		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Facility"			, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
		nameAndLevelText = getStagePropName		(character.CurrentEquip.StageProp		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Stage prop"			, nameAndLevelText.c_str(), character.CurrentEquip.StageProp	.Level);
		lineToGrid(display, ++offsetY, offsetX, LEFT, "- Final Points:");
		offsetY+=2;
		nameAndLevelText = std::to_string		(agentPoints.LifeMax.Health				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Health"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.LifeMax.Shield				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Shield"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.LifeMax.Mana				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Mana"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Hit					);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Hit Chance"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Damage				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Damage"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.DirectDamage.Health	);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "DD. Health"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.DirectDamage.Shield	);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "DD. Shield"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.DirectDamage.Mana	);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "DD. Mana"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Absorption			);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Absorption"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Speed.Attack		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Atk. Speed"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Speed.Movement		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Mov. Speed"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Speed.Reflexes		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Reflexes"	, nameAndLevelText.c_str());
	}
	else {
		nameAndLevelText = character.Name												;	lineToGrid(display, offsetY++	, offsetX, LEFT, "-- %-21.21s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = getProfessionName	(character.CurrentEquip.Profession		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = getWeaponName		(character.CurrentEquip.Weapon			);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = getArmorName			(character.CurrentEquip.Armor			);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = getAccessoryName		(character.CurrentEquip.Accessory		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = getVehicleName		(character.CurrentEquip.Vehicle			);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
		//nameAndLevelText = getFacilityName	(character.CurrentEquip.Facility		);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-34.34s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
		offsetY+=1;
		nameAndLevelText = std::to_string		(agentPoints.LifeMax.Health				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Health"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.LifeMax.Shield				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Shield"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.LifeMax.Mana				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Mana"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Hit					);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Hit Chance"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string		(agentPoints.Attack.Damage				);	lineToGrid(display, offsetY++	, offsetX, LEFT, "%-10.10s: %-10.10s"	, "Damage"		, nameAndLevelText.c_str());
	}
}

#define MAX_AGENT_ROWS		2
#define MAX_AGENT_COLUMNS	3

void drawSquadSlots(SGame& instanceGame, const SGameState& returnValue)
{
	SGlobalDisplay& display = instanceGame.GlobalDisplay;
	static const int32_t slotWidth	= display.Width / MAX_AGENT_COLUMNS;
	static const int32_t slotRowSpace	= 27;// display.Depth / (MAX_AGENT_ROWS);

	int32_t playerOffset = (instanceGame.SelectedPlayerUnit != -1) ? std::min(std::max(0, instanceGame.SelectedPlayerUnit-1), SGameSquad::Size-6) : 0;

	for(int32_t y=0, countY=MAX_AGENT_ROWS; y<countY; ++y)
	{
		for(int32_t x=0, countX=MAX_AGENT_COLUMNS; x<countX; ++x) 
		{
			int32_t linearIndex			= y*countX+x;
			int32_t agentIndexOffset	= linearIndex+playerOffset;
			
			if(agentIndexOffset < SGameSquad::Size) {
				if( instanceGame.PlayerSquads.Agents[agentIndexOffset] != -1 )
					displayAgentSlot(display.Screen, TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, agentIndexOffset+1, instanceGame.GameArmies.Player[instanceGame.PlayerSquads.Agents[agentIndexOffset]], true);
				else											 
					displayEmptySlot(display.Screen, TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, agentIndexOffset+1);
			}
		}
	}
}

SGameState drawSquadSetupMenu(SGame& instanceGame, const SGameState& returnValue)
{
	drawSquadSlots(instanceGame, returnValue);

	static SMenuItem<int32_t> menuItems[SGameSquad::Size] = {};
	static int32_t maxNameLen = 0;
	for(uint32_t i=0, count=(uint32_t)size(menuItems); i<count; i++) 
	{
		menuItems[i].ReturnValue = i;

		char buffer[128];
		if(instanceGame.PlayerSquads.Agents[i] != -1) {
			maxNameLen = std::max(maxNameLen, sprintf_s(buffer, "Agent #%u: %s", i+1, instanceGame.GameArmies.Player[instanceGame.PlayerSquads.Agents[i]].Name.c_str()));
			menuItems[i].Text = buffer;
		}
		else {
			maxNameLen = std::max(maxNameLen, sprintf_s(buffer, "Agent #%u: Empty slot", i+1));
			menuItems[i].Text = buffer;
		}
	}

	int32_t result = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], "Squad setup", menuItems, instanceGame.FrameInput, SGameSquad::Size, -1, std::max(24, maxNameLen+4));
	if(SGameSquad::Size == result)
		return {GAME_STATE_WELCOME_COMMANDER};

	if( result < 0 || result >= SGameSquad::Size )
		return {GAME_STATE_MENU_SQUAD_SETUP};

	instanceGame.SelectedPlayerUnit = result;

	if( instanceGame.PlayerSquads.Agents[result] != -1 && 0 == instanceGame.FrameInput.Keys[VK_LSHIFT] ) {
		return {GAME_STATE_MENU_EQUIPMENT};
	}

	return {GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_CHARACTER};
}