#define NOMINMAX

#include "Agent_helper.h"

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

static const god::glabel labelAccessory	= "Accessory"		;
static const god::glabel labelStageProp	= "Stage Prop"		;
static const god::glabel labelFacility	= "Facility"		;
static const god::glabel labelVehicle	= "Vehicle"			;
static const god::glabel labelJob		= "Job License"		;
static const god::glabel labelWeapon	= "Weapon"			;
static const god::glabel labelArmor		= "Armor"			;
static const god::glabel labelItem		= "Item"			;


template <typename _EntityType, size_t _MaxSlots, size_t _SizeDefinitions, size_t _SizeModifiers>
bool equipIfResearched
(	int16_t selectedChoice
,	std::vector<CCharacter>& playerArmy
,	SEntityContainer<_EntityType, _MaxSlots>& playerInventory
,	_EntityType& agentEquippedEntity
,	const SSquad& playerSquad
,	const SPlayerSelection& playerSelection
,	const SResearchGroup<_EntityType>& playerCompletedResearch
,	const SEntityRecord<_EntityType> (&entityDefinitions)	[_SizeDefinitions]
,	const SEntityRecord<_EntityType> (&entityModifiers)		[_SizeModifiers]
,	const god::glabel& modifierTypeName
,	const god::glabel& entityTypePlural
,	std::string& messageSuccess
,	std::string& messageError
)
{
	_EntityType selectedItem = playerInventory.Slots[selectedChoice].Entity;	
	if(selectedItem.Definition	&&	playerCompletedResearch.Definitions.FindElement(selectedItem.Definition) == -1)	
	{	
		messageError = "You can't use " + std::string(entityDefinitions[selectedItem.Definition].Name)	+ " without researching it first!"; 
		return false; 
	}	
	else if(selectedItem.Modifier && playerCompletedResearch.Modifiers.FindElement(selectedItem.Modifier) == -1) 
	{	
		char modifierName[72] = {}; 
		sprintf_s(modifierName, entityModifiers[selectedItem.Modifier].Name.c_str(), modifierTypeName.c_str());	
		char message[256] = {}; 
		sprintf_s(message, "You can't use %s without researching them first!", message);
		messageError = message;
		return false;
	}	
	else 
	{	
		playerInventory.AddElement(agentEquippedEntity);	
		agentEquippedEntity	= selectedItem; 
		playerInventory.DecreaseEntity(selectedChoice); 
		messageSuccess = "You equipped " + getEntityName(selectedItem, entityDefinitions, entityModifiers) + " to " + playerArmy[playerSquad.Agents[playerSelection.PlayerUnit]].Name; 
		return true;
	}
}

static bool equipIfResearchedAccessory	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; static const god::glabel modifierTypeName = "Crafting"			, entityTypePlural = "Accessories"	; return equipIfResearched(selectedChoice, player.Army, player.Goods.Inventory.Accessory	, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Accessory		, player.Squad, player.Selection, player.Goods.CompletedResearch.Accessory		, definitionsAccessory		, modifiersAccessory	, modifierTypeName, entityTypePlural, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedStageProp	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; static const god::glabel modifierTypeName = ""					, entityTypePlural = "Stage Props"	; return equipIfResearched(selectedChoice, player.Army, player.Goods.Inventory.StageProp	, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.StageProp		, player.Squad, player.Selection, player.Goods.CompletedResearch.StageProp		, definitionsStageProp		, modifiersStageProp	, modifierTypeName, entityTypePlural, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedFacility	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; static const god::glabel modifierTypeName = "Architectonics"	, entityTypePlural = "Facilities"	; return equipIfResearched(selectedChoice, player.Army, player.Goods.Inventory.Facility		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Facility		, player.Squad, player.Selection, player.Goods.CompletedResearch.Facility		, definitionsFacility		, modifiersFacility		, modifierTypeName, entityTypePlural, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedVehicle	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; static const god::glabel modifierTypeName = "Transportation"	, entityTypePlural = "Vehicles"		; return equipIfResearched(selectedChoice, player.Army, player.Goods.Inventory.Vehicle		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Vehicle		, player.Squad, player.Selection, player.Goods.CompletedResearch.Vehicle		, definitionsVehicle		, modifiersVehicle		, modifierTypeName, entityTypePlural, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedProfession	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; static const god::glabel modifierTypeName = "Rank"				, entityTypePlural = "Licenses"		; return equipIfResearched(selectedChoice, player.Army, player.Goods.Inventory.Profession	, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Profession		, player.Squad, player.Selection, player.Goods.CompletedResearch.Profession		, definitionsProfession		, modifiersProfession	, modifierTypeName, entityTypePlural, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedWeapon		(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; static const god::glabel modifierTypeName = "Science"			, entityTypePlural = "Weapons"		; return equipIfResearched(selectedChoice, player.Army, player.Goods.Inventory.Weapon		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Weapon			, player.Squad, player.Selection, player.Goods.CompletedResearch.Weapon			, definitionsWeapon			, modifiersWeapon		, modifierTypeName, entityTypePlural, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedArmor		(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; static const god::glabel modifierTypeName = "Technology"		, entityTypePlural = "Armors"		; return equipIfResearched(selectedChoice, player.Army, player.Goods.Inventory.Armor		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Armor			, player.Squad, player.Selection, player.Goods.CompletedResearch.Armor			, definitionsArmor			, modifiersArmor		, modifierTypeName, entityTypePlural, instanceGame.UserMessage, instanceGame.UserError);};
//bool equipIfResearchedItems			(SGame& instanceGame, int16_t selectedChoice){ return false; } //SPlayer& player = instanceGame.Player; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Items		, player.CurrentEquip.Items			, player.Squad, player.Selection, player.CompletedResearch.Items			, definitionsItems			, modifiersItems		, ""		, entityTypePlural, messageSuccess, messageError);};

SGameState drawEquipMenu(SGame& instanceGame, const SGameState& returnState)
{
#define MAX_BUY_ITEMS 1024
#define SHOP_EXIT_VALUE ((MAX_BUY_ITEMS)*2)

	static klib::SMenuItem<int32_t> menuItems[MAX_BUY_ITEMS+1] = {};

	size_t itemCount=0, iCharacter, iSlot, count;
	bool bAlreadyInSquad;

	SPlayer&				player			= instanceGame.Players[PLAYER_USER];
	SCharacterGoods&		playerCompany	= instanceGame.Players[PLAYER_USER].Goods;
	SCharacterInventory&	playerInventory	= playerCompany.Inventory;
	SGlobalDisplay&	display	= instanceGame.GlobalDisplay;

	int16_t selectedChoice=0;
	std::string menuTitle = "Equip ";

	if( player.Selection.PlayerUnit != -1 && player.Squad.Agents[player.Selection.PlayerUnit] != -1 && GAME_SUBSTATE_CHARACTER != instanceGame.State.Substate)
		menuTitle = player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name + ": ";
	else if(player.Selection.PlayerUnit != -1)
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1);

	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	menuTitle += labelAccessory	.c_str();	for(iSlot=0, itemCount=playerInventory.Accessory	.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getAccessoryName		(playerInventory.Accessory		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_STAGEPROP	:	menuTitle += labelStageProp	.c_str();	for(iSlot=0, itemCount=playerInventory.StageProp	.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getStagePropName		(playerInventory.StageProp		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_FACILITY		:	menuTitle += labelFacility	.c_str();	for(iSlot=0, itemCount=playerInventory.Facility		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getFacilityName			(playerInventory.Facility		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_VEHICLE		:	menuTitle += labelVehicle	.c_str();	for(iSlot=0, itemCount=playerInventory.Vehicle		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getVehicleName			(playerInventory.Vehicle		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_PROFESSION	:	menuTitle += labelJob		.c_str();	for(iSlot=0, itemCount=playerInventory.Profession	.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getProfessionName		(playerInventory.Profession		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_WEAPON		:	menuTitle += labelWeapon	.c_str();	for(iSlot=0, itemCount=playerInventory.Weapon		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getWeaponName			(playerInventory.Weapon			.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ARMOR		:	menuTitle += labelArmor		.c_str();	for(iSlot=0, itemCount=playerInventory.Armor		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getArmorName			(playerInventory.Armor			.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ITEM			:	menuTitle += labelItem		.c_str();	for(iSlot=0, itemCount=playerInventory.Items		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getItemName				(playerInventory.Items			.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_CHARACTER	:	
		for(iCharacter=0, count=player.Army.size(); iCharacter<count; ++iCharacter)	
		{	
			bAlreadyInSquad = false;
			for(iSlot=0; iSlot<ktools::size(player.Squad.Agents); ++iSlot )
				if(player.Squad.Agents[iSlot] == iCharacter)
				{
					bAlreadyInSquad = true;
					break;
				}

			if(bAlreadyInSquad)
				continue;

			menuItems[itemCount++]	= { (int32_t)iCharacter, player.Army[iCharacter].Name }; 
		}
		selectedChoice	= drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	
		break;
	default:
		break;
	}

	if( selectedChoice == -1 ) {
		return returnState;
	}
	else if(selectedChoice == SHOP_EXIT_VALUE) {
		if( player.Selection.PlayerUnit == -1 || player.Squad.Agents[player.Selection.PlayerUnit] == -1)
			return { GAME_STATE_MENU_SQUAD_SETUP };
		else
			return { GAME_STATE_MENU_EQUIPMENT };
	}

	SGameState retVal = returnState;

	SEntity selectedItem = {0,0,0};
	instanceGame.UserMessage = instanceGame.UserError = "";
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	if(equipIfResearchedAccessory	(instanceGame, selectedChoice)) retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	case GAME_SUBSTATE_STAGEPROP	:	if(equipIfResearchedStageProp	(instanceGame, selectedChoice)) retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	case GAME_SUBSTATE_FACILITY		:	if(equipIfResearchedFacility	(instanceGame, selectedChoice)) retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	case GAME_SUBSTATE_VEHICLE		:	if(equipIfResearchedVehicle		(instanceGame, selectedChoice)) retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	case GAME_SUBSTATE_PROFESSION	:	if(equipIfResearchedProfession	(instanceGame, selectedChoice)) retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	case GAME_SUBSTATE_WEAPON		:	if(equipIfResearchedWeapon		(instanceGame, selectedChoice)) retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	case GAME_SUBSTATE_ARMOR		:	if(equipIfResearchedArmor		(instanceGame, selectedChoice)) retVal = { GAME_STATE_MENU_EQUIPMENT }; break;
	case GAME_SUBSTATE_ITEM			:	break;
	case GAME_SUBSTATE_CHARACTER	:	
		player.Squad.Agents[player.Selection.PlayerUnit] = selectedChoice; 
		klib::resetCursorString(instanceGame.SlowMessage);
		instanceGame.UserMessage = "You assigned " + player.Army[selectedChoice].Name + " as Agent #" + std::to_string(player.Selection.PlayerUnit+1) + "";
		retVal = { GAME_STATE_MENU_EQUIPMENT }; 
		break;
	default:
		break;
	}

	return retVal;
}


template <typename _TEntity, size_t _Width, size_t _Depth, size_t _SizeDefinitions, size_t _SizeModifiers> 
void drawEntityDetail(SWeightedDisplay<_Width, _Depth>& display_, int32_t offsetY, int32_t offsetX, const _TEntity& entity, const SEntityRecord<_TEntity> (&definitions)[_SizeDefinitions], const SEntityRecord<_TEntity> (&modifiers)[_SizeModifiers], const god::glabel& entityType)
{
	printfToGrid(display_.Screen		, offsetY++, offsetX, ktools::LEFT, "-- %s:", entityType.c_str());
	uint16_t color = COLOR_GREEN;
	valueToGrid(display_.TextAttributes	, offsetY-1, offsetX, ktools::LEFT, &color, 1, (int32_t)entityType.size()+4);

	std::string nameAndLevelText = getEntityName(entity, definitions, modifiers) + " Lv. " + std::to_string(entity.Level);	
	lineToGrid(display_.Screen			, ++offsetY	, offsetX, ktools::LEFT, nameAndLevelText.c_str() );
	valueToGrid(display_.TextAttributes	, offsetY	, offsetX, ktools::LEFT, &(color = COLOR_YELLOW), 1, (int32_t)nameAndLevelText.size());

	const SEntityPoints entityPoints = getEntityPoints(entity, definitions, modifiers);
	static const god::glabel formatPoints	= "%-21.21s: %-10.10s";
	static const god::glabel formatCoins	= "%-21.21s: %-11.11s";
	offsetY+=2;
	nameAndLevelText = std::to_string		(entityPoints.LifeMax.Health				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Max Health"				, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeMax.Shield				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Max Shield"				, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeMax.Mana					);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Max Mana"				, nameAndLevelText.c_str());
	offsetY+=1;
	nameAndLevelText = std::to_string		(entityPoints.LifeCurrent.Health			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Health per turn"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeCurrent.Shield			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Shield per turn"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeCurrent.Mana				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Mana per turn"			, nameAndLevelText.c_str());
	offsetY+=1;
	nameAndLevelText = std::to_string		(entityPoints.Attack.Hit					);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Hit Chance"				, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Damage					);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Damage"					, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.DirectDamage.Health	);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Direct Damage Health"	, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.DirectDamage.Shield	);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Direct Damage Shield"	, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.DirectDamage.Mana		);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Direct Damage Mana"		, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Absorption				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Absorption"				, nameAndLevelText.c_str());
	offsetY+=1;
	nameAndLevelText = std::to_string		(entityPoints.Attack.Speed.Attack			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Attack Speed"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Speed.Movement			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Movement Speed"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Speed.Reflexes			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, formatPoints.c_str()	, "Reflexes"				, nameAndLevelText.c_str());

	nameAndLevelText = std::to_string		(entityPoints.Coins							);	printfToGrid(display_.Screen, ++offsetY	, offsetX, ktools::LEFT, formatCoins.c_str()	, "Coins per turn"			, nameAndLevelText.c_str());
	valueToGrid(display_.TextAttributes, offsetY, offsetX+23, ktools::LEFT, &(color = (entityPoints.Coins >= 0) ? COLOR_ORANGE : COLOR_RED), 1, 11);
	nameAndLevelText = std::to_string		(entityPoints.PriceBuy/2					);	printfToGrid(display_.Screen, ++offsetY	, offsetX, ktools::LEFT, formatCoins.c_str()	, "Sell Price"				, nameAndLevelText.c_str());
	valueToGrid(display_.TextAttributes, offsetY, offsetX+23, ktools::LEFT, &(color = COLOR_ORANGE), 1, 11);
	nameAndLevelText = std::to_string		(entityPoints.CostMaintenance				);	printfToGrid(display_.Screen, ++offsetY	, offsetX, ktools::LEFT, formatCoins.c_str()	, "Maintenance Cost"		, nameAndLevelText.c_str());
	valueToGrid(display_.TextAttributes, offsetY, offsetX+23, ktools::LEFT, &(color = COLOR_ORANGE), 1, 11);
}


void drawCharacterDetail(SGame& instanceGame)
{
}
#define MAX_ENTITY_COLUMNS	4

SGameState drawEquip(SGame& instanceGame, const SGameState& returnState)
{
	SPlayer& player = instanceGame.Players[PLAYER_USER];
	SGlobalDisplay& display = instanceGame.GlobalDisplay;
	uint16_t color = COLOR_GREEN;
	
	std::string menuTitle = "Agent Setup";
	if( player.Selection.PlayerUnit != -1 && player.Squad.Agents[player.Selection.PlayerUnit] != -1) 
	{
		static const int32_t slotWidth		= display.Width / MAX_ENTITY_COLUMNS;
		static const int32_t slotRowSpace	= 28;// display.Depth / (MAX_AGENT_ROWS);
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1) + ": "+ player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name + ".";

		int32_t offsetY = TACTICAL_DISPLAY_POSY-3, offsetX;
		drawEntityDetail(display, offsetY		, offsetX = 3, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Profession	, definitionsProfession	, modifiersProfession	, labelJob			);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Weapon		, definitionsWeapon		, modifiersWeapon		, labelWeapon		);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Armor			, definitionsArmor		, modifiersArmor		, labelArmor		);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Accessory		, definitionsAccessory	, modifiersAccessory	, labelAccessory	);
		drawEntityDetail(display, offsetY+=28	, offsetX = 3, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Vehicle		, definitionsVehicle	, modifiersVehicle		, labelVehicle		);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Facility		, definitionsFacility	, modifiersFacility		, labelFacility		);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.StageProp		, definitionsStageProp	, modifiersStageProp	, labelStageProp	);
		displayAgentSlot(display, offsetY		, offsetX+=48, player.Selection.PlayerUnit+1, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]], false);
	}
	else if(player.Selection.PlayerUnit != -1)
	{
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1) + ": Open position.";
	}

	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuTitle, optionsEquip, instanceGame.FrameInput, {GAME_STATE_MENU_SQUAD_SETUP}, instanceGame.State, 30);
	}
	else 
	{
		if( player.Selection.PlayerUnit >= (int16_t)ktools::size(player.Squad.Agents))
			player.Selection.PlayerUnit = -1;

		return drawEquipMenu(instanceGame, returnState);
	}

	return returnState;
};


