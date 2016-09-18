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
,	const std::string& modifierTypeName
,	const std::string& entityTypePlural
,	std::string& messageSuccess
,	std::string& messageError
)
{
	_EntityType selectedItem = playerInventory.Slots[selectedChoice].Entity;	
	if(selectedItem.Definition	&&	playerCompletedResearch.Definitions.FindElement(selectedItem.Definition) == -1)	
	{	
		messageError = "You can't use " + entityDefinitions[selectedItem.Definition].Name	+ " without researching it first!"; 
		return false; 
	}	
	else if(selectedItem.Modifier && playerCompletedResearch.Modifiers.FindElement(selectedItem.Modifier) == -1) 
	{	
		char modifierName[128] = {}; 
		sprintf_s(modifierName, entityModifiers[selectedItem.Modifier].Name.c_str(), modifierTypeName.c_str());	
		messageError = "You can't use " + std::string(modifierName) + " " + entityTypePlural + " without researching them first!"; 
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

static bool equipIfResearchedAccessory	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Accessory	, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Accessory		, player.Squad, player.Selection, player.CompletedResearch.Accessory		, definitionsAccessory		, modifiersAccessory	, "Crafting"		, "Accessories"	, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedStageProp	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; return equipIfResearched(selectedChoice, player.Army, player.Inventory.StageProp	, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.StageProp		, player.Squad, player.Selection, player.CompletedResearch.StageProp		, definitionsStageProp		, modifiersStageProp	, ""				, "Stage Props"	, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedFacility	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Facility		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Facility		, player.Squad, player.Selection, player.CompletedResearch.Facility			, definitionsFacility		, modifiersFacility		, "Architectonics"	, "Facilities"	, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedVehicle	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Vehicle		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Vehicle		, player.Squad, player.Selection, player.CompletedResearch.Vehicle			, definitionsVehicle		, modifiersVehicle		, "Transportation"	, "Vehicles"	, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedProfession	(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Profession	, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Profession		, player.Squad, player.Selection, player.CompletedResearch.Profession		, definitionsProfession		, modifiersProfession	, "Rank"			, "Licenses"	, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedWeapon		(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Weapon		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Weapon			, player.Squad, player.Selection, player.CompletedResearch.Weapon			, definitionsWeapon			, modifiersWeapon		, "Science"			, "Weapons"		, instanceGame.UserMessage, instanceGame.UserError);};
static bool equipIfResearchedArmor		(SGame& instanceGame, int16_t selectedChoice){ SPlayer& player = instanceGame.Players[PLAYER_USER]; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Armor		, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Armor			, player.Squad, player.Selection, player.CompletedResearch.Armor			, definitionsArmor			, modifiersArmor		, "Technology"		, "Armors"		, instanceGame.UserMessage, instanceGame.UserError);};
//bool equipIfResearchedItems			(SGame& instanceGame, int16_t selectedChoice){ return false; } //SPlayer& player = instanceGame.Player; return equipIfResearched(selectedChoice, player.Army, player.Inventory.Items		, player.CurrentEquip.Items			, player.Squad, player.Selection, player.CompletedResearch.Items			, definitionsItems			, modifiersItems		, ""		, entityTypePlural, messageSuccess, messageError);};

SGameState drawEquipMenu(SGame& instanceGame, const SGameState& returnState)
{
#define MAX_BUY_ITEMS 1024
#define SHOP_EXIT_VALUE ((MAX_BUY_ITEMS)*2)

	static klib::SMenuItem<int32_t> menuItems[MAX_BUY_ITEMS+1] = {};

	size_t itemCount=0, iCharacter, iSlot, count;
	bool bAlreadyInSquad;

	SPlayer&		player	= instanceGame.Players[PLAYER_USER];
	SGlobalDisplay&	display	= instanceGame.GlobalDisplay;

	int16_t selectedChoice=0;
	std::string menuTitle = "Equip ";

	if( player.Selection.PlayerUnit != -1 && player.Squad.Agents[player.Selection.PlayerUnit] != -1 && GAME_SUBSTATE_CHARACTER != instanceGame.State.Substate)
		menuTitle = player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name + ": ";
	else if(player.Selection.PlayerUnit != -1)
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1);

	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	menuTitle += "Accessory"	;	for(iSlot=0, itemCount=player.Inventory.Accessory	.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getAccessoryName		(player.Inventory.Accessory		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_STAGEPROP	:	menuTitle += "Stage Prop"	;	for(iSlot=0, itemCount=player.Inventory.StageProp	.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getStagePropName		(player.Inventory.StageProp		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_FACILITY		:	menuTitle += "Facility"		;	for(iSlot=0, itemCount=player.Inventory.Facility	.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getFacilityName			(player.Inventory.Facility		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_VEHICLE		:	menuTitle += "Vehicle"		;	for(iSlot=0, itemCount=player.Inventory.Vehicle		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getVehicleName			(player.Inventory.Vehicle		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_PROFESSION	:	menuTitle += "Job License"	;	for(iSlot=0, itemCount=player.Inventory.Profession	.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getProfessionName		(player.Inventory.Profession	.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_WEAPON		:	menuTitle += "Weapon"		;	for(iSlot=0, itemCount=player.Inventory.Weapon		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getWeaponName			(player.Inventory.Weapon		.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ARMOR		:	menuTitle += "Armor"		;	for(iSlot=0, itemCount=player.Inventory.Armor		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getArmorName			(player.Inventory.Armor			.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
	case GAME_SUBSTATE_ITEM			:	menuTitle += "Item"			;	for(iSlot=0, itemCount=player.Inventory.Items		.Count; iSlot<itemCount; ++iSlot)	menuItems[iSlot] = { (int32_t)iSlot,	getItemName				(player.Inventory.Items			.Slots[iSlot].Entity)	}; selectedChoice = drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");	break;
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
void drawEntityDetail(SWeightedDisplay<_Width, _Depth>& display_, int32_t offsetY, int32_t offsetX, const _TEntity& entity, const SEntityRecord<_TEntity> (&definitions)[_SizeDefinitions], const SEntityRecord<_TEntity> (&modifiers)[_SizeModifiers], const std::string& entityType)
{
	printfToGrid(display_.Screen		, offsetY++, offsetX, ktools::LEFT, "-- %s:", entityType.c_str());
	uint16_t color = COLOR_GREEN;
	valueToGrid(display_.TextAttributes	, offsetY-1, offsetX, ktools::LEFT, &color, 1, (int32_t)entityType.size()+4);

	std::string nameAndLevelText = getEntityName(entity, definitions, modifiers) + " Lv. " + std::to_string(entity.Level);	
	lineToGrid(display_.Screen			, ++offsetY	, offsetX, ktools::LEFT, nameAndLevelText.c_str() );
	valueToGrid(display_.TextAttributes	, offsetY	, offsetX, ktools::LEFT, &(color = COLOR_YELLOW), 1, (int32_t)nameAndLevelText.size());

	const SEntityPoints entityPoints = getEntityPoints(entity, definitions, modifiers);
	offsetY+=2;
	nameAndLevelText = std::to_string		(entityPoints.LifeMax.Health				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Max Health"				, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeMax.Shield				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Max Shield"				, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeMax.Mana					);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Max Mana"				, nameAndLevelText.c_str());
	offsetY+=1;
	nameAndLevelText = std::to_string		(entityPoints.LifeCurrent.Health			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Health per turn"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeCurrent.Shield			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Shield per turn"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.LifeCurrent.Mana				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Mana per turn"			, nameAndLevelText.c_str());
	offsetY+=1;
	nameAndLevelText = std::to_string		(entityPoints.Attack.Hit					);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Hit Chance"				, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Damage					);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Damage"					, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.DirectDamage.Health	);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Health"	, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.DirectDamage.Shield	);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Shield"	, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.DirectDamage.Mana		);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Direct Damage Mana"		, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Absorption				);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Absorption"				, nameAndLevelText.c_str());
	offsetY+=1;
	nameAndLevelText = std::to_string		(entityPoints.Attack.Speed.Attack			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Attack Speed"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Speed.Movement			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Movement Speed"			, nameAndLevelText.c_str());
	nameAndLevelText = std::to_string		(entityPoints.Attack.Speed.Reflexes			);	printfToGrid(display_.Screen, offsetY++	, offsetX, ktools::LEFT, "%-21.21s: %-10.10s"	, "Reflexes"				, nameAndLevelText.c_str());

	nameAndLevelText = std::to_string		(entityPoints.Coins							);	printfToGrid(display_.Screen, ++offsetY	, offsetX, ktools::LEFT, "%-21.21s: %-11.11s"	, "Coins per turn"			, nameAndLevelText.c_str());
	valueToGrid(display_.TextAttributes, offsetY, offsetX+23, ktools::LEFT, &(color = (entityPoints.Coins >= 0) ? COLOR_ORANGE : COLOR_RED), 1, 11);
	nameAndLevelText = std::to_string		(entityPoints.PriceBuy/2					);	printfToGrid(display_.Screen, ++offsetY	, offsetX, ktools::LEFT, "%-21.21s: %-11.11s"	, "Sell Price"				, nameAndLevelText.c_str());
	valueToGrid(display_.TextAttributes, offsetY, offsetX+23, ktools::LEFT, &(color = COLOR_ORANGE), 1, 11);
	nameAndLevelText = std::to_string		(entityPoints.CostMaintenance				);	printfToGrid(display_.Screen, ++offsetY	, offsetX, ktools::LEFT, "%-21.21s: %-11.11s"	, "Maintenance Cost"		, nameAndLevelText.c_str());
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
		drawEntityDetail(display, offsetY		, offsetX = 3, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Profession	, definitionsProfession	, modifiersProfession	, "Job License"	);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Weapon		, definitionsWeapon		, modifiersWeapon		, "Weapon"		);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Armor			, definitionsArmor		, modifiersArmor		, "Armor"		);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Accessory		, definitionsAccessory	, modifiersAccessory	, "Accessory"	);
		drawEntityDetail(display, offsetY+=28	, offsetX = 3, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Vehicle		, definitionsVehicle	, modifiersVehicle		, "Vehicle"		);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.Facility		, definitionsFacility	, modifiersFacility		, "Facility"	);
		drawEntityDetail(display, offsetY		, offsetX+=48, player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].CurrentEquip.StageProp		, definitionsStageProp	, modifiersStageProp	, "Stage Prop"	);
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


