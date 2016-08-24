#include "Game.h"

#include "Item.h"
#include "Enemy.h"
#include "Profession.h"
#include "Armor.h"
#include "Weapon.h"
#include "Vehicle.h"

#include "Menu.h"

void displayInventory(const klib::SInventoryItems& inventory)
{
	printf("\n-- Your inventory --\n");
	if(inventory.Count) {
		printf("You look at the remaining supplies in your backpack...\n");
		for (unsigned int i = 0; i < inventory.Count; i++)
			printf("%u: x%.2u %s.\n", i + 1, inventory.Slots[i].Count, klib::getItemName(inventory.Slots[i].Entity).c_str());
	}
	printf("\n");
}


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
		sprintf_s(menuItemText, "x%.02i: %s level %u", adventurer.Weapons.Slots[i].Count, klib::getWeaponName(adventurer.Weapons.Slots[i].Entity).c_str(), adventurer.Weapons.Slots[i].Entity.Level);
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
		sprintf_s(menuItemText, "x%.02i: %s level %u", adventurer.Weapons.Slots[i].Count, klib::getArmorName(adventurer.Armors.Slots[i].Entity).c_str(), adventurer.Armors.Slots[i].Entity.Level);
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
		sprintf_s(menuItemText, "x%.02i: %s level %u", adventurer.Weapons.Slots[i].Count, klib::getProfessionName(adventurer.Professions.Slots[i].Entity).c_str(), adventurer.Professions.Slots[i].Entity.Level);
		menuItems[menuItemCount++] = {(int16_t)i, menuItemText}; 
	}	

	printf("You're currently assigned as a %s level %u.\n", klib::getProfessionName(adventurer.CurrentProfession).c_str(), adventurer.CurrentProfession.Level);
	menuItems[menuItemCount++] = {256, "Back to tavern"};
	int32_t selectedValue = klib::displayMenu("Select your mastery of choice", menuItems, menuItemCount);	
	if(selectedValue == 256) 
		return; 

	adventurer.EquipProfession(selectedValue);
};

void equipVehicle	(klib::CCharacter& adventurer) 
{ 
	if( 0 == adventurer.Vehicles.Count )	{ 
		printf("You don't have any other vehicles yet!\n"); 
		return; 
	}

	static char						menuItemText[256] = {};	
	static klib::SMenuItem<int16_t>	menuItems	[256] = {}; 
	int32_t							menuItemCount=0; 

	for(int32_t i=0, count = adventurer.Vehicles.Count; i<count; ++i)	{ 
		sprintf_s(menuItemText, "x%.02i: %s level %u", adventurer.Weapons.Slots[i].Count, klib::getVehicleName(adventurer.Vehicles.Slots[i].Entity).c_str(), adventurer.Vehicles.Slots[i].Entity.Level);
		menuItems[menuItemCount++] = {(int16_t)i, menuItemText}; 
	}	

	printf("You're currently assigned as a %s level %u.\n", klib::getVehicleName(adventurer.CurrentVehicle).c_str(), adventurer.CurrentVehicle.Level);
	menuItems[menuItemCount++] = {256, "Back to tavern"};
	int32_t selectedValue = klib::displayMenu("Select your vehicle of choice", menuItems, menuItemCount);	
	if(selectedValue == 256) 
		return; 

	adventurer.EquipVehicle(selectedValue);
};

void researchWeaponDefinition		(klib::CCharacter& adventurer);
void researchWeaponModifier			(klib::CCharacter& adventurer);
void researchWeaponLevel			(klib::CCharacter& adventurer);
void researchArmorDefinition		(klib::CCharacter& adventurer);
void researchArmorModifier			(klib::CCharacter& adventurer);
void researchArmorLevel				(klib::CCharacter& adventurer);
void researchProfessionDefinition	(klib::CCharacter& adventurer);
void researchProfessionModifier		(klib::CCharacter& adventurer);
void researchProfessionLevel		(klib::CCharacter& adventurer);
void researchVehicleDefinition		(klib::CCharacter& adventurer);
void researchVehicleModifier		(klib::CCharacter& adventurer);
void researchVehicleLevel			(klib::CCharacter& adventurer);

void labs(klib::CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const klib::SMenuItem<int> tavernOptions[] =
	{ { 0, "Research new weapons"		}
	, { 1, "Research new sciences"		}
	, { 2, "Research new armors"		}
	, { 3, "Research new technologies"	}
	, { 4, "Learn new professions"		}
	, { 5, "Train for new ranks"		}
	, { 6, "Research new vehicles"		}
	, { 7, "Research new advancements"	}
	, { 8, "Back to main menu"			}
	};

	while (true)  // Wait for exit request
	{
		int tavernChoice = klib::displayMenu("You take a look at your collected samples..", tavernOptions);

			 if( 0 == tavernChoice )	{	researchWeaponDefinition		(adventurer);	}	// go to the research menu
		else if( 1 == tavernChoice )	{	researchWeaponModifier			(adventurer);	}	// go to the research menu
		else if( 2 == tavernChoice )	{	researchArmorDefinition			(adventurer);	}	// go to the research menu
		else if( 3 == tavernChoice )	{	researchArmorModifier			(adventurer);	}	// go to the research menu
		else if( 4 == tavernChoice )	{	researchProfessionDefinition	(adventurer);	}	// go to the research menu
		else if( 5 == tavernChoice )	{	researchProfessionModifier		(adventurer);	}	// go to the research menu
		else if( 6 == tavernChoice )	{	researchVehicleDefinition		(adventurer);	}	// go to the research menu
		else if( 7 == tavernChoice )	{	researchVehicleModifier			(adventurer);	}	// go to the research menu
		else if( 8 == tavernChoice )	{	break;											}	// Exit the labs menu.
		else {	
			printf("Option not supported yet.\n");
		}	// Exit the main loop, which effectively closes the game.
	}
}


void arsenal(klib::CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const klib::SMenuItem<int> tavernOptions[] =
	{ {  1, "Show inventory"			}
	, {  2, "Show equipment"			}
	, {  3, "Equip weapon"				}
	, {  4, "Equip armor"				}
	, {  5, "Study for next task"		}
	, {  6, "Get into vehicle"			}
	, {  7, "Back to main menu"					}
	};

	while (true)  // Wait for exit request
	{
		int tavernChoice = klib::displayMenu("You wonder about what to do next..", tavernOptions);

			 if( 1 == tavernChoice )	{	displayInventory				(adventurer.Inventory);	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 2 == tavernChoice )	{	displayEquip					(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 3 == tavernChoice )	{	equipWeapon						(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 4 == tavernChoice )	{	equipArmor						(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 5 == tavernChoice )	{	equipProfession					(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 6 == tavernChoice )	{	equipVehicle					(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 7 == tavernChoice )	{	break;							}	// Exit the main loop, which effectively closes the game.
		else {	
			printf("Option not supported yet.\n");
		}	// Exit the main loop, which effectively closes the game.
	}
}

void tavern(klib::CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const klib::SMenuItem<int> tavernOptions[] =
	{ {  1, "Rest"						}
	, {  2, "Look for a mercenary job"	}
	, {  3, "Review arsenal"			}
	, {  4, "Visit research labs"		}
	, {  5, "Go for a drink"			}
	, {  6, "Display score"				}
	, {  7, "Exit game"					}
	};

	while (true)  // Wait for exit request
	{
		int tavernChoice = klib::displayMenu("You wonder about what to do next..", tavernOptions);

			 if( 1	== tavernChoice )	{	rest							(adventurer);	}	// Rest and ask again for the action.
		else if( 2	== tavernChoice )	{	mercenaryJob					(adventurer);	}	// Go for a mercenary job and ask again for action once it's done
		else if( 3 == tavernChoice )	{	arsenal							(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 4 == tavernChoice )	{	labs							(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 5	== tavernChoice )	{	bar								(adventurer);	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 6 == tavernChoice )	{	displayScore					(adventurer.Score);	}	// Display score and player points and ask again for action once it's done.
		else if( 7 == tavernChoice )	{	break;							}	// Exit the main loop, which effectively closes the game.
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
	menuItems[klib::getFinalItemCount()].ReturnValue	= { (int16_t)klib::getFinalItemCount(), 0, 0 };
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
			else if(adventurer.Inventory.AddElement(selectedItem))	// addItem() returns false if the inventory is full.
			{
				printf("You spend %u coins buying %s.\n", itemPrice, itemName.c_str());
				adventurer.Points.Coins		-= itemPrice;
				adventurer.Score.MoneySpent			+= itemPrice;
			}
			else
				printf("Not enough space in inventory!\n");
		}
	}
	displayInventory(adventurer.Inventory);
}

void displayEquip(const klib::CCharacter& adventurer) 
{
	const klib::SEntityPoints&	basePoints		= adventurer.Points;
	const klib::SEntityPoints	finalPoints		= klib::calculateFinalPoints(adventurer);
	const klib::SEntityFlags	finalFlags		= klib::calculateFinalFlags(adventurer);
	
	const klib::SEntityPoints weaponPoints		= klib::getWeaponPoints(adventurer.CurrentWeapon);
	const klib::SEntityPoints armorPoints		= klib::getArmorPoints(adventurer.CurrentArmor);
	const klib::SEntityPoints professionPoints	= klib::getProfessionPoints(adventurer.CurrentProfession);
	const klib::SEntityPoints vehiclePoints		= klib::getVehiclePoints(adventurer.CurrentVehicle);
	
	const klib::SEntityFlags weaponFlags		= klib::getWeaponFlags(adventurer.CurrentWeapon);
	const klib::SEntityFlags armorFlags			= klib::getArmorFlags(adventurer.CurrentArmor);
	const klib::SEntityFlags professionFlags	= klib::getProfessionFlags(adventurer.CurrentProfession);
	const klib::SEntityFlags vehicleFlags		= klib::getVehicleFlags(adventurer.CurrentVehicle);

	printf("\n-- %s final points:\n", adventurer.Name.c_str());
	printf("- Max Life:\n");
	finalPoints.LifeMax.Print();
	printf("- Current Life:\n");
	basePoints.LifeCurrent.Print();
	printf("- Attack:\n");
	finalPoints.Attack.Print();
	printf("- Coins: %i.\n", basePoints.Coins);
	printf("- Bonus Coins per turn: %i.\n", finalPoints.Coins);
	finalFlags.Print();

	printf("\n-- %s base character points:\n", adventurer.Name.c_str());
	printf("- Max Life:\n");
	basePoints.LifeMax.Print();
	printf("- Attack:\n");
	basePoints.Attack.Print();
	finalFlags.Print();

	printf("\n-- %s is a %s level %u:\n", adventurer.Name.c_str(), klib::getProfessionName(adventurer.CurrentProfession).c_str(), adventurer.CurrentProfession.Level);
	professionPoints.Print();
	professionFlags.Print();

	printf("\n-- %s is wearing %s level %u:\n", adventurer.Name.c_str(), klib::getArmorName(adventurer.CurrentArmor).c_str(), adventurer.CurrentArmor.Level);
	armorPoints.Print();
	armorFlags.Print();

	printf("\n-- %s is carrying %s level %u:\n", adventurer.Name.c_str(), klib::getWeaponName(adventurer.CurrentWeapon).c_str(), adventurer.CurrentWeapon.Level);
	weaponPoints.Print();
	weaponFlags.Print();

	printf("\n-- %s is on a %s level %u:\n", adventurer.Name.c_str(), klib::getVehicleName(adventurer.CurrentVehicle).c_str(), adventurer.CurrentVehicle.Level);
	vehiclePoints.Print();
	vehicleFlags.Print();
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
