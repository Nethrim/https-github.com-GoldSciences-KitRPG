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
#define MAX_BUY_ITEMS 9
#define SHOP_EXIT_VALUE ((MAX_BUY_ITEMS)*2)

	static klib::SMenuItem<int32_t> menuItems[MAX_BUY_ITEMS+1] = {};

	menuItems[MAX_BUY_ITEMS+1].ReturnValue = MAX_BUY_ITEMS+1;

	int32_t itemCount=0;

	int32_t selectedChoice=0;
	std::string menuTitle;
	switch(instanceGame.State.Substate)
	{
	case GAME_SUBSTATE_ACCESSORY	:	menuTitle = "Accessory"		; break;
	case GAME_SUBSTATE_STAGEPROP	:	menuTitle = "Stage Prop"	; break;
	case GAME_SUBSTATE_FACILITY		:	menuTitle = "Facility"		; break;
	case GAME_SUBSTATE_VEHICLE		:	menuTitle = "Vehicle"		; break;
	case GAME_SUBSTATE_PROFESSION	:	menuTitle = "Profession"	; break;
	case GAME_SUBSTATE_WEAPON		:	menuTitle = "Weapon"		; break;
	case GAME_SUBSTATE_ARMOR		:	menuTitle = "Armor"			; break;
	default:
		break;
	}
	menuTitle += " a la carte";

	selectedChoice = drawMenu(instanceGame.GlobalDisplay.Screen, (size_t)itemCount, menuTitle, menuItems, instanceGame.FrameInput, SHOP_EXIT_VALUE, -1, 40U, false, "Exit this menu");
	if(selectedChoice == SHOP_EXIT_VALUE)
		return {GAME_STATE_MENU_BUY};

	return returnState;
}

SGameState drawBuy(SGame& instanceGame, const SGameState& returnState)
{
	std::string textToPrint = "Tell me how much money you have and I will tell you what you'll become.";

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds*3);
	memcpy(&instanceGame.PostEffectDisplay.Screen.Cells[instanceGame.PostEffectDisplay.Depth/2][instanceGame.PostEffectDisplay.Width/2-(strlen(instanceGame.SlowMessage)+1)/2], instanceGame.SlowMessage, strlen(instanceGame.SlowMessage));
	if ( !bDonePrinting ) 
		return returnState;

	drawBubblesBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);
	if(GAME_SUBSTATE_MAIN == instanceGame.State.Substate) 
	{
		static const SMenu<SGameState, size(optionsBuy)> menuBuy(optionsBuy, {GAME_STATE_WELCOME_COMMANDER}, "Order Menu", 26);
		return drawMenu(instanceGame.GlobalDisplay.Screen,	menuBuy, instanceGame.FrameInput, instanceGame.State);
	}
	else 
		return drawBuyMenu(instanceGame, returnState);

	return returnState;
};


