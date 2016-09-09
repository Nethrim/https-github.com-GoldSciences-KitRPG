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


SGameState drawBuyMenu(SGame& instanceGame, const SGameState& returnState)
{
#define MAX_BUY_ITEMS 64
#define SHOP_EXIT_VALUE 0x7FFF

	struct SBuyable
	{
		int16_t			Definition; 
		int16_t			Grade; 
		int32_t			Price; 
		std::string		Name;
	} selectedChoice;

	static klib::SMenuItem<SBuyable> menuItems[MAX_BUY_ITEMS+1] = {};

	size_t itemCount=0, i;
	std::string menuTitle;
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	menuTitle = "Accessory"		;	menuTitle += " a la carte";	for(i=0, itemCount=size(definitionsAccessory	); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, definitionsAccessory	[i].Points.Price, definitionsAccessory	[i].Name},	definitionsAccessory	[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_STAGEPROP	:	menuTitle = "Stage Prop"	;	menuTitle += " a la carte";	for(i=0, itemCount=size(definitionsStageProp	); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, definitionsStageProp	[i].Points.Price, definitionsStageProp	[i].Name},	definitionsStageProp	[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_FACILITY		:	menuTitle = "Facility"		;	menuTitle += " a la carte";	for(i=0, itemCount=size(definitionsFacility		); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, definitionsFacility	[i].Points.Price, definitionsFacility	[i].Name},	definitionsFacility		[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_VEHICLE		:	menuTitle = "Vehicle"		;	menuTitle += " a la carte";	for(i=0, itemCount=size(definitionsVehicle		); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, definitionsVehicle	[i].Points.Price, definitionsVehicle	[i].Name},	definitionsVehicle		[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_PROFESSION	:	menuTitle = "Profession"	;	menuTitle += " a la carte";	for(i=0, itemCount=size(definitionsProfession	); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, definitionsProfession	[i].Points.Price, definitionsProfession	[i].Name},	definitionsProfession	[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_WEAPON		:	menuTitle = "Weapon"		;	menuTitle += " a la carte";	for(i=0, itemCount=size(definitionsWeapon		); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, definitionsWeapon		[i].Points.Price, definitionsWeapon		[i].Name},	definitionsWeapon		[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ARMOR		:	menuTitle = "Armor"			;	menuTitle += " a la carte";	for(i=0, itemCount=size(definitionsArmor		); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, definitionsArmor		[i].Points.Price, definitionsArmor		[i].Name},	definitionsArmor		[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_CHARACTER	:	menuTitle = "Agent"			;	menuTitle += " a la carte";	for(i=0, itemCount=size(enemyDefinitions		); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, enemyDefinitions		[i].Points.Price, enemyDefinitions		[i].Name},	enemyDefinitions		[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ITEM			:	menuTitle = "Item"			;	menuTitle += " a la carte";	for(i=0, itemCount=size(itemDescriptions		); i<itemCount; ++i)	menuItems[i] = { { (int16_t)i, 1, itemDescriptions		[i].Price, itemDescriptions				[i].Name},	itemDescriptions		[i].Name };	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, {SHOP_EXIT_VALUE}, {-1}, 40U, false, "Exit this menu");	break;
	default:
		break;
	}
	//menuTitle += " a la carte";
	//
	//selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");
	if(selectedChoice.Definition == SHOP_EXIT_VALUE)
		return {GAME_STATE_MENU_BUY};
	else if( selectedChoice.Definition == -1 )
		return returnState;

	SGameState retVal = returnState;

	SPlayer& player = instanceGame.Player;

	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	player.Inventory.Accessory	.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins."; break;
	case GAME_SUBSTATE_STAGEPROP	:	player.Inventory.StageProp	.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins."; break;
	case GAME_SUBSTATE_FACILITY		:	player.Inventory.Facility	.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins."; break;
	case GAME_SUBSTATE_VEHICLE		:	player.Inventory.Vehicle	.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins."; break;
	case GAME_SUBSTATE_PROFESSION	:	player.Inventory.Profession	.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " Job License for " + std::to_string(selectedChoice.Price) + " Coins."	; break;
	case GAME_SUBSTATE_WEAPON		:	player.Inventory.Weapon		.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins."; break;
	case GAME_SUBSTATE_ARMOR		:	player.Inventory.Armor		.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins."; break;
	case GAME_SUBSTATE_ITEM			:	player.Inventory.Items		.AddElement({selectedChoice.Definition, 0, selectedChoice.Grade});	instanceGame.UserMessage = "You have successfully bought " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins."; break;
	case GAME_SUBSTATE_CHARACTER	:	player.Army.push_back(enemyDefinitions[selectedChoice.Definition]);	break;
	default:
		break;
	}

	return retVal;
}

SGameState drawBuy(SGame& instanceGame, const SGameState& returnState)
{
	std::string textToPrint = "Tell me how much money you have and I will tell you what you'll become.";

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds*3);
	memcpy(&instanceGame.PostEffectDisplay.Screen.Cells[instanceGame.PostEffectDisplay.Depth>>1][instanceGame.PostEffectDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
	if ( !bDonePrinting ) 
		return returnState;

	drawBubblesBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);
	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		static const SMenu<SGameState, size(optionsBuy)> menuBuy(optionsBuy, {GAME_STATE_WELCOME_COMMANDER}, "Order Menu", 26);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuBuy, instanceGame.FrameInput, instanceGame.State);
	}
	else 
		return drawBuyMenu(instanceGame, returnState);

	return returnState;
};


