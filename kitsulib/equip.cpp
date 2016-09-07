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
		if( instanceGame.Player.Selection.PlayerUnit == -1 || player.Squad.Agents[instanceGame.Player.Selection.PlayerUnit] == -1)
			return { GAME_STATE_MENU_SQUAD_SETUP };
		else
			return { GAME_STATE_MENU_EQUIPMENT };
	}

	SGameState retVal = returnState;

	SEntity selectedItem = {0,0,0};
	instanceGame.UserMessage = instanceGame.UserError = "";
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	selectedItem = player.Inventory.Accessory	.Slots[selectedChoice].Entity;	if(selectedItem.Definition	&&	player.CompletedResearch.Accessory	.Definitions.FindElement(selectedItem.Definition) == -1)	{	instanceGame.UserError = "You can't use " + definitionsAccessory	[selectedItem.Definition].Name	+ " without researching it first!"; break; }	else if(selectedItem.Modifier &&	player.CompletedResearch.Accessory	.Modifiers.FindElement(selectedItem.Modifier) == -1) {	char modifierName[64] = {}; sprintf_s(modifierName, modifiersAccessory	[selectedItem.Modifier].Name.c_str(), "Crafting"		);	instanceGame.UserError = "You can't use " + std::string(modifierName) + " Accessories"	" without researching them first!"; break;}	else {	player.Inventory.Accessory	.AddElement(player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Accessory	);	player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Accessory	= selectedChoice; player.Inventory.Accessory	.DecreaseEntity(selectedChoice); instanceGame.UserMessage = "You equipped " + getAccessoryName	({selectedItem.Definition, selectedItem.Modifier}) + " to " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name; retVal = { GAME_STATE_MENU_EQUIPMENT }; }	break;
	case GAME_SUBSTATE_STAGEPROP	:	selectedItem = player.Inventory.StageProp	.Slots[selectedChoice].Entity;	if(selectedItem.Definition	&&	player.CompletedResearch.StageProp	.Definitions.FindElement(selectedItem.Definition) == -1)	{	instanceGame.UserError = "You can't use " + definitionsStageProp	[selectedItem.Definition].Name	+ " without researching it first!"; break; }	else if(selectedItem.Modifier &&	player.CompletedResearch.StageProp	.Modifiers.FindElement(selectedItem.Modifier) == -1) {	char modifierName[64] = {}; sprintf_s(modifierName, modifiersStageProp	[selectedItem.Modifier].Name.c_str(), ""				);	instanceGame.UserError = "You can't use " + std::string(modifierName) + " Stage Props"	" without researching them first!"; break;}	else {	player.Inventory.StageProp	.AddElement(player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.StageProp	);	player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.StageProp	= selectedChoice; player.Inventory.StageProp	.DecreaseEntity(selectedChoice); instanceGame.UserMessage = "You equipped " + getStagePropName	({selectedItem.Definition, selectedItem.Modifier}) + " to " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name; retVal = { GAME_STATE_MENU_EQUIPMENT }; }	break;
	case GAME_SUBSTATE_FACILITY		:	selectedItem = player.Inventory.Facility	.Slots[selectedChoice].Entity;	if(selectedItem.Definition	&&	player.CompletedResearch.Facility	.Definitions.FindElement(selectedItem.Definition) == -1)	{	instanceGame.UserError = "You can't use " + definitionsFacility		[selectedItem.Definition].Name	+ " without researching it first!"; break; }	else if(selectedItem.Modifier &&	player.CompletedResearch.Facility	.Modifiers.FindElement(selectedItem.Modifier) == -1) {	char modifierName[64] = {}; sprintf_s(modifierName, modifiersFacility	[selectedItem.Modifier].Name.c_str(), "Architectonics"	);	instanceGame.UserError = "You can't use " + std::string(modifierName) + " Facilities"	" without researching them first!"; break;}	else {	player.Inventory.Facility	.AddElement(player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Facility		);	player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Facility		= selectedChoice; player.Inventory.Facility		.DecreaseEntity(selectedChoice); instanceGame.UserMessage = "You equipped " + getFacilityName	({selectedItem.Definition, selectedItem.Modifier}) + " to " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name; retVal = { GAME_STATE_MENU_EQUIPMENT }; }	break;
	case GAME_SUBSTATE_VEHICLE		:	selectedItem = player.Inventory.Vehicle		.Slots[selectedChoice].Entity;	if(selectedItem.Definition	&&	player.CompletedResearch.Vehicle	.Definitions.FindElement(selectedItem.Definition) == -1)	{	instanceGame.UserError = "You can't use " + definitionsVehicle		[selectedItem.Definition].Name	+ " without researching it first!"; break; }	else if(selectedItem.Modifier &&	player.CompletedResearch.Vehicle	.Modifiers.FindElement(selectedItem.Modifier) == -1) {	char modifierName[64] = {}; sprintf_s(modifierName, modifiersVehicle	[selectedItem.Modifier].Name.c_str(), "Transportation"	);	instanceGame.UserError = "You can't use " + std::string(modifierName) + " Vehicles"		" without researching them first!"; break;}	else {	player.Inventory.Vehicle	.AddElement(player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Vehicle		);	player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Vehicle		= selectedChoice; player.Inventory.Vehicle		.DecreaseEntity(selectedChoice); instanceGame.UserMessage = "You equipped " + getVehicleName	({selectedItem.Definition, selectedItem.Modifier}) + " to " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name; retVal = { GAME_STATE_MENU_EQUIPMENT }; }	break;
	case GAME_SUBSTATE_PROFESSION	:	selectedItem = player.Inventory.Profession	.Slots[selectedChoice].Entity;	if(selectedItem.Definition	&&	player.CompletedResearch.Profession	.Definitions.FindElement(selectedItem.Definition) == -1)	{	instanceGame.UserError = "You can't use " + definitionsProfession	[selectedItem.Definition].Name	+ " without researching it first!"; break; }	else if(selectedItem.Modifier &&	player.CompletedResearch.Profession	.Modifiers.FindElement(selectedItem.Modifier) == -1) {	char modifierName[64] = {}; sprintf_s(modifierName, modifiersProfession	[selectedItem.Modifier].Name.c_str(), "Rank"			);	instanceGame.UserError = "You can't use " + std::string(modifierName) + " Licenses"		" without researching them first!"; break;}	else {	player.Inventory.Profession	.AddElement(player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Profession	);	player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Profession	= selectedChoice; player.Inventory.Profession	.DecreaseEntity(selectedChoice); instanceGame.UserMessage = "You equipped " + getProfessionName	({selectedItem.Definition, selectedItem.Modifier}) + " to " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name; retVal = { GAME_STATE_MENU_EQUIPMENT }; }	break;
	case GAME_SUBSTATE_WEAPON		:	selectedItem = player.Inventory.Weapon		.Slots[selectedChoice].Entity;	if(selectedItem.Definition	&&	player.CompletedResearch.Weapon		.Definitions.FindElement(selectedItem.Definition) == -1)	{	instanceGame.UserError = "You can't use " + definitionsWeapon		[selectedItem.Definition].Name	+ " without researching it first!"; break; }	else if(selectedItem.Modifier &&	player.CompletedResearch.Weapon		.Modifiers.FindElement(selectedItem.Modifier) == -1) {	char modifierName[64] = {}; sprintf_s(modifierName, modifiersWeapon		[selectedItem.Modifier].Name.c_str(), "Science"			);	instanceGame.UserError = "You can't use " + std::string(modifierName) + " Weapons"		" without researching them first!"; break;}	else {	player.Inventory.Weapon		.AddElement(player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Weapon		);	player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Weapon		= selectedChoice; player.Inventory.Weapon		.DecreaseEntity(selectedChoice); instanceGame.UserMessage = "You equipped " + getWeaponName		({selectedItem.Definition, selectedItem.Modifier}) + " to " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name; retVal = { GAME_STATE_MENU_EQUIPMENT }; }	break;
	case GAME_SUBSTATE_ARMOR		:	selectedItem = player.Inventory.Armor		.Slots[selectedChoice].Entity;	if(selectedItem.Definition	&&	player.CompletedResearch.Armor		.Definitions.FindElement(selectedItem.Definition) == -1)	{	instanceGame.UserError = "You can't use " + definitionsArmor		[selectedItem.Definition].Name	+ " without researching it first!"; break; }	else if(selectedItem.Modifier &&	player.CompletedResearch.Armor		.Modifiers.FindElement(selectedItem.Modifier) == -1) {	char modifierName[64] = {}; sprintf_s(modifierName, modifiersArmor		[selectedItem.Modifier].Name.c_str(), "Technology"		);	instanceGame.UserError = "You can't use " + std::string(modifierName) + " Armors"		" without researching them first!"; break;}	else {	player.Inventory.Armor		.AddElement(player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Armor		);	player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Armor		= selectedChoice; player.Inventory.Armor		.DecreaseEntity(selectedChoice); instanceGame.UserMessage = "You equipped " + getArmorName		({selectedItem.Definition, selectedItem.Modifier}) + " to " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name; retVal = { GAME_STATE_MENU_EQUIPMENT }; }	break;
	case GAME_SUBSTATE_ITEM			:	break;
	case GAME_SUBSTATE_CHARACTER	:	player.Squad.Agents[player.Selection.PlayerUnit] = selectedChoice; retVal = { GAME_STATE_MENU_EQUIPMENT }; 
		instanceGame.UserMessage = "You assigned " + player.Army[selectedChoice].Name + " to the squad.";
		break;
	default:
		break;
	}

	return retVal;
}

SGameState drawEquip(SGame& instanceGame, const SGameState& returnState)
{
	SPlayer& player = instanceGame.Player;
	if( player.Selection.PlayerUnit != -1 && player.Squad.Agents[player.Selection.PlayerUnit] != -1) {
		std::string textToPrint = "Agent #" + std::to_string(player.Selection.PlayerUnit+1) + ": "+ instanceGame.Player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name + ".";
		bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds*3);
		memcpy(&instanceGame.PostEffectDisplay.Screen.Cells[instanceGame.PostEffectDisplay.Depth/2][instanceGame.PostEffectDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
		if ( !bDonePrinting ) 
			return returnState;
	}

	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		static const SMenu<SGameState, size(optionsEquip)> menuEquip(optionsEquip, {GAME_STATE_MENU_SQUAD_SETUP}, "Agent Setup", 26);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuEquip, instanceGame.FrameInput, instanceGame.State);
	}
	else 
	{
		if( player.Selection.PlayerUnit > (int16_t)klib::size(player.Squad.Agents))
			player.Selection.PlayerUnit = -1;

		return drawEquipMenu(instanceGame, returnState);
	}

	return returnState;
};


