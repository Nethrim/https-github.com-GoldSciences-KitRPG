#include "Game.h"

#include "Item.h"
#include "Enemy.h"
#include "Profession.h"
#include "Armor.h"
#include "Weapon.h"

#include "Menu.h"

void tavern(CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const SMenuItem<int> tavernOptions[] =
	{ { 1, "Rest"						}
	, { 2, "Look for a mercenary job"	}
	, { 3, "Go for a drink"				}
	, { 4, "Show inventory"				}
	, { 5, "Display score"				}
	, { 6, "Exit game"					}
	};

	while (true)  // If the last action didn't go well we cancel the loop and exit the game.
	{
		int tavernChoice = displayMenu("You wonder about what to do next..", tavernOptions);

			 if( 1 == tavernChoice )	{	rest(adventurer);			}	// Rest and ask again for the action.
		else if( 2 == tavernChoice )	{	mercenaryJob(adventurer);	}	// Go for a mercenary job and ask again for action once it's done
		else if( 3 == tavernChoice )	{	bar(adventurer);			}	// Go to the shop and ask again for action once it's done.
		else if( 4 == tavernChoice )	{	showInventory(adventurer);	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 5 == tavernChoice )	{	displayScore(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 6 == tavernChoice )	{	break;						}	// Exit the main loop, which effectively closes the game.
	}
}

void mercenaryJob(CCharacter& adventurer)
{
	static const size_t enemyCount = size(enemyDefinitions);
	SMenuItem<uint32_t> jobOptions[enemyCount];
	for(uint32_t i=0, count = enemyCount-1; i<count; ++i)
	{
		jobOptions[i].ReturnValue	= i+1;
		jobOptions[i].Text			= "Level " + std::to_string(i+1);
	}
	jobOptions[enemyCount-1].ReturnValue	= enemyCount;
	jobOptions[enemyCount-1].Text			= "Back to tavern";

	const uint32_t enemyType = displayMenu("You decide to enroll for a mercenary job", jobOptions);

	// Set bCombat to true and the enemy type for executing the combat logic.
	bool bCombat = false;

	if(enemyCount == enemyType)	// This option cancels the loop which causes to exit to the tavern.
		printf("Welcome back, %s.\n", adventurer.Name.c_str());
	else {
		printf("You challenge a %s.\n", enemyDefinitions[enemyType].Name.c_str()); 
		combat(adventurer, enemyType);
	}
}

static inline constexpr int32_t getFinalItemCount() {
	return (int32_t)((size(itemDefinitions)-1)*size(itemModifiers));
}

template<size_t _Size>
static int initializeItemMenu(SMenuItem<SItem>(&menuItems)[_Size])
{
	char itemOption[128] = {};
	static const size_t descriptionCount = size(itemDefinitions);
	static const size_t modifierCount = size(itemModifiers);
	for(uint32_t indexItem=0, itemCount = descriptionCount-1; indexItem<itemCount; ++indexItem)
	{
		const int32_t indexDefinition = (int32_t)indexItem+1;
		for(int32_t grade=0; grade < modifierCount; ++grade)
		{
			const int32_t finalMenuItemIndex = indexItem*modifierCount+grade;
			menuItems[finalMenuItemIndex].ReturnValue	= { indexDefinition, grade };
			std::string itemName = getItemName( menuItems[finalMenuItemIndex].ReturnValue );
			sprintf_s(itemOption, "- $%.2u Coins - %s", (uint32_t)getItemPrice(menuItems[finalMenuItemIndex].ReturnValue), itemName.c_str());
			menuItems[finalMenuItemIndex].Text			= itemOption;
		}
	}
	menuItems[getFinalItemCount()].ReturnValue	= { getFinalItemCount(), 0 };
	menuItems[getFinalItemCount()].Text			= "Leave the bar";
	return 0;
};

void bar(CCharacter& adventurer)
{
	printf("\nDo you want to buy some drinks?\n\n");

	static const size_t menuItemCount = getFinalItemCount()+1;
	static SMenuItem<SItem> itemOptions[menuItemCount];
	static const int initialized = initializeItemMenu(itemOptions);

	char menuTitle[128] = {};
	while (true)	// break the loop to leave the shop
	{
		sprintf_s(menuTitle, "You have %u coins", adventurer.Points.Coins);
		const SItem selectedItem = displayMenu(menuTitle, itemOptions);
		if( selectedItem.Index == getFinalItemCount() ) {
			printf("You leave the bar.\n");
			break;
		}
		else 
		{
			int itemPrice	= getItemPrice(selectedItem);	// Get a copy of this value because we use it very often.
			const std::string itemName = getItemName(selectedItem);

			// Check first for conditions that prevent from acquiring the item
			if(adventurer.Points.Coins < itemPrice)
				printf("You can't afford to buy %s! Choose something else...\n", itemName.c_str());
			else if(addItem(adventurer.Inventory, selectedItem))	// addItem() returns false if the inventory is full.
			{
				printf("You spend %u coins buying %s.\n", itemPrice, itemName.c_str());
				adventurer.Points.Coins		-= itemPrice;
				adventurer.Score.MoneySpent	+= itemPrice;
			}
			else
				printf("Not enough space in inventory!\n");
		}
	}
	showInventory(adventurer);
}

void displayScore(const CCharacter& adventurer) 
{
	const SCharacterPoints& points = adventurer.Points;
	const SCharacterScore& gameCounters		= adventurer.Score;
	const SCharacterPoints finalPoints		= calculateFinalPoints(adventurer);
	const SCharacterPoints weaponPoints		= getWeaponPoints(adventurer.Weapon);
	const SCharacterPoints armorPoints		= getArmorPoints(adventurer.Armor);
	const SCharacterPoints professionPoints	= getProfessionPoints(adventurer.Profession);

	printf("\n-- %s final points:\n", adventurer.Name.c_str());
	printf("- Max Life:\n");
	finalPoints.MaxLife.Print();
	printf("- Current Life:\n");
	points.CurrentLife.Print();
	printf("- Attack:\n");
	finalPoints.Attack.Print();
	printf("- Coins: %u.\n", points.Coins);
	printf("- Bonus Coins per turn: %u.\n", finalPoints.Coins);

	printf("\n-- %s base character points:\n", adventurer.Name.c_str());
	printf("- Max Life:\n");
	points.MaxLife.Print();
	printf("- Attack:\n");
	points.Attack.Print();

	printf("\n-- %s is a %s level %u:\n", adventurer.Name.c_str(), getProfessionName(adventurer.Profession).c_str(), adventurer.Profession.Level);
	professionPoints.Print();

	printf("\n-- %s is wearing %s level %u:\n", adventurer.Name.c_str(), getArmorName(adventurer.Armor).c_str(), adventurer.Armor.Level);
	armorPoints.Print();

	printf("\n-- %s is carrying %s level %u:\n", adventurer.Name.c_str(), getWeaponName(adventurer.Weapon).c_str(), adventurer.Weapon.Level);
	weaponPoints.Print();

	printf("\n-- Player statistics:\n\n"
		"Battles Won         : %llu\n"
		"Battles Lost        : %llu\n"
		"Turns Played        : %llu\n"
		"--------------------\n"
		"Enemies Killed      : %llu\n"
		"Damage Dealt        : %llu\n"
		"Damage Taken        : %llu\n"
		"--------------------\n"
		"Escapes Succeeded   : %llu\n"
		"Escapes Failed      : %llu\n"
		"--------------------\n"
		"Money Earned        : %llu\n"
		"Money Spent         : %llu\n"
		"--------------------\n"
		"Attacks Hit         : %llu\n"
		"Attacks Missed      : %llu\n"
		"Attacks Received    : %llu\n"
		"Attacks Avoided     : %llu\n"
		"--------------------\n"
		"Potions Used        : %llu\n"
		"Grenades Used       : %llu\n\n"
		, gameCounters.BattlesWon			
		, gameCounters.BattlesLost
		, gameCounters.TurnsPlayed			
	
		, gameCounters.EnemiesKilled		
		, gameCounters.DamageDealt			
		, gameCounters.DamageTaken			
	
		, gameCounters.EscapesSucceeded	
		, gameCounters.EscapesFailed		
	
		, gameCounters.MoneyEarned			
		, gameCounters.MoneySpent			

		, gameCounters.AttacksHit			
		, gameCounters.AttacksMissed		
		, gameCounters.AttacksReceived		
		, gameCounters.AttacksAvoided		

		, gameCounters.PotionsUsed			
		, gameCounters.GrenadesUsed		
	);
};
