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
	std::string		Name;
} selectedChoice;


static klib::SMenuItem<SBuyable> menuItemsAccessory		[size(definitionsAccessory	)] = {};
static klib::SMenuItem<SBuyable> menuItemsStageProp		[size(definitionsStageProp	)] = {};
static klib::SMenuItem<SBuyable> menuItemsFacility		[size(definitionsFacility	)] = {};
static klib::SMenuItem<SBuyable> menuItemsVehicle		[size(definitionsVehicle	)] = {};
static klib::SMenuItem<SBuyable> menuItemsProfession	[size(definitionsProfession	)] = {};
static klib::SMenuItem<SBuyable> menuItemsWeapon		[size(definitionsWeapon		)] = {};
static klib::SMenuItem<SBuyable> menuItemsArmor			[size(definitionsArmor		)] = {};
static klib::SMenuItem<SBuyable> menuItemsItem			[size(itemDescriptions		)] = {};
static klib::SMenuItem<SBuyable> menuItemsAgent			[size(enemyDefinitions		)] = {};

#define SHOP_EXIT_VALUE 0x7FFF
static klib::SMenu<SBuyable, size(menuItemsAccessory	)> menuAccessory	(menuItemsAccessory		,  {SHOP_EXIT_VALUE},	"Accessory"		" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsStageProp	)> menuStageProp	(menuItemsStageProp		,  {SHOP_EXIT_VALUE},	"Stage Prop"	" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsFacility		)> menuFacility		(menuItemsFacility		,  {SHOP_EXIT_VALUE},	"Facility"		" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsVehicle		)> menuVehicle		(menuItemsVehicle		,  {SHOP_EXIT_VALUE},	"Vehicle"		" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsProfession	)> menuProfession	(menuItemsProfession	,  {SHOP_EXIT_VALUE},	"Profession"	" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsWeapon		)> menuWeapon		(menuItemsWeapon		,  {SHOP_EXIT_VALUE},	"Weapon"		" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsArmor		)> menuArmor		(menuItemsArmor			,  {SHOP_EXIT_VALUE},	"Armor"			" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsItem			)> menuItem			(menuItemsItem			,  {SHOP_EXIT_VALUE},	"Item"			" a la carte"	, 26);
static klib::SMenu<SBuyable, size(menuItemsAgent		)> menuAgent		(menuItemsAgent			,  {SHOP_EXIT_VALUE},	"Agent"			" a la carte"	, 26);


static int32_t initBuyMenus()
{
	for(size_t i=0, itemCount=size(	definitionsAccessory	); i<itemCount; ++i)	menuItemsAccessory	[i] = { { (int16_t)i, 1, definitionsAccessory	[i].Points.PriceBuy	, definitionsAccessory	[i].Name},	definitionsAccessory	[i].Name };	
	for(size_t i=0, itemCount=size(	definitionsStageProp	); i<itemCount; ++i)	menuItemsStageProp	[i] = { { (int16_t)i, 1, definitionsStageProp	[i].Points.PriceBuy	, definitionsStageProp	[i].Name},	definitionsStageProp	[i].Name };	
	for(size_t i=0, itemCount=size(	definitionsFacility		); i<itemCount; ++i)	menuItemsFacility	[i] = { { (int16_t)i, 1, definitionsFacility	[i].Points.PriceBuy	, definitionsFacility	[i].Name},	definitionsFacility		[i].Name };	
	for(size_t i=0, itemCount=size(	definitionsVehicle		); i<itemCount; ++i)	menuItemsVehicle	[i] = { { (int16_t)i, 1, definitionsVehicle		[i].Points.PriceBuy	, definitionsVehicle	[i].Name},	definitionsVehicle		[i].Name };	
	for(size_t i=0, itemCount=size(	definitionsProfession	); i<itemCount; ++i)	menuItemsProfession	[i] = { { (int16_t)i, 1, definitionsProfession	[i].Points.PriceBuy	, definitionsProfession	[i].Name},	definitionsProfession	[i].Name };	
	for(size_t i=0, itemCount=size(	definitionsWeapon		); i<itemCount; ++i)	menuItemsWeapon		[i] = { { (int16_t)i, 1, definitionsWeapon		[i].Points.PriceBuy	, definitionsWeapon		[i].Name},	definitionsWeapon		[i].Name };	
	for(size_t i=0, itemCount=size(	definitionsArmor		); i<itemCount; ++i)	menuItemsArmor		[i] = { { (int16_t)i, 1, definitionsArmor		[i].Points.PriceBuy	, definitionsArmor		[i].Name},	definitionsArmor		[i].Name };	
	for(size_t i=0, itemCount=size(	itemDescriptions		); i<itemCount; ++i)	menuItemsItem		[i] = { { (int16_t)i, 1, itemDescriptions		[i].Price			, itemDescriptions		[i].Name},	itemDescriptions		[i].Name };	
	for(size_t i=0, itemCount=size(	enemyDefinitions		); i<itemCount; ++i)	menuItemsAgent		[i] = { { (int16_t)i, 1, enemyDefinitions		[i].Points.PriceBuy	, enemyDefinitions		[i].Name},	enemyDefinitions		[i].Name };	

	for(size_t i=0, itemCount=size(	definitionsAccessory	); i<itemCount; ++i)	menuAccessory	.Items[i] = menuItemsAccessory	[i] ;	
	for(size_t i=0, itemCount=size(	definitionsStageProp	); i<itemCount; ++i)	menuStageProp	.Items[i] = menuItemsStageProp	[i] ;	
	for(size_t i=0, itemCount=size(	definitionsFacility		); i<itemCount; ++i)	menuFacility	.Items[i] = menuItemsFacility	[i] ;	
	for(size_t i=0, itemCount=size(	definitionsVehicle		); i<itemCount; ++i)	menuVehicle		.Items[i] = menuItemsVehicle	[i] ;	
	for(size_t i=0, itemCount=size(	definitionsProfession	); i<itemCount; ++i)	menuProfession	.Items[i] = menuItemsProfession	[i] ;	
	for(size_t i=0, itemCount=size(	definitionsWeapon		); i<itemCount; ++i)	menuWeapon		.Items[i] = menuItemsWeapon		[i] ;	
	for(size_t i=0, itemCount=size(	definitionsArmor		); i<itemCount; ++i)	menuArmor		.Items[i] = menuItemsArmor		[i] ;	
	for(size_t i=0, itemCount=size( itemDescriptions		); i<itemCount; ++i)	menuItem		.Items[i] = menuItemsItem		[i] ;	
	for(size_t i=0, itemCount=size( enemyDefinitions		); i<itemCount; ++i)	menuAgent		.Items[i] = menuItemsAgent		[i] ;	

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
	case GAME_SUBSTATE_CHARACTER	:	player.Army.push_back(enemyDefinitions[selectedChoice.Definition]);	instanceGame.UserMessage = "You have successfully hired " + selectedChoice.Name + " for " + std::to_string(selectedChoice.Price) + " Coins/Month."; break;
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


