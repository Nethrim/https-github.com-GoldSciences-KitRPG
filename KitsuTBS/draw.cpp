#include "draw.h"

#include "Profession.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "Facility.h"
#include "Vehicle.h"

void drawIntro(SGame& instanceGame);
static const std::string namesProgramming[] =
{	"Kitsu"											
};

static const std::string namesEverythingElse[] =
{	"Kitsu"											
};

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
	int32_t armySize = instanceGame.PlayerArmy.size();
	int32_t squadSize = instanceGame.PlayerSquad.size();

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

	int32_t selectedChoice;
	int32_t iArmySoldier, iSquadSoldier;
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY:
		for(iArmySoldier=0; iArmySoldier<armySize; ++iArmySoldier)
			generateResearchableList(researchableItems.Accessory, instanceGame.PlayerArmy[iArmySoldier].Inventory.Accessories, researchCompleted.Accessory);
		for(iSquadSoldier=0; iSquadSoldier<squadSize; ++iSquadSoldier)
			generateResearchableList(researchableItems.Accessory, instanceGame.PlayerSquad[iSquadSoldier].Inventory.Accessories, researchCompleted.Accessory);
		break;
	case GAME_SUBSTATE_STAGEPROP:
		for(iArmySoldier=0; iArmySoldier<armySize; ++iArmySoldier)
			generateResearchableList(researchableItems.StageProp, instanceGame.PlayerArmy[iArmySoldier].Inventory.Accessories, researchCompleted.StageProp);
		for(iSquadSoldier=0; iSquadSoldier<squadSize; ++iSquadSoldier)
			generateResearchableList(researchableItems.StageProp, instanceGame.PlayerSquad[iSquadSoldier].Inventory.Accessories, researchCompleted.StageProp);
		break;
	case GAME_SUBSTATE_FACILITY:
		for(iArmySoldier=0; iArmySoldier<armySize; ++iArmySoldier)
			generateResearchableList(researchableItems.Facility, instanceGame.PlayerArmy[iArmySoldier].Inventory.Accessories, researchCompleted.Facility);
		for(iSquadSoldier=0; iSquadSoldier<squadSize; ++iSquadSoldier)
			generateResearchableList(researchableItems.Facility, instanceGame.PlayerSquad[iSquadSoldier].Inventory.Accessories, researchCompleted.Facility);
		break;
	case GAME_SUBSTATE_VEHICLE:
		for(iArmySoldier=0; iArmySoldier<armySize; ++iArmySoldier)
			generateResearchableList(researchableItems.Vehicle, instanceGame.PlayerArmy[iArmySoldier].Inventory.Accessories, researchCompleted.Vehicle);
		for(iSquadSoldier=0; iSquadSoldier<squadSize; ++iSquadSoldier)
			generateResearchableList(researchableItems.Vehicle, instanceGame.PlayerSquad[iSquadSoldier].Inventory.Accessories, researchCompleted.Vehicle);
		break;
	case GAME_SUBSTATE_PROFESSION:
		for(iArmySoldier=0; iArmySoldier<armySize; ++iArmySoldier)
			generateResearchableList(researchableItems.Profession, instanceGame.PlayerArmy[iArmySoldier].Inventory.Accessories, researchCompleted.Profession);
		for(iSquadSoldier=0; iSquadSoldier<squadSize; ++iSquadSoldier)
			generateResearchableList(researchableItems.Profession, instanceGame.PlayerSquad[iSquadSoldier].Inventory.Accessories, researchCompleted.Profession);
		break;
	case GAME_SUBSTATE_WEAPON:
		for(iArmySoldier=0; iArmySoldier<armySize; ++iArmySoldier)
			generateResearchableList(researchableItems.Weapon, instanceGame.PlayerArmy[iArmySoldier].Inventory.Accessories, researchCompleted.Weapon);
		for(iSquadSoldier=0; iSquadSoldier<squadSize; ++iSquadSoldier)
			generateResearchableList(researchableItems.Weapon, instanceGame.PlayerSquad[iSquadSoldier].Inventory.Accessories, researchCompleted.Weapon);
		break;
	case GAME_SUBSTATE_ARMOR:
		for(iArmySoldier=0; iArmySoldier<armySize; ++iArmySoldier)
			generateResearchableList(researchableItems.Armor, instanceGame.PlayerArmy[iArmySoldier].Inventory.Accessories, researchCompleted.Armor);
		for(iSquadSoldier=0; iSquadSoldier<squadSize; ++iSquadSoldier)
			generateResearchableList(researchableItems.Armor, instanceGame.PlayerSquad[iSquadSoldier].Inventory.Accessories, researchCompleted.Armor);
		break;
	default:
		break;
	}

	selectedChoice = drawMenu("", menuItems, instanceGame.FrameInput, 8);
	if(selectedChoice == 8)
		return {GAME_STATE_MENU_CONTROL_CENTER};


	return instanceGame.State;
}


SGameState drawCredits(double lastFrameTime)
{
	static double offset = (double)game::getASCIIBackBufferHeight();// (game::getASCIIBackBufferHeight()>>1)-4;
	int32_t curLine = (int32_t)offset;
	static int32_t maxDifference = curLine;
	int32_t curDifference = curLine;
	double bbHeight = (double)game::getASCIIBackBufferHeight();
#define IN_RANGE(currentline) (currentline >= 0 && currentline < bbHeight)
	curLine = (int32_t)(curLine += 1);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "-- Loosely based on homework code and idea by --"	);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "Nethrim"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "-- Programming --"								);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "Kitsu"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "-- Beta Testing --"								);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "Gorby"											);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "Nethrim"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "-- Everything else --"							);
	curLine = (int32_t)(curLine += 2);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "Kitsu"											);
	curLine = (int32_t)(curLine += 4);	if(IN_RANGE(curLine))	game::lineToScreen(curLine, 0, game::CENTER, "-- Special Thanks to --"							);
	for(uint32_t i=0; i<klib::size(namesSpecialThanks); ++i) {
		curLine = (int32_t)(curLine += 2);	
		if(curLine >= 0 && curLine < bbHeight) 
			game::lineToScreen(curLine, 0, game::CENTER, "%s", namesSpecialThanks[i].c_str());
	}

	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < bbHeight)	game::lineToScreen(curLine, 0, game::CENTER, "Have a nice weekend!"					); 
	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < bbHeight)	game::lineToScreen(curLine, 0, game::CENTER, "Press ESC to return to main menu."	); 
	
	maxDifference = std::max(curLine - curDifference, maxDifference);

	offset -= lastFrameTime*6.0;

	if( offset <= -maxDifference )
		offset += game::getASCIIBackBufferHeight()+maxDifference;

	return {GAME_STATE_CREDITS,};
}

template<size_t _Width, size_t _Depth>
void drawDisplay(SWeightedDisplay<_Width, _Depth>& source, uint32_t offset)	
{
	for(uint32_t z=0, maxZ=std::min(source.Depth, game::getASCIIBackBufferHeight()-1-offset); z<maxZ; ++z) {
		char row[_Width+1] = {};
		memcpy(row, source.Screen[z], std::min(source.Width, game::getASCIIBackBufferWidth()-1));
		game::lineToScreen(offset, 0, game::CENTER, row);
		++offset;
	}
};

void drawIntro( SGame& instanceGame )
{
	drawFireBackground(instanceGame.TacticalDisplay, instanceGame.FrameTimer.LastTimeSeconds);

	int32_t displayWidth	= (int32_t)instanceGame.TacticalDisplay.Width;
	int32_t displayDepth	= (int32_t)instanceGame.TacticalDisplay.Depth;

	memcpy(&instanceGame.TacticalDisplay.Screen[displayDepth	/2-3]	[displayWidth/2-strlen("A "			)/2-1], "A "		, strlen("A "		));
	memcpy(&instanceGame.TacticalDisplay.Screen[displayDepth	/2-1]	[displayWidth/2-strlen("Tactical"	)/2-1], "Tactical"	, strlen("Tactical"	));
	memcpy(&instanceGame.TacticalDisplay.Screen[displayDepth	/2+1]	[displayWidth/2-strlen("Game"		)/2-1], "Game" 		, strlen("Game"		));
};



SGameState drawResearch(SGame& instanceGame, SGameState returnValue)
{
	static const std::string textToPrint = "Welcome back commander " + instanceGame.PlayerName + ".";

	bool bDonePrinting = drawMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.NextTick, instanceGame.TickCount, instanceGame.FrameTimer.LastTimeSeconds);
	if ( bDonePrinting ) {
		return drawMenu(instanceGame.UserMessage = "Control Center", optionsControlCenter, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN,}, {GAME_STATE_WELCOME_COMMANDER,}, false, "Exit this menu");
	}

	return returnValue;
};

void drawState( SGame& instanceGame )
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

		bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.NextTick, instanceGame.TickCount, instanceGame.FrameTimer.LastTimeSeconds);
		memcpy(&instanceGame.TacticalDisplay.Screen[instanceGame.TacticalDisplay.Depth/2][instanceGame.TacticalDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
		if ( bDonePrinting )
			drawBubblesBackground(instanceGame.TacticalDisplay, instanceGame.FrameTimer.LastTimeSeconds/2);

		drawDisplay(instanceGame.TacticalDisplay, 5);
	}
	else if(instanceGame.State.State == GAME_STATE_MENU_SQUAD_SETUP)
	{
		static const std::string textToPrint = "  Squad setup.";

		bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.NextTick, instanceGame.TickCount, instanceGame.FrameTimer.LastTimeSeconds);
		memcpy(&instanceGame.GlobalDisplay.Screen[instanceGame.GlobalDisplay.Depth/2][instanceGame.GlobalDisplay.Width/2-(strlen(instanceGame.SlowMessage)+4)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
		if ( bDonePrinting )
			drawSpaceBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds/2);

		drawDisplay(instanceGame.GlobalDisplay, 0);
	}
	else
		drawDisplay(instanceGame.TacticalDisplay, 5);
}


#define MAX_AGENT_ROWS 2
#define MAX_AGENT_COLUMNS 3
#define MAX_AGENT_SLOTS (MAX_AGENT_ROWS*MAX_AGENT_COLUMNS)

void displayEmptySlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex)
{
	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- Agent #%u: %-36.36s", agentIndex, "Open position");
}

void displayAgentSlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex, klib::CCharacter& character, bool bAddFieldNames=false)
{
	std::string nameAndLevelText;
	klib::SEntityPoints agentPoints = klib::calculateFinalPoints( character );
	if( bAddFieldNames ) {
		nameAndLevelText = character.Name													;	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- Agent #%u: %-25.25s (Lv. %i)", agentIndex		, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getProfessionName	(character.CurrentEquip.Profession		);	game::lineToScreen(offsetY+2	, offsetX, game::LEFT, "%-10.10s: %-26.26s (Lv. %i)"	, "Class"				, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getWeaponName		(character.CurrentEquip.Weapon			);	game::lineToScreen(offsetY+3	, offsetX, game::LEFT, "%-10.10s: %-26.26s (Lv. %i)"	, "Weapon"				, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = klib::getArmorName		(character.CurrentEquip.Armor			);	game::lineToScreen(offsetY+4	, offsetX, game::LEFT, "%-10.10s: %-26.26s (Lv. %i)"	, "Armor"				, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = klib::getAccessoryName	(character.CurrentEquip.Accessory		);	game::lineToScreen(offsetY+5	, offsetX, game::LEFT, "%-10.10s: %-26.26s (Lv. %i)"	, "Accessory"			, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = klib::getVehicleName		(character.CurrentEquip.Vehicle			);	game::lineToScreen(offsetY+6	, offsetX, game::LEFT, "%-10.10s: %-26.26s (Lv. %i)"	, "Vehicle"				, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
		//nameAndLevelText = klib::getFacilityName	(character.CurrentEquip.Facility		);	game::lineToScreen(offsetY+7	, offsetX, game::LEFT, "%-10.10s: %-26.26s (Lv. %i)"	, "Building assigned"	, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
		nameAndLevelText = std::to_string			(agentPoints.LifeMax.Health				);	game::lineToScreen(offsetY+8	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Health"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.LifeMax.Shield				);	game::lineToScreen(offsetY+9	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Shield"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Hit					);	game::lineToScreen(offsetY+10	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Hit Chance"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Damage				);	game::lineToScreen(offsetY+11	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Damage"		, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Absorption			);	game::lineToScreen(offsetY+12	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Absorption"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.DirectDamage.Health	);	game::lineToScreen(offsetY+13	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "DD. Health"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.DirectDamage.Shield	);	game::lineToScreen(offsetY+14	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "DD. Shield"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Speed.Attack		);	game::lineToScreen(offsetY+15	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Atk. Speed"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Speed.Movement		);	game::lineToScreen(offsetY+16	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Mov. Speed"	, nameAndLevelText.c_str());
		nameAndLevelText = std::to_string			(agentPoints.Attack.Speed.Reflexes		);	game::lineToScreen(offsetY+17	, offsetX, game::LEFT, "%-10.10s: %-26.26s"	, "Reflexes"	, nameAndLevelText.c_str());
	}
	else {
		nameAndLevelText = character.Name												;	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- %-21.21s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getProfessionName	(character.CurrentEquip.Profession	);	game::lineToScreen(offsetY+2	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getWeaponName		(character.CurrentEquip.Weapon		);	game::lineToScreen(offsetY+3	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = klib::getArmorName		(character.CurrentEquip.Armor		);	game::lineToScreen(offsetY+4	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = klib::getAccessoryName	(character.CurrentEquip.Accessory	);	game::lineToScreen(offsetY+5	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = klib::getVehicleName		(character.CurrentEquip.Vehicle		);	game::lineToScreen(offsetY+6	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
		//nameAndLevelText = klib::getFacilityName	(character.CurrentEquip.Facility	);	game::lineToScreen(offsetY+7	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
	}
}

void drawSquadSlots(SGame& instanceGame, SGameState returnValue)
{
	//game::clearASCIIBackBuffer(' ');
	SGlobalDisplay& display = instanceGame.GlobalDisplay;
	static const int32_t slotWidth	= display.Width / MAX_AGENT_COLUMNS;
	static const int32_t slotRowSpace	= 20;// display.Depth / (MAX_AGENT_ROWS);

	for(int32_t y=0, countY=MAX_AGENT_ROWS; y<countY; ++y)
		for(int32_t x=0, countX=MAX_AGENT_COLUMNS; x<countX; ++x) 
		{
			int32_t linearIndex = y*countX+x;
			if(linearIndex < (int32_t)instanceGame.PlayerSquad.size())
				displayAgentSlot(TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, linearIndex+1, instanceGame.PlayerSquad[linearIndex], true);
			else											 
				displayEmptySlot(TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, linearIndex+1);
		}
}


SGameState drawSquadSetupMenu(SGame& instanceGame, SGameState returnValue)
{
	int32_t optionCount = (int32_t)instanceGame.PlayerArmy.size();
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
		if(i < instanceGame.PlayerSquad.size()) {
			sprintf_s(buffer, menuItems[i].Text.c_str(), instanceGame.PlayerSquad[i].Name.c_str());
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

	if( result < ((int32_t)instanceGame.PlayerSquad.size()) && 0 == instanceGame.FrameInput.Keys[VK_LSHIFT] )
		return {GAME_STATE_MENU_EQUIPMENT};

	return {GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_CHARACTER};
}