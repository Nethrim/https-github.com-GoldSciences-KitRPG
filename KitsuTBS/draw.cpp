#include "draw.h"

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
	switch(instanceGame.CurrentState.Substate)
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


	return instanceGame.CurrentState;
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
void drawDisplay(STacticalDisplay<_Width, _Depth>& source, uint32_t offset)	
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

void drawState( SGame& instanceGame )
{
	if(instanceGame.CurrentState.State == GAME_STATE_MENU_MAIN)
		drawIntro(instanceGame);
	
	if(instanceGame.CurrentState.State == GAME_STATE_CREDITS)
	{
		drawSnowBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);
		drawDisplay(instanceGame.GlobalDisplay, 0);
		drawCredits(instanceGame.FrameTimer.LastTimeSeconds);
	}
	else if(instanceGame.CurrentState.State == GAME_STATE_MENU_RESEARCH)
	{
		drawBubblesBackground(instanceGame.TacticalDisplay, instanceGame.FrameTimer.LastTimeSeconds/2);
		drawDisplay(instanceGame.TacticalDisplay, 5);
	}
	else
		drawDisplay(instanceGame.TacticalDisplay, 5);
}

