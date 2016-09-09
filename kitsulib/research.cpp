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

using namespace klib;

template<typename _EntityType>
void addIfResearchable
(	klib::SResearchGroup<_EntityType>& researchableItems
,	const _EntityType& entity
,	const klib::SResearchGroup<_EntityType>& researchCompleted
,	bool bIsParallelDefinition		= false
,	bool bIsParallelModifier		= false
,	const _EntityType& maxResearch	= {0,0,1}
)
{
	if(entity.Definition && -1 == researchCompleted.Definitions.FindElement(entity.Definition) && (bIsParallelDefinition || entity.Definition <= (maxResearch.Definition+1)) )
		researchableItems.Definitions.AddElement(entity.Definition);

	if(entity.Modifier && -1 == researchCompleted.Modifiers.FindElement(entity.Modifier) && (bIsParallelModifier || entity.Modifier <= (maxResearch.Modifier+1)) ) 
		researchableItems.Modifiers.AddElement(entity.Modifier);
}


// Returns the possible research for a given type of researchable
template<typename _EntityType, size_t _Size>
void generateResearchableList(klib::SResearchGroup<_EntityType>& researchableItems, const klib::SEntityContainer<_EntityType, _Size>& playerInventory, const klib::SResearchGroup<_EntityType>& researchCompleted
,	bool bIsParallelDefinition		= false
,	bool bIsParallelModifier		= false
,	const _EntityType& maxResearch	= {0,0,1}
)
{
	for(uint32_t iSoldierSlot = 0; iSoldierSlot < playerInventory.Count; ++iSoldierSlot) {
		const _EntityType& entity = playerInventory.Slots[iSoldierSlot].Entity;
		addIfResearchable( researchableItems, entity, researchCompleted, bIsParallelDefinition, bIsParallelModifier, maxResearch );
	}
}

// Returns all the possible research for a given player
void generateResearchableList(klib::SCharacterResearch& researchableItems, const klib::SCharacterInventory& playerInventory, const klib::SCharacterResearch& researchCompleted) //SGame& instanceGame, GAME_STATE returnState)
{
	for(uint32_t iSoldierSlot = 0; iSoldierSlot < playerInventory.Accessory.Count; ++iSoldierSlot) {
		generateResearchableList(researchableItems.Accessory	, playerInventory.Accessory		, researchCompleted.Accessory	);
		generateResearchableList(researchableItems.Armor		, playerInventory.Armor			, researchCompleted.Armor		);
		generateResearchableList(researchableItems.Weapon		, playerInventory.Weapon		, researchCompleted.Weapon		);
		generateResearchableList(researchableItems.Profession	, playerInventory.Profession	, researchCompleted.Profession	);
		generateResearchableList(researchableItems.Vehicle		, playerInventory.Vehicle		, researchCompleted.Vehicle		);
		generateResearchableList(researchableItems.Facility		, playerInventory.Facility		, researchCompleted.Facility	);
		generateResearchableList(researchableItems.StageProp	, playerInventory.StageProp		, researchCompleted.StageProp	);
	}
}

template<typename _EntityType, size_t _Size>
void generateResearchableListFromAgent(klib::SResearchGroup<_EntityType>& researchableItems, const _EntityType& equippedEntity, const klib::SEntityContainer<_EntityType, _Size>& agentInventory, const klib::SResearchGroup<_EntityType>& researchCompleted
,	bool bIsParallelDefinition		= false
,	bool bIsParallelModifier		= false
,	const _EntityType& maxResearch	= {0,0,1}
)
{
	addIfResearchable( researchableItems, equippedEntity, researchCompleted, bIsParallelDefinition, bIsParallelModifier, maxResearch);
	generateResearchableList(researchableItems, agentInventory, researchCompleted, bIsParallelDefinition, bIsParallelModifier, maxResearch);
}

struct SResearchable 
{
	int32_t		ResearchIndex;
	bool		IsModifier;
	std::string	Name;
};

template <typename _EntityType>
void completeResearch(const SResearchable& selectedChoice, _EntityType& maxResearch, SResearchGroup<_EntityType>& researchableItems, SResearchGroup<_EntityType>& researchCompleted, std::string& successMessage)
{
	if(selectedChoice.IsModifier)	
	{
		researchCompleted.Modifiers.AddElement(researchableItems.Modifiers.Slots[selectedChoice.ResearchIndex].Entity); 
		maxResearch.Modifier	= std::max(maxResearch.Modifier,	researchableItems.Modifiers.Slots[selectedChoice.ResearchIndex].Entity);
	}
	else 
	{
		researchCompleted.Definitions.AddElement(researchableItems.Definitions.Slots[selectedChoice.ResearchIndex].Entity); 
		maxResearch.Definition	= std::max(maxResearch.Definition,	researchableItems.Definitions.Slots[selectedChoice.ResearchIndex].Entity);
	}
	successMessage = "You have successfully researched " + selectedChoice.Name + ".";
}

SGameState drawResearchMenu(SGame& instanceGame, const SGameState& returnState)
{
	klib::SCharacterResearch& researchCompleted = instanceGame.Player.CompletedResearch;
	klib::SCharacterResearch  researchableItems;
	SPlayer& player = instanceGame.Player;


#define GET_AVAILABLE_RESEARCH_FOR_ENTITY(EntityToken_, ProgressiveDefinitions_, ProgressiveModifiers_)																\
		generateResearchableList(researchableItems.EntityToken_, player.Inventory.EntityToken_, researchCompleted.EntityToken_										\
			, ProgressiveDefinitions_, ProgressiveModifiers_, player.MaxResearch.EntityToken_);																		\
		for(iAgent=0; iAgent<armySize; ++iAgent) 																													\
			generateResearchableListFromAgent( researchableItems.EntityToken_ 																						\
				, player.Army[iAgent].CurrentEquip.EntityToken_ 																									\
				, player.Army[iAgent].Inventory.EntityToken_ 																										\
				, researchCompleted.EntityToken_																													\
				, ProgressiveDefinitions_, ProgressiveModifiers_, player.MaxResearch.EntityToken_																	\
			);																																						\
																																									\
		researchableDefinitions	+= researchableItems.EntityToken_.Definitions.Count;																				\
		researchableModifiers	+= researchableItems.EntityToken_.Modifiers.Count;

	int32_t iAgent;
	const int32_t armySize	= (int32_t)player.Army.size();
	uint32_t researchableDefinitions=0, researchableModifiers=0;
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	: GET_AVAILABLE_RESEARCH_FOR_ENTITY(Accessory	, false, false); break;
	case GAME_SUBSTATE_STAGEPROP	: GET_AVAILABLE_RESEARCH_FOR_ENTITY(StageProp	, true , false); break;
	case GAME_SUBSTATE_FACILITY		: GET_AVAILABLE_RESEARCH_FOR_ENTITY(Facility	, false, false); break;
	case GAME_SUBSTATE_VEHICLE		: GET_AVAILABLE_RESEARCH_FOR_ENTITY(Vehicle		, false, false); break;
	case GAME_SUBSTATE_PROFESSION	: GET_AVAILABLE_RESEARCH_FOR_ENTITY(Profession	, true , false); break;
	case GAME_SUBSTATE_WEAPON		: GET_AVAILABLE_RESEARCH_FOR_ENTITY(Weapon		, false, false); break;
	case GAME_SUBSTATE_ARMOR		: GET_AVAILABLE_RESEARCH_FOR_ENTITY(Armor		, false, false); break;
	default:
		break;
	}

#define MAX_RESEARCH_ITEMS	64
	static klib::SMenuItem<SResearchable> menuItems[MAX_RESEARCH_ITEMS] = {};

	uint32_t researchableCount=0;
	for(uint32_t i=0, count=std::min(MAX_RESEARCH_ITEMS, (int32_t)researchableDefinitions); i<count; ++i) 
	{
		menuItems[researchableCount].ReturnValue.ResearchIndex	= i;
		menuItems[researchableCount].ReturnValue.IsModifier		= false;
		switch(instanceGame.State.Substate)
		{
		case GAME_SUBSTATE_ACCESSORY	:	menuItems[researchableCount].ReturnValue.Name = menuItems[researchableCount].Text	= definitionsAccessory		[researchableItems.Accessory	.Definitions.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_STAGEPROP	:	menuItems[researchableCount].ReturnValue.Name = menuItems[researchableCount].Text	= definitionsStageProp		[researchableItems.StageProp	.Definitions.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_FACILITY		:	menuItems[researchableCount].ReturnValue.Name = menuItems[researchableCount].Text	= definitionsFacility		[researchableItems.Facility		.Definitions.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_VEHICLE		:	menuItems[researchableCount].ReturnValue.Name = menuItems[researchableCount].Text	= definitionsVehicle		[researchableItems.Vehicle		.Definitions.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_PROFESSION	:	menuItems[researchableCount].ReturnValue.Name = menuItems[researchableCount].Text	= definitionsProfession		[researchableItems.Profession	.Definitions.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_WEAPON		:	menuItems[researchableCount].ReturnValue.Name = menuItems[researchableCount].Text	= definitionsWeapon			[researchableItems.Weapon		.Definitions.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_ARMOR		:	menuItems[researchableCount].ReturnValue.Name = menuItems[researchableCount].Text	= definitionsArmor			[researchableItems.Armor		.Definitions.Slots[i].Entity	].Name; break;
		default:
			break;
		}
		researchableCount++;
	}
	
	for(uint32_t i=0, count=std::min(MAX_RESEARCH_ITEMS-researchableCount, researchableModifiers); i<count; ++i) 
	{
		menuItems[researchableCount].ReturnValue.ResearchIndex	= i;
		menuItems[researchableCount].ReturnValue.IsModifier		= true;
		switch(instanceGame.State.Substate)
		{
		case GAME_SUBSTATE_ACCESSORY	:	menuItems[researchableCount].Text	= modifiersAccessory		[researchableItems.Accessory	.Modifiers.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_STAGEPROP	:	menuItems[researchableCount].Text	= modifiersStageProp		[researchableItems.StageProp	.Modifiers.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_FACILITY		:	menuItems[researchableCount].Text	= modifiersFacility			[researchableItems.Facility		.Modifiers.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_VEHICLE		:	menuItems[researchableCount].Text	= modifiersVehicle			[researchableItems.Vehicle		.Modifiers.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_PROFESSION	:	menuItems[researchableCount].Text	= modifiersProfession		[researchableItems.Profession	.Modifiers.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_WEAPON		:	menuItems[researchableCount].Text	= modifiersWeapon			[researchableItems.Weapon		.Modifiers.Slots[i].Entity	].Name; break;
		case GAME_SUBSTATE_ARMOR		:	menuItems[researchableCount].Text	= modifiersArmor			[researchableItems.Armor		.Modifiers.Slots[i].Entity	].Name; break;
		default:
			break;
		}

		char composite[128] ={};
			 if(instanceGame.State.Substate == GAME_SUBSTATE_ACCESSORY		) sprintf_s(composite, menuItems[researchableCount].Text.c_str(), "Crafting Technique"	);
		else if(instanceGame.State.Substate == GAME_SUBSTATE_STAGEPROP		) sprintf_s(composite, menuItems[researchableCount].Text.c_str(), "Enhacement"			);
		else if(instanceGame.State.Substate == GAME_SUBSTATE_FACILITY		) sprintf_s(composite, menuItems[researchableCount].Text.c_str(), "Architectonics"		);
		else if(instanceGame.State.Substate == GAME_SUBSTATE_VEHICLE		) sprintf_s(composite, menuItems[researchableCount].Text.c_str(), "Transportation"		);
		else if(instanceGame.State.Substate == GAME_SUBSTATE_PROFESSION		) sprintf_s(composite, menuItems[researchableCount].Text.c_str(), "Rank"				);
		else if(instanceGame.State.Substate == GAME_SUBSTATE_WEAPON			) sprintf_s(composite, menuItems[researchableCount].Text.c_str(), "Science"				);
		else if(instanceGame.State.Substate == GAME_SUBSTATE_ARMOR			) sprintf_s(composite, menuItems[researchableCount].Text.c_str(), "Technology"			);

		menuItems[researchableCount].Text = composite;
		menuItems[researchableCount].ReturnValue.Name = composite;

		researchableCount++;
	}

	SResearchable selectedChoice;
	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], (size_t)researchableCount, "Available Research", menuItems, instanceGame.FrameInput, {(int32_t)researchableCount}, {-1}, 40U);
	if(selectedChoice.ResearchIndex == researchableCount)
		return {GAME_STATE_MENU_RESEARCH};

	if(selectedChoice.ResearchIndex == -1)
		return returnState;

	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	: completeResearch(selectedChoice, player.MaxResearch.	Accessory	, researchableItems.	Accessory	, researchCompleted.	Accessory	, instanceGame.UserMessage); break; 
	case GAME_SUBSTATE_STAGEPROP	: completeResearch(selectedChoice, player.MaxResearch.	StageProp	, researchableItems.	StageProp	, researchCompleted.	StageProp	, instanceGame.UserMessage); break; 
	case GAME_SUBSTATE_FACILITY		: completeResearch(selectedChoice, player.MaxResearch.	Facility	, researchableItems.	Facility	, researchCompleted.	Facility	, instanceGame.UserMessage); break; 
	case GAME_SUBSTATE_VEHICLE		: completeResearch(selectedChoice, player.MaxResearch.	Vehicle		, researchableItems.	Vehicle		, researchCompleted.	Vehicle		, instanceGame.UserMessage); break; 
	case GAME_SUBSTATE_PROFESSION	: completeResearch(selectedChoice, player.MaxResearch.	Profession	, researchableItems.	Profession	, researchCompleted.	Profession	, instanceGame.UserMessage); break; 
	case GAME_SUBSTATE_WEAPON		: completeResearch(selectedChoice, player.MaxResearch.	Weapon		, researchableItems.	Weapon		, researchCompleted.	Weapon		, instanceGame.UserMessage); break; 
	case GAME_SUBSTATE_ARMOR		: completeResearch(selectedChoice, player.MaxResearch.	Armor		, researchableItems.	Armor		, researchCompleted.	Armor		, instanceGame.UserMessage); break; 
	default:
		break;
	}

	return returnState;
}


SGameState drawResearch(SGame& instanceGame, const SGameState& returnState)
{
	const std::string textToPrint = "Research center.";

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	memcpy(&instanceGame.PostEffectDisplay.Screen.Cells[instanceGame.PostEffectDisplay.Depth/2][instanceGame.PostEffectDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
	if ( !bDonePrinting ) 
		return returnState;

	drawBubblesBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);
	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		static const SMenu<SGameState, size(optionsResearch)> menuResearch(optionsResearch, {GAME_STATE_WELCOME_COMMANDER}, "Research Center", 28);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuResearch, instanceGame.FrameInput, instanceGame.State);
	}
	else 
		return drawResearchMenu(instanceGame, returnState);

	return returnState;
};
