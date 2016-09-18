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

struct SBuyable
{
	int16_t			Definition; 
	int16_t			Grade; 
	int32_t			Price; 
	int32_t			MaintenanceCost; 
	std::string		Name;
} selectedChoice;


static klib::SMenuItem<SBuyable> menuItemsAccessory		[ktools::size(definitionsAccessory	)] = {};
static klib::SMenuItem<SBuyable> menuItemsStageProp		[ktools::size(definitionsStageProp	)] = {};
static klib::SMenuItem<SBuyable> menuItemsFacility		[ktools::size(definitionsFacility	)] = {};
static klib::SMenuItem<SBuyable> menuItemsVehicle		[ktools::size(definitionsVehicle	)] = {};
static klib::SMenuItem<SBuyable> menuItemsProfession	[ktools::size(definitionsProfession	)] = {};
static klib::SMenuItem<SBuyable> menuItemsWeapon		[ktools::size(definitionsWeapon		)] = {};
static klib::SMenuItem<SBuyable> menuItemsArmor			[ktools::size(definitionsArmor		)] = {};
static klib::SMenuItem<SBuyable> menuItemsItem			[ktools::size(itemDescriptions		)] = {};
static klib::SMenuItem<SBuyable> menuItemsAgent			[ktools::size(enemyDefinitions		)] = {};

#define SHOP_EXIT_VALUE 0x7FFF
static klib::SMenu<SBuyable, ktools::size(menuItemsAccessory	)> menuAccessory	(menuItemsAccessory		,  {SHOP_EXIT_VALUE},	"Accessory"		" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsStageProp	)> menuStageProp	(menuItemsStageProp		,  {SHOP_EXIT_VALUE},	"Stage Prop"	" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsFacility		)> menuFacility		(menuItemsFacility		,  {SHOP_EXIT_VALUE},	"Facility"		" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsVehicle		)> menuVehicle		(menuItemsVehicle		,  {SHOP_EXIT_VALUE},	"Vehicle"		" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsProfession	)> menuProfession	(menuItemsProfession	,  {SHOP_EXIT_VALUE},	"Job License"	" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsWeapon		)> menuWeapon		(menuItemsWeapon		,  {SHOP_EXIT_VALUE},	"Weapon"		" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsArmor		)> menuArmor		(menuItemsArmor			,  {SHOP_EXIT_VALUE},	"Armor"			" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsItem			)> menuItem			(menuItemsItem			,  {SHOP_EXIT_VALUE},	"Item"			" a la carte"	, 26);
static klib::SMenu<SBuyable, ktools::size(menuItemsAgent		)> menuAgent		(menuItemsAgent			,  {SHOP_EXIT_VALUE},	"Agent"			" a la carte"	, 26);


static int32_t initBuyMenus()
{
	for(size_t i=0, itemCount=ktools::size(	definitionsAccessory	); i<itemCount; ++i)	menuItemsAccessory	[i] = { { (int16_t)i, 1, definitionsAccessory	[i].Points.PriceBuy	, definitionsAccessory	[i].Points.CostMaintenance	, definitionsAccessory	[i].Name},	definitionsAccessory	[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	definitionsStageProp	); i<itemCount; ++i)	menuItemsStageProp	[i] = { { (int16_t)i, 1, definitionsStageProp	[i].Points.PriceBuy	, definitionsStageProp	[i].Points.CostMaintenance	, definitionsStageProp	[i].Name},	definitionsStageProp	[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	definitionsFacility		); i<itemCount; ++i)	menuItemsFacility	[i] = { { (int16_t)i, 1, definitionsFacility	[i].Points.PriceBuy	, definitionsFacility	[i].Points.CostMaintenance	, definitionsFacility	[i].Name},	definitionsFacility		[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	definitionsVehicle		); i<itemCount; ++i)	menuItemsVehicle	[i] = { { (int16_t)i, 1, definitionsVehicle		[i].Points.PriceBuy	, definitionsVehicle	[i].Points.CostMaintenance	, definitionsVehicle	[i].Name},	definitionsVehicle		[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	definitionsProfession	); i<itemCount; ++i)	menuItemsProfession	[i] = { { (int16_t)i, 1, definitionsProfession	[i].Points.PriceBuy	, definitionsProfession	[i].Points.CostMaintenance	, definitionsProfession	[i].Name},	definitionsProfession	[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	definitionsWeapon		); i<itemCount; ++i)	menuItemsWeapon		[i] = { { (int16_t)i, 1, definitionsWeapon		[i].Points.PriceBuy	, definitionsWeapon		[i].Points.CostMaintenance	, definitionsWeapon		[i].Name},	definitionsWeapon		[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	definitionsArmor		); i<itemCount; ++i)	menuItemsArmor		[i] = { { (int16_t)i, 1, definitionsArmor		[i].Points.PriceBuy	, definitionsArmor		[i].Points.CostMaintenance	, definitionsArmor		[i].Name},	definitionsArmor		[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	itemDescriptions		); i<itemCount; ++i)	menuItemsItem		[i] = { { (int16_t)i, 1, itemDescriptions		[i].Price			, 0													, itemDescriptions		[i].Name},	itemDescriptions		[i].Name };	
	for(size_t i=0, itemCount=ktools::size(	enemyDefinitions		); i<itemCount; ++i)	menuItemsAgent		[i] = { { (int16_t)i, 1, enemyDefinitions		[i].Points.PriceBuy	, enemyDefinitions		[i].Points.CostMaintenance	, enemyDefinitions		[i].Name},	enemyDefinitions		[i].Name };	

	for(size_t i=0, itemCount=ktools::size(	definitionsAccessory	); i<itemCount; ++i)	menuAccessory	.Items[i] = menuItemsAccessory	[i] ;	
	for(size_t i=0, itemCount=ktools::size(	definitionsStageProp	); i<itemCount; ++i)	menuStageProp	.Items[i] = menuItemsStageProp	[i] ;	
	for(size_t i=0, itemCount=ktools::size(	definitionsFacility		); i<itemCount; ++i)	menuFacility	.Items[i] = menuItemsFacility	[i] ;	
	for(size_t i=0, itemCount=ktools::size(	definitionsVehicle		); i<itemCount; ++i)	menuVehicle		.Items[i] = menuItemsVehicle	[i] ;	
	for(size_t i=0, itemCount=ktools::size(	definitionsProfession	); i<itemCount; ++i)	menuProfession	.Items[i] = menuItemsProfession	[i] ;	
	for(size_t i=0, itemCount=ktools::size(	definitionsWeapon		); i<itemCount; ++i)	menuWeapon		.Items[i] = menuItemsWeapon		[i] ;	
	for(size_t i=0, itemCount=ktools::size(	definitionsArmor		); i<itemCount; ++i)	menuArmor		.Items[i] = menuItemsArmor		[i] ;	
	for(size_t i=0, itemCount=ktools::size( itemDescriptions		); i<itemCount; ++i)	menuItem		.Items[i] = menuItemsItem		[i] ;	
	for(size_t i=0, itemCount=ktools::size( enemyDefinitions		); i<itemCount; ++i)	menuAgent		.Items[i] = menuItemsAgent		[i] ;	

	return 0;
}

SGameState drawBuyMenu(SGame& instanceGame, const SGameState& returnState)
{
#define MAX_BUY_ITEMS 64

	static const int32_t initedMenus = initBuyMenus();

	static klib::SMenuItem<SBuyable> menuItems[MAX_BUY_ITEMS+1] = {};

	std::string menuTitle;
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuAccessory	, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_STAGEPROP	:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuStageProp	, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_FACILITY		:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuFacility	, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_VEHICLE		:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuVehicle	, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_PROFESSION	:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuProfession	, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_WEAPON		:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuWeapon		, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_ARMOR		:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuArmor		, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_ITEM			:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuItem		, instanceGame.FrameInput, {-1});	break;
	case GAME_SUBSTATE_CHARACTER	:	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuAgent		, instanceGame.FrameInput, {-1});	break;
	default:
		break;
	}

	if(selectedChoice.Definition == SHOP_EXIT_VALUE)
		return {GAME_STATE_MENU_BUY};
	else if( selectedChoice.Definition == -1 )
		return returnState;

	SGameState retVal = returnState;

	SPlayer& player = instanceGame.Players[PLAYER_USER];

	if(selectedChoice.Price > player.Money)
	{
		instanceGame.UserMessage	= "";
		instanceGame.UserError		= "You don't have enough money for " + selectedChoice.Name + "!!";
		return retVal;
	}
	else
		instanceGame.UserError		= "";

	player.Money -= selectedChoice.Price;

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
	case GAME_SUBSTATE_CHARACTER	:	player.Army.push_back(enemyDefinitions[selectedChoice.Definition]);	instanceGame.UserMessage = "You have successfully hired " + selectedChoice.Name + " for " + std::to_string(selectedChoice.MaintenanceCost) + " Coins/Month."; break;
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

	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		static const SMenu<SGameState, ktools::size(optionsBuy)> menuBuy(optionsBuy, {GAME_STATE_WELCOME_COMMANDER}, "Order Menu", 26);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuBuy, instanceGame.FrameInput, instanceGame.State);
	}
	else 
		return drawBuyMenu(instanceGame, returnState);

	return returnState;
};


