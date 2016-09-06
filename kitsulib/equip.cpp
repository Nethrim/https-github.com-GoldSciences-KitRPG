#define NOMINMAX

#include "Game.h"
#include "draw.h"

#include "Accessory.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Vehicle.h"
#include "Facility.h"
#include "StageProp.h"
#include "Item.h"
#include "Enemy.h"

using namespace klib;


SGameState drawEquipMenu(SGame& instanceGame, const SGameState& returnState)
{
#define MAX_BUY_ITEMS 1024
#define SHOP_EXIT_VALUE ((MAX_BUY_ITEMS)*2)

	static klib::SMenuItem<int32_t> menuItems[MAX_BUY_ITEMS+1] = {};

	size_t itemCount=0, i;
	bool bFound;

	SPlayer& player = instanceGame.Player;

	int16_t selectedChoice=0;
	std::string menuTitle = "Equip";
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	menuTitle = " Accessory"	;	for(i=0, itemCount=player.Inventory.Accessory	.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getAccessoryName		(player.Inventory.Accessory		.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_STAGEPROP	:	menuTitle = " Stage Prop"	;	for(i=0, itemCount=player.Inventory.StageProp	.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getStagePropName		(player.Inventory.StageProp		.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_FACILITY		:	menuTitle = " Facility"		;	for(i=0, itemCount=player.Inventory.Facility	.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getFacilityName			(player.Inventory.Facility		.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_VEHICLE		:	menuTitle = " Vehicle"		;	for(i=0, itemCount=player.Inventory.Vehicle		.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getVehicleName			(player.Inventory.Vehicle		.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_PROFESSION	:	menuTitle = " Profession"	;	for(i=0, itemCount=player.Inventory.Profession	.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getProfessionName		(player.Inventory.Profession	.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_WEAPON		:	menuTitle = " Weapon"		;	for(i=0, itemCount=player.Inventory.Weapon		.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getWeaponName			(player.Inventory.Weapon		.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ARMOR		:	menuTitle = " Armor"		;	for(i=0, itemCount=player.Inventory.Armor		.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getArmorName			(player.Inventory.Armor			.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ITEM			:	menuTitle = " Item"			;	for(i=0, itemCount=player.Inventory.Items		.Count; i<itemCount; ++i)	menuItems[i] = { (int32_t)i,	getItemName				(player.Inventory.Items			.Slots[i].Entity)	}; selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_CHARACTER	:	menuTitle = " Agent"		;	
		for(i=0; i<player.Army.size(); ++i)	
		{	
			bFound = false;
			for(uint32_t iSlot=0; iSlot<klib::size(player.Squad.Agents); ++iSlot )
				if(player.Squad.Agents[iSlot] == i)
				{
					bFound = true;
					break;
				}

			if(bFound)
				continue;

			menuItems[itemCount++]	= { (int32_t)i, player.Army[i].Name }; 
		}
		selectedChoice	= drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	
		break;
	default:
		break;
	}

	if( selectedChoice == -1 ) {
		return returnState;
	}
	else if(selectedChoice == SHOP_EXIT_VALUE) {
		if( instanceGame.Player.Selection.PlayerUnit == -1 )
			return { GAME_STATE_MENU_SQUAD_SETUP };
		else
			return { GAME_STATE_MENU_EQUIPMENT };
	}

	SGameState retVal = returnState;

	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	break;
	case GAME_SUBSTATE_STAGEPROP	:	break;
	case GAME_SUBSTATE_FACILITY		:	break;
	case GAME_SUBSTATE_VEHICLE		:	break;
	case GAME_SUBSTATE_PROFESSION	:	break;
	case GAME_SUBSTATE_WEAPON		:	break;
	case GAME_SUBSTATE_ARMOR		:	break;
	case GAME_SUBSTATE_ITEM			:	break;
	case GAME_SUBSTATE_CHARACTER	:	player.Squad.Agents[player.Selection.PlayerUnit] = selectedChoice; retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	default:
		break;
	}

	return retVal;
}

SGameState drawEquip(SGame& instanceGame, const SGameState& returnState)
{
	std::string textToPrint = "You can leave your hat on.";

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds*3);
	memcpy(&instanceGame.PostEffectDisplay.Screen.Cells[instanceGame.PostEffectDisplay.Depth/2][instanceGame.PostEffectDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
	if ( !bDonePrinting ) 
		return returnState;


	drawBubblesBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);
	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		static const SMenu<SGameState, size(optionsEquip)> menuEquip(optionsEquip, {GAME_STATE_WELCOME_COMMANDER}, "Agent Setup", 26);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuEquip, instanceGame.FrameInput, instanceGame.State);
	}
	else 
	{
		if(instanceGame.Player.Selection.PlayerUnit > (int16_t)klib::size(instanceGame.Player.Squad.Agents))
			instanceGame.Player.Selection.PlayerUnit = -1;

		return drawEquipMenu(instanceGame, returnState);
	}

	return returnState;
};


