#include "draw.h"

#include "Profession.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "Facility.h"
#include "Vehicle.h"

using namespace klib;

void drawIntro(SGame& instanceGame);
static const std::string namesProgramming[] =
{	"Kitsu"											
};

static const std::string namesEverythingElse[] =
{	"Kitsu"											
};

void klib::drawAndPresentGame( SGame& instanceGame )
{
	static STimer frameMeasure;

	klib::clearASCIIBackBuffer( ' ' );

	klib::lineToScreen(klib::getASCIIBackBufferHeight()-2, 1, klib::RIGHT, "%s.", instanceGame.UserMessage.c_str());
	klib::lineToScreen(klib::getASCIIBackBufferHeight()-2, 1, klib::LEFT, "sizeof(SGame): %u.", sizeof(SGame));

	drawState(instanceGame);
	showMenu(instanceGame);

	frameMeasure.Frame();
	instanceGame.FrameTimer.Frame();
	klib::lineToScreen(1, 1, klib::LEFT, "Frame time: %.5f seconds.", instanceGame.FrameTimer.LastTimeSeconds);
	klib::lineToScreen(2, 1, klib::LEFT, "Frames last second: %f.", instanceGame.FrameTimer.FramesLastSecond);

	
	klib::presentASCIIBackBuffer();
}; 	// 


static const std::string namesSpecialThanks[] =
{	"Nethrim"												
,	"Gorby"													
,	"Andrew"												
,	"James"													
,	"Suelika"												
,	"Paul"													
,	"Bill"													
,	"Robert"												
,	"Kirk"													
,	"Pia"													
,	"Stuart"												
,	"Brent"													
,	"Riva"													
,	"Serguei"												
,	"David"													
,	"Nhi"													
,	"Andy"													
,	"Esteban"												
,	"Chi-Wei"												
,	"Matt"													
,	"Michael"												
,	"Henry"													
,	"Gabriela"												
,	"Marc"											
,	"Terran"												
,	"Adan"													
,	"Ted"													
,	"Fredrik"												
,	"Jose"													
,	"Jose"													
,	"Zach"													
,	"Cara"													
,	"Nicolas"												
,	"Martin"												
,	"Mauro"													
,	"Luis"														
,	"Carlos"												
,	"... and also to those which I'm going to add later to this list!"	
};

void addIfResearchable
(	klib::SResearchGroup& researchableItems
,	const klib::SEntity& entity
,	const klib::SResearchGroup& researchCompleted
)
{
	if(entity.Index && -1 == researchCompleted.Definitions.FindElement(entity.Index))
		researchableItems.Definitions.AddElement(entity.Index);

	if(entity.Modifier && -1 == researchCompleted.Modifiers.FindElement(entity.Modifier)) 
		researchableItems.Modifiers.AddElement(entity.Modifier);
}


template<typename _EntityType, size_t _Size>
void generateResearchableList(klib::SResearchGroup& researchableItems, const klib::SEntityContainer<_EntityType, _Size>& playerInventory, const klib::SResearchGroup& researchCompleted) //SGame& instanceGame, GAME_STATE returnState)
{
	for(uint32_t iSoldierSlot = 0; iSoldierSlot < playerInventory.Count; ++iSoldierSlot) {
		const _EntityType& entity = playerInventory.Slots[iSoldierSlot].Entity;
		addIfResearchable(researchableItems, entity, researchCompleted	);
	}
}

void generateResearchableList(klib::SCharacterResearch& researchableItems, const klib::SCharacterInventory& playerInventory, const klib::SCharacterResearch& researchCompleted) //SGame& instanceGame, GAME_STATE returnState)
{
	for(uint32_t iSoldierSlot = 0; iSoldierSlot < playerInventory.Accessories.Count; ++iSoldierSlot) {
		generateResearchableList(researchableItems.Accessory	, playerInventory.Accessories	, researchCompleted.Accessory	);
		generateResearchableList(researchableItems.Armor		, playerInventory.Armors		, researchCompleted.Armor		);
		generateResearchableList(researchableItems.Weapon		, playerInventory.Weapons		, researchCompleted.Weapon		);
		generateResearchableList(researchableItems.Profession	, playerInventory.Professions	, researchCompleted.Profession	);
		generateResearchableList(researchableItems.Vehicle		, playerInventory.Vehicles		, researchCompleted.Vehicle		);
		generateResearchableList(researchableItems.Facility		, playerInventory.Facilities	, researchCompleted.Facility	);
		generateResearchableList(researchableItems.StageProp	, playerInventory.StageProps	, researchCompleted.StageProp	);
	}
}

SGameState drawResearchMenu(SGame& instanceGame, SGameState returnState)
{

	klib::SCharacterResearch& researchCompleted = instanceGame.PlayerResearch;
	klib::SCharacterResearch  researchableItems;


	klib::SMenuItem<int32_t> menuItems[8] =
	{	{0,	""}
	,	{1,	""}
	,	{2,	""}
	,	{3,	""}
	,	{4,	""}
	,	{5,	""}
	,	{6,	""}
	,	{7,	""}
	};

	int32_t iAgent;
	const int32_t armySize	= instanceGame.GameArmies.Player.size();
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY:
		for(iAgent=0; iAgent<armySize; ++iAgent)
			generateResearchableList(researchableItems.Accessory, instanceGame.GameArmies.Player[iAgent].Inventory.Accessories, researchCompleted.Accessory);

		break;
	case GAME_SUBSTATE_STAGEPROP:
		for(iAgent=0; iAgent<armySize; ++iAgent)
			generateResearchableList(researchableItems.StageProp, instanceGame.GameArmies.Player[iAgent].Inventory.StageProps, researchCompleted.StageProp);

		break;
	case GAME_SUBSTATE_FACILITY:
		for(iAgent=0; iAgent<armySize; ++iAgent)
			generateResearchableList(researchableItems.Facility, instanceGame.GameArmies.Player[iAgent].Inventory.Facilities, researchCompleted.Facility);

		break;
	case GAME_SUBSTATE_VEHICLE:
		for(iAgent=0; iAgent<armySize; ++iAgent)
			generateResearchableList(researchableItems.Vehicle, instanceGame.GameArmies.Player[iAgent].Inventory.Vehicles, researchCompleted.Vehicle);

		break;
	case GAME_SUBSTATE_PROFESSION:
		for(iAgent=0; iAgent<armySize; ++iAgent)
			generateResearchableList(researchableItems.Profession, instanceGame.GameArmies.Player[iAgent].Inventory.Professions, researchCompleted.Profession);

		break;
	case GAME_SUBSTATE_WEAPON:
		for(iAgent=0; iAgent<armySize; ++iAgent)
			generateResearchableList(researchableItems.Weapon, instanceGame.GameArmies.Player[iAgent].Inventory.Weapons, researchCompleted.Weapon);

		break;
	case GAME_SUBSTATE_ARMOR:
		for(iAgent=0; iAgent<armySize; ++iAgent)
			generateResearchableList(researchableItems.Armor, instanceGame.GameArmies.Player[iAgent].Inventory.Armors, researchCompleted.Armor);

		break;
	default:
		break;
	}
	int32_t selectedChoice;
	selectedChoice = drawMenu("Available Research", menuItems, instanceGame.FrameInput, 50);
	if(selectedChoice == 50)
		return {GAME_STATE_MENU_CONTROL_CENTER};


	return instanceGame.State;
}


SGameState drawCredits(double lastFrameTime)
{
	static double offset = (double)klib::getASCIIBackBufferHeight();// (game::getASCIIBackBufferHeight()>>1)-4;
	int32_t curLine = (int32_t)offset;
	static int32_t maxDifference = curLine;
	int32_t curDifference = curLine;
	double bbHeight = (double)klib::getASCIIBackBufferHeight();
#define IN_RANGE(currentline) (currentline >= 0 && currentline < bbHeight)
	curLine = (int32_t)(curLine += 1);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "-- Loosely based on homework code and idea by --"	);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "Nethrim"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "-- Programming --"								);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "Kitsu"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "-- Beta Testing --"								);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "Gorby"											);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "Nethrim"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "-- Everything else --"							);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "Kitsu"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	klib::lineToScreen(curLine, 0, klib::CENTER, "-- Special Thanks to --"							);
	for(uint32_t i=0; i<klib::size(namesSpecialThanks); ++i) {
		curLine = (int32_t)(curLine += 2);	
		if(curLine >= 0 && curLine < bbHeight) 
			klib::lineToScreen(curLine, 0, klib::CENTER, "%s", namesSpecialThanks[i].c_str());
	}

	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < bbHeight)	klib::lineToScreen(curLine, 0, klib::CENTER, "Have a nice weekend!"					); 
	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < bbHeight)	klib::lineToScreen(curLine, 0, klib::CENTER, "Press ESC to return to main menu."	); 
	
	maxDifference = std::max(curLine - curDifference, maxDifference);

	offset -= lastFrameTime*6.0;

	if( offset <= -maxDifference )
		offset += klib::getASCIIBackBufferHeight()+maxDifference;

	return {GAME_STATE_CREDITS,};
}

template<size_t _Width, size_t _Depth>
void drawDisplay(SWeightedDisplay<_Width, _Depth>& source, uint32_t offset)	
{
	for(uint32_t z=0, maxZ=std::min(source.Depth, klib::getASCIIBackBufferHeight()-1-offset); z<maxZ; ++z) {
		char row[_Width+1] = {};
		memcpy(row, source.Screen.Cells[z], std::min(source.Width, klib::getASCIIBackBufferWidth()-1));
		klib::lineToScreen(offset, 0, klib::CENTER, row);
		++offset;
	}
};

void drawIntro( SGame& instanceGame )
{
	drawFireBackground(instanceGame.TacticalDisplay, instanceGame.FrameTimer.LastTimeSeconds);

	int32_t displayWidth	= (int32_t)instanceGame.TacticalDisplay.Width;
	int32_t displayDepth	= (int32_t)instanceGame.TacticalDisplay.Depth;

	static std::string words[] = {"Vulgar", "Display", "of", "Power"};//{ "A ", "Tactical", "Game" };
	//static std::string words[] = {"Vulgar Display of Power"};//{ "A ", "Tactical", "Game" };
	//static std::string words[4] = { "A ", "Tactical", "Game", "" };
	for( uint32_t i=0; i<klib::size(words); ++i)
		klib::lineToGrid(instanceGame.TacticalDisplay.Screen, (displayDepth/2)-(klib::size(words)/2)+i*2, 0, klib::CENTER, "%s", words[i].c_str());
	//memcpy(&instanceGame.TacticalDisplay.Screen.Cells[displayDepth/2-3][displayWidth/2-words[0].size()/2-1], words[0].c_str(), words[0].size());
	//memcpy(&instanceGame.TacticalDisplay.Screen.Cells[displayDepth/2-1][displayWidth/2-words[1].size()/2-1], words[1].c_str(), words[1].size());
	//memcpy(&instanceGame.TacticalDisplay.Screen.Cells[displayDepth/2+1][displayWidth/2-words[2].size()/2-1], words[2].c_str(), words[2].size());
	//memcpy(&instanceGame.TacticalDisplay.Screen.Cells[displayDepth	/2+3][displayWidth/2-words[3].size()/2-1], words[3].c_str(), words[3].size());
};



SGameState drawResearch(SGame& instanceGame, const SGameState& returnValue)
{
	std::string textToPrint = "Welcome back commander " + instanceGame.PlayerName + ".";

	bool bDonePrinting = drawMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	if ( bDonePrinting ) {
		return drawMenu(instanceGame.UserMessage = "Control Center", optionsControlCenter, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN,}, {GAME_STATE_WELCOME_COMMANDER,}, false, "Exit this menu");
	}

	return returnValue;
};

void klib::drawState( SGame& instanceGame )
{
	if(instanceGame.State.State == GAME_STATE_MENU_MAIN)
		drawIntro(instanceGame);
	
	if(instanceGame.State.State == GAME_STATE_CREDITS)
	{
		drawSnowBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);
		drawDisplay(instanceGame.GlobalDisplay, 0);
		drawCredits(instanceGame.FrameTimer.LastTimeSeconds);
	}
	else if(instanceGame.State.State == GAME_STATE_MENU_RESEARCH)
	{
		static const std::string textToPrint = "Resarch center.";

		bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
		memcpy(&instanceGame.TacticalDisplay.Screen.Cells[instanceGame.TacticalDisplay.Depth/2][instanceGame.TacticalDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
		if ( bDonePrinting )
			drawBubblesBackground(instanceGame.TacticalDisplay, instanceGame.FrameTimer.LastTimeSeconds);

		drawDisplay(instanceGame.TacticalDisplay, 5);
	}
	else if(instanceGame.State.State == GAME_STATE_MENU_SQUAD_SETUP)
	{
		drawRainBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);
		drawDisplay(instanceGame.GlobalDisplay, 0);
	}
	else
		drawDisplay(instanceGame.TacticalDisplay, 5);
}


void displayEmptySlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex)
{
	klib::lineToScreen(offsetY		, offsetX, klib::LEFT, "-- Agent #%u: %-36.36s", agentIndex, "Open position");
}

void displayAgentSlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex, klib::CCharacter& character, bool bAddFieldNames=false)
{
	std::string nameAndLevelText;
	klib::SEntityPoints agentPoints = klib::calculateFinalPoints( character );
	if( bAddFieldNames ) {
		nameAndLevelText = character.Name													;	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "-- Agent #%u: %-34.34s (Lv. %i)", agentIndex			, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getProfessionName	(character.CurrentEquip.Profession		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Class"				, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getWeaponName		(character.CurrentEquip.Weapon			);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Weapon"				, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = klib::getArmorName		(character.CurrentEquip.Armor			);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Armor"				, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = klib::getAccessoryName	(character.CurrentEquip.Accessory		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Accessory"			, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = klib::getVehicleName		(character.CurrentEquip.Vehicle			);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Vehicle"				, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
//		nameAndLevelText = klib::getFacilityName	(character.CurrentEquip.Facility		);	klib::lineToScreen(offsetY+++	, offsetX, klib::LEFT, "%-10.10s: %-35.35s (Lv. %i)"	, "Building assigned"	, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
		klib::lineToScreen(++offsetY, offsetX, klib::LEFT, "- Final Points:");
		offsetY+=2;
		nameAndLevelText = std::to_string			(agentPoints.LifeMax.Health				);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Health"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.LifeMax.Shield				);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Shield"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.LifeMax.Mana				);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Mana"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Hit					);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Hit Chance"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Damage				);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Damage"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.DirectDamage.Health	);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "DD. Health"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.DirectDamage.Shield	);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "DD. Shield"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.DirectDamage.Mana	);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "DD. Mana"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Absorption			);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Absorption"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Speed.Attack		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Atk. Speed"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Speed.Movement		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Mov. Speed"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Speed.Reflexes		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-10.10s: %-30.30s"	, "Reflexes"	, nameAndLevelText.c_str());
	}
	else {
		nameAndLevelText = character.Name												;	klib::lineToScreen(offsetY		, offsetX, klib::LEFT, "-- %-21.21s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getProfessionName	(character.CurrentEquip.Profession	);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getWeaponName		(character.CurrentEquip.Weapon		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = klib::getArmorName		(character.CurrentEquip.Armor		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = klib::getAccessoryName	(character.CurrentEquip.Accessory	);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = klib::getVehicleName		(character.CurrentEquip.Vehicle		);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
		//nameAndLevelText = klib::getFacilityName	(character.CurrentEquip.Facility	);	klib::lineToScreen(offsetY++	, offsetX, klib::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
	}
}

#define MAX_AGENT_ROWS 2
#define MAX_AGENT_COLUMNS 3
#define MAX_AGENT_SLOTS (MAX_AGENT_ROWS*MAX_AGENT_COLUMNS)

void drawSquadSlots(SGame& instanceGame, const SGameState& returnValue)
{
	//game::clearASCIIBackBuffer(' ');
	SGlobalDisplay& display = instanceGame.GlobalDisplay;
	static const int32_t slotWidth	= display.Width / MAX_AGENT_COLUMNS;
	static const int32_t slotRowSpace	= 25;// display.Depth / (MAX_AGENT_ROWS);

	for(int32_t y=0, countY=MAX_AGENT_ROWS; y<countY; ++y)
		for(int32_t x=0, countX=MAX_AGENT_COLUMNS; x<countX; ++x) 
		{
			int32_t linearIndex = y*countX+x;
			if( instanceGame.PlayerSquads.Agents[linearIndex] != -1 )
				displayAgentSlot(TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, linearIndex+1, instanceGame.GameArmies.Player[instanceGame.PlayerSquads.Agents[linearIndex]], true);
			else											 
				displayEmptySlot(TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, linearIndex+1);
		}
}


SGameState drawSquadSetupMenu(SGame& instanceGame, const SGameState& returnValue)
{

	drawSquadSlots(instanceGame, returnValue);

	klib::SMenuItem<int32_t> menuItems[] = 
	{	{0, "%s"}
	,	{1, "%s"}
	,	{2, "%s"}
	,	{3, "%s"}
	,	{4, "%s"}
	,	{5, "%s"}
	};

	for(uint32_t i=0, count=(uint32_t)size(menuItems); i<count; i++) 
	{
		char buffer[128];
		if(instanceGame.PlayerSquads.Agents[i] != -1) {
			sprintf_s(buffer, menuItems[i].Text.c_str(), instanceGame.GameArmies.Player[instanceGame.PlayerSquads.Agents[i]].Name.c_str());
			menuItems[i].Text = buffer;
		}
		else
			menuItems[i].Text = "Empty slot";
	}

	int32_t result = drawMenu(instanceGame.UserMessage = "Squad Setup", menuItems, instanceGame.FrameInput, 6);
	if(6 == result)
		return {GAME_STATE_WELCOME_COMMANDER};

	if( result < 0 || result > 5 )
		return {GAME_STATE_MENU_SQUAD_SETUP};

	if( instanceGame.PlayerSquads.Agents[result] != -1 && 0 == instanceGame.FrameInput.Keys[VK_LSHIFT] )
		return {GAME_STATE_MENU_EQUIPMENT};

	return {GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_CHARACTER};
}