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


SGameState drawBuyMenu(SGame& instanceGame, const SGameState& returnState)
{
#define MAX_BUY_ITEMS 8
#define SHOP_EXIT_VALUE ((MAX_BUY_ITEMS)*2)

	static klib::SMenuItem<int32_t> menuItems[MAX_BUY_ITEMS+1] = {};

	menuItems[MAX_BUY_ITEMS+1].ReturnValue = MAX_BUY_ITEMS+1;


	int32_t selectedChoice=0;
	//selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, (size_t)researchableCount, "Item a la carte", menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");
	if(selectedChoice == MAX_BUY_ITEMS)
		return {GAME_STATE_WELCOME_COMMANDER};

	return returnState;
}

SGameState drawBuy(SGame& instanceGame, const SGameState& returnState)
{
	std::string textToPrint = "Research center.";

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	memcpy(&instanceGame.PostEffectDisplay.Screen.Cells[instanceGame.PostEffectDisplay.Depth/2][instanceGame.PostEffectDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
	if ( !bDonePrinting ) 
		return returnState;

	drawBubblesBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);
	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		static const SMenu<SGameState, size(optionsResearch)> menuResearch(optionsResearch, {GAME_STATE_WELCOME_COMMANDER}, "Research Center", 26);
		return drawMenu(instanceGame.GlobalDisplay.Screen,	menuResearch, instanceGame.FrameInput, instanceGame.State);
	}
	else 
		return drawBuyMenu(instanceGame, returnState);

	return returnState;
};


