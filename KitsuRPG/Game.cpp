#include "Game.h"

#include "Item.h"
#include "Enemy.h"
#include "Profession.h"
#include "Armor.h"
#include "Weapon.h"
#include "Inventory.h"

#include "Menu.h"

void displayInventory(const klib::SCharacterInventory& inventory)
{
	printf("\n-- Your inventory --\n");
	if(inventory.ItemCount) {
		printf("You look at the remaining supplies in your backpack...\n");
		for (unsigned int i = 0; i < inventory.ItemCount; i++)
			printf("%u: x%.2u %s.\n", i + 1, inventory.Slots[i].ItemCount, klib::getItemName(inventory.Slots[i].Item).c_str());
	}
	printf("\n");
}

void researchWeaponDefinition		(klib::CCharacter& adventurer);
void researchWeaponModifier			(klib::CCharacter& adventurer);
void researchWeaponLevel			(klib::CCharacter& adventurer);
void researchArmorDefinition		(klib::CCharacter& adventurer);
void researchArmorModifier			(klib::CCharacter& adventurer);
void researchArmorLevel				(klib::CCharacter& adventurer);
void researchProfessionDefinition	(klib::CCharacter& adventurer);
void researchProfessionModifier		(klib::CCharacter& adventurer);
void researchProfessionLevel		(klib::CCharacter& adventurer);

void equipWeapon	(klib::CCharacter& adventurer) 
{ 
	if( 0 == adventurer.Weapons.Count )	{ 
		printf("You don't have any other weapons yet!\n"); 
		return; 
	}
	
	static char						menuItemText[256] = {};	
	static klib::SMenuItem<int16_t>	menuItems	[256] = {}; 
	int32_t							menuItemCount=0; 

	for(int32_t i=0, count = adventurer.Weapons.Count; i<count; ++i) { 
		sprintf_s(menuItemText, "%s level %u", klib::getWeaponName(adventurer.Weapons.Slots[i]).c_str(), adventurer.Weapons.Slots[i].Level);
		menuItems[menuItemCount++] = {(int16_t)i, menuItemText}; 
	}	

	printf("You're currently carrying a %s level %u.\n", klib::getWeaponName(adventurer.CurrentWeapon).c_str(), adventurer.CurrentWeapon.Level);
	menuItems[menuItemCount++] = {256, "Back to tavern"}; 
	int32_t selectedValue = klib::displayMenu("Select your weapon of choice", menuItems, menuItemCount);	
	if(selectedValue == 256) 
		return; 
	adventurer.EquipWeapon(selectedValue); 
};

void equipArmor		(klib::CCharacter& adventurer) 
{ 
	if( 0 == adventurer.Armors.Count )	{ 
		printf("You don't have any other armors yet!\n"); 
		return; 
	}

	static char						menuItemText[256] = {};	
	static klib::SMenuItem<int16_t>	menuItems	[256] = {}; 
	int32_t							menuItemCount=0; 

	for(int32_t i=0, count = adventurer.Armors.Count; i<count; ++i)	{ 
		sprintf_s(menuItemText, "%s level %u", klib::getArmorName(adventurer.Armors.Slots[i]).c_str(), adventurer.Armors.Slots[i].Level);
		menuItems[menuItemCount++] = {(int16_t)i, menuItemText}; 
	}	

	printf("You're currently wearing a %s level %u.\n", klib::getArmorName(adventurer.CurrentArmor).c_str(), adventurer.CurrentArmor.Level);
	menuItems[menuItemCount++] = {256, "Back to tavern"};
	int32_t selectedValue = klib::displayMenu("Select your armor of choice", menuItems, menuItemCount);	
	if(selectedValue == 256) 
		return; 

	adventurer.EquipArmor(selectedValue);
};


void equipProfession	(klib::CCharacter& adventurer) 
{ 
	if( 0 == adventurer.Professions.Count )	{ 
		printf("You don't have any other masteries yet!\n"); 
		return; 
	}

	static char						menuItemText[256] = {};	
	static klib::SMenuItem<int16_t>	menuItems	[256] = {}; 
	int32_t							menuItemCount=0; 

	for(int32_t i=0, count = adventurer.Professions.Count; i<count; ++i)	{ 
		sprintf_s(menuItemText, "%s level %u", klib::getProfessionName(adventurer.Professions.Slots[i]).c_str(), adventurer.Professions.Slots[i].Level);
		menuItems[menuItemCount++] = {(int16_t)i, menuItemText}; 
	}	

	printf("You're currently assigned as a %s level %u.\n", klib::getProfessionName(adventurer.CurrentProfession).c_str(), adventurer.CurrentProfession.Level);
	menuItems[menuItemCount++] = {256, "Back to tavern"};
	int32_t selectedValue = klib::displayMenu("Select your mastery of choice", menuItems, menuItemCount);	
	if(selectedValue == 256) 
		return; 

	adventurer.EquipProfession(selectedValue);
};


void tavern(klib::CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const klib::SMenuItem<int> tavernOptions[] =
	{ {  1, "Rest"						}
	, {  2, "Look for a mercenary job"	}
	, {  3, "Go for a drink"			}
	, {  4, "Show inventory"			}
	, {  5, "Show equipment"			}
	, {  6, "Equip weapon"				}
	, {  7, "Equip armor"				}
	, {  8, "Study for next task"		}
	, {  9, "Research new weapons"		}
	, { 10, "Research new sciences"		}
	, { 11, "Research new armors"		}
	, { 12, "Research new technologies"	}
	, { 13, "Learn new professions"		}
	, { 14, "Train for new ranks"		}
	, { 15, "Display score"				}
	, { 16, "Exit game"					}
	};

	while (true)  // If the last action didn't go well we cancel the loop and exit the game.
	{
		int tavernChoice = klib::displayMenu("You wonder about what to do next..", tavernOptions);

			 if(  1	== tavernChoice )	{	rest							(adventurer);	}	// Rest and ask again for the action.
		else if(  2	== tavernChoice )	{	mercenaryJob					(adventurer);	}	// Go for a mercenary job and ask again for action once it's done
		else if(  3	== tavernChoice )	{	bar								(adventurer);	}	// Display the inventory and coins and ask again for action once it's done.
		else if(  4	== tavernChoice )	{	displayInventory				(adventurer.Inventory);	}	// Display the inventory and coins and ask again for action once it's done.
		else if(  5	== tavernChoice )	{	displayEquip					(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if(  6	== tavernChoice )	{	equipWeapon						(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if(  7	== tavernChoice )	{	equipArmor						(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if(  8	== tavernChoice )	{	equipProfession					(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if(  9	== tavernChoice )	{	researchWeaponDefinition		(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 10	== tavernChoice )	{	researchWeaponModifier			(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 11	== tavernChoice )	{	researchArmorDefinition			(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 12	== tavernChoice )	{	researchArmorModifier			(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 13	== tavernChoice )	{	researchProfessionDefinition	(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 14	== tavernChoice )	{	researchProfessionModifier		(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 15 == tavernChoice )	{	displayScore					(adventurer.Score);	}	// Display score and player points and ask again for action once it's done.
		else if( 16 == tavernChoice )	{	break;						}	// Exit the main loop, which effectively closes the game.
		else {	
			printf("Option not supported yet.\n");
		}	// Exit the main loop, which effectively closes the game.
	}
}

void mercenaryJob(klib::CCharacter& adventurer)
{
	static const size_t enemyCount = size(klib::enemyDefinitions);
	klib::SMenuItem<int32_t> jobOptions[enemyCount];
	for(int32_t i=0, count = enemyCount-1; i<count; ++i)
	{
		jobOptions[i].ReturnValue	= i+1;
		jobOptions[i].Text			= "Level " + std::to_string(i+1);
	}
	jobOptions[enemyCount-1].ReturnValue	= enemyCount;
	jobOptions[enemyCount-1].Text			= "Back to tavern";

	const int32_t enemyType = displayMenu("You decide to enroll for a mercenary job", jobOptions);

	// Set bCombat to true and the enemy type for executing the combat logic.
	bool bCombat = false;

	if(enemyCount == enemyType)	// This option cancels the loop which causes to exit to the tavern.
		printf("Welcome back, %s.\n", adventurer.Name.c_str());
	else {
		printf("You challenge a %s.\n", klib::enemyDefinitions[enemyType].Name.c_str()); 
		combat(adventurer, enemyType);
	}
}


template<size_t _Size>
static int initializeItemMenu(klib::SMenuItem<klib::SItem>(&menuItems)[_Size])
{
	char itemOption[128] = {};
	static const size_t descriptionCount = size(klib::itemDescriptions);
	static const size_t gradeCount = size(klib::itemGrades);
	for(int32_t indexItem=0, itemCount = descriptionCount-1; indexItem<itemCount; ++indexItem)
	{
		const int32_t indexDefinition = (int32_t)indexItem+1;
		for(int32_t grade=0; grade < gradeCount; ++grade)
		{
			const int32_t finalMenuItemIndex = indexItem*gradeCount+grade;
			menuItems[finalMenuItemIndex].ReturnValue	= { (int16_t)indexDefinition, 0, (int16_t)grade };
			std::string itemName = klib::getItemName( menuItems[finalMenuItemIndex].ReturnValue );
			sprintf_s(itemOption, "- $%.2u Coins - %s", (int32_t)klib::getItemPrice(menuItems[finalMenuItemIndex].ReturnValue), itemName.c_str());
			menuItems[finalMenuItemIndex].Text			= itemOption;
		}
	}
	menuItems[klib::getFinalItemCount()].ReturnValue	= { (int16_t)klib::getFinalItemCount() };
	menuItems[klib::getFinalItemCount()].Text			= "Leave the bar";
	return 0;
};

void bar(klib::CCharacter& adventurer)
{
	printf("\nDo you want to buy some drinks?\n\n");

	static const size_t menuItemCount = klib::getFinalItemCount()+1;
	static klib::SMenuItem<klib::SItem> itemOptions[menuItemCount];
	static const int initialized = initializeItemMenu(itemOptions);

	char menuTitle[128] = {};
	while (true)	// break the loop to leave the shop
	{
		sprintf_s(menuTitle, "You have %u coins", adventurer.Points.Coins);
		const klib::SItem selectedItem = displayMenu(menuTitle, itemOptions);
		if( selectedItem.Index == klib::getFinalItemCount() ) {
			printf("You leave the bar.\n");
			break;
		}
		else 
		{
			int itemPrice	= klib::getItemPrice(selectedItem);	// Get a copy of this value because we use it very often.
			const std::string itemName = klib::getItemName(selectedItem);

			// Check first for conditions that prevent from acquiring the item
			if(adventurer.Points.Coins < itemPrice)
				printf("You can't afford to buy %s! Choose something else...\n", itemName.c_str());
			else if(klib::addItem(adventurer.Inventory, selectedItem))	// addItem() returns false if the inventory is full.
			{
				printf("You spend %u coins buying %s.\n", itemPrice, itemName.c_str());
				adventurer.Points.Coins		-= itemPrice;
				adventurer.Score.MoneySpent	+= itemPrice;
			}
			else
				printf("Not enough space in inventory!\n");
		}
	}
	displayInventory(adventurer.Inventory);
}

void displayEquip(const klib::CCharacter& adventurer) 
{
	const klib::SCharacterPoints& basePoints = adventurer.Points;
	const klib::SCharacterPoints finalPoints		= klib::calculateFinalPoints(adventurer);
	const klib::SCharacterPoints weaponPoints		= klib::getWeaponPoints(adventurer.CurrentWeapon);
	const klib::SCharacterPoints armorPoints		= klib::getArmorPoints(adventurer.CurrentArmor);
	const klib::SCharacterPoints professionPoints	= klib::getProfessionPoints(adventurer.CurrentProfession);

	printf("\n-- %s final points:\n", adventurer.Name.c_str());
	printf("- Max Life:\n");
	finalPoints.MaxLife.Print();
	printf("- Current Life:\n");
	basePoints.CurrentLife.Print();
	printf("- Attack:\n");
	finalPoints.Attack.Print();
	printf("- Coins: %i.\n", basePoints.Coins);
	printf("- Bonus Coins per turn: %i.\n", finalPoints.Coins);
	finalPoints.PrintStatusAndEffect();

	printf("\n-- %s base character points:\n", adventurer.Name.c_str());
	printf("- Max Life:\n");
	basePoints.MaxLife.Print();
	printf("- Attack:\n");
	basePoints.Attack.Print();
	basePoints.PrintStatusAndEffect();

	printf("\n-- %s is a %s level %u:\n", adventurer.Name.c_str(), klib::getProfessionName(adventurer.CurrentProfession).c_str(), adventurer.CurrentProfession.Level);
	professionPoints.Print();

	printf("\n-- %s is wearing %s level %u:\n", adventurer.Name.c_str(), klib::getArmorName(adventurer.CurrentArmor).c_str(), adventurer.CurrentArmor.Level);
	armorPoints.Print();

	printf("\n-- %s is carrying %s level %u:\n", adventurer.Name.c_str(), klib::getWeaponName(adventurer.CurrentWeapon).c_str(), adventurer.CurrentWeapon.Level);
	weaponPoints.Print();
}

void displayScore(const klib::SCharacterScore& score) 
{
	const klib::SCharacterScore& gameCounters		= score;
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
