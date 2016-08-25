#include "Game.h"

#include "Item.h"
#include "Enemy.h"
#include "Profession.h"
#include "Armor.h"
#include "Weapon.h"
#include "Vehicle.h"

#include "Menu.h"

void displayInventory(const klib::SInventoryItems& inventory, const std::string& characterName)
{
	printf("\n-- %s's inventory --\n", characterName.c_str());
	if(inventory.Count) {
		printf("You look at the remaining supplies...\n");
		for (unsigned int i = 0; i < inventory.Count; i++)
			printf("%u: x%.2u %s.\n", i + 1, inventory.Slots[i].Count, klib::getItemName(inventory.Slots[i].Entity).c_str());
	}
	printf("\n");
}

template<typename _EntityType, size_t _EntityContainerSize, size_t _DefinitionCount, size_t _ModifierCount>
void equipEntityMenu
	( klib::SEntityContainer<_EntityType, _EntityContainerSize>& characterInventory
	, const klib::SResearchGroup& completedResearch 
	, const klib::SEntityRecord<_EntityType>(&tableDefinitions)[_DefinitionCount]
	, const klib::SEntityRecord<_EntityType>(&tableModifiers	)[_ModifierCount]
	, _EntityType& currentEntity
	, const std::string& noWeaponsMessage
	, const std::string& currentlyCarryingMessage
	, const std::string& selectYourChoiceMessage
	, const std::string& cantAccessDefinitionError
	, const std::string& cantAccessModifierError
	, const std::string& storeOldWeaponMessage
	, const std::string& equipNewWeaponMessage
	, const std::string& researchTypeString
	) 
{ 
	if( 0 == characterInventory.Count )	{ 
		printf("%s", noWeaponsMessage.c_str()); 
		return; 
	}
	
	static char						menuItemText[256] = {};	
	static klib::SMenuItem<int16_t>	menuItems	[256] = {}; 
	int32_t							menuItemCount=0; 

	for(int32_t i=0, count = characterInventory.Count; i<count; ++i) { 
		sprintf_s(menuItemText, "x%.02i: %s level %u", characterInventory.Slots[i].Count, klib::getEntityName(characterInventory.Slots[i].Entity, tableDefinitions, tableModifiers).c_str(), characterInventory.Slots[i].Entity.Level);
		menuItems[menuItemCount++] = {(int16_t)i, menuItemText}; 
	}	

	printf(currentlyCarryingMessage.c_str(), klib::getEntityName(currentEntity, tableDefinitions, tableModifiers).c_str(), currentEntity.Level);
	menuItems[menuItemCount++] = {256, "Back to tavern"}; 
	int32_t selectedValue = klib::displayMenu(selectYourChoiceMessage.c_str(), menuItems, menuItemCount);	
	if(selectedValue == 256) 
		return; 

	equipEntityIfResearched(selectedValue, characterInventory, completedResearch, tableDefinitions, tableModifiers, currentEntity, cantAccessDefinitionError, cantAccessModifierError, storeOldWeaponMessage, equipNewWeaponMessage, researchTypeString);
};

void equipWeapon	(klib::CCharacter& adventurer) 
{ 
	equipEntityMenu
		( adventurer.Inventory.Weapons
		, adventurer.Researched.Weapon
		, klib::definitionsWeapon
		, klib::modifiersWeapon
		, adventurer.CurrentEquip.Weapon
		, "You don't have any other weapons yet!\n"
		, "You're currently carrying a %s level %u.\n"
		, "Select your weapon of choice"
		, "You can't access to %s weapons until you have researched them!\n"	
		, "You can't access to %s weapons until you have researched them!\n"	
		, "You store %s level %u in your arsenal.\n"
		, "You equip %s level %u.\n"
		, "Science"
		);
};

void equipArmor		(klib::CCharacter& adventurer) 
{ 
	equipEntityMenu
		( adventurer.Inventory.Armors
		, adventurer.Researched.Armor
		, klib::definitionsArmor
		, klib::modifiersArmor
		, adventurer.CurrentEquip.Armor
		, "You don't have any other armors yet!\n"
		, "You're currently wearing a %s level %u.\n"
		, "Select your armor of choice"
		, "You can't access to %s armors until you have researched them!\n"	
		, "You can't access to %s armors until you have researched them!\n"	
		, "You store %s level %u in your wardrobe.\n"
		, "You equip %s level %u.\n"
		, "Technology"
		);
};

void equipProfession	(klib::CCharacter& adventurer) 
{ 
	equipEntityMenu
		( adventurer.Inventory.Professions
		, adventurer.Researched.Profession
		, klib::definitionsProfession
		, klib::modifiersProfession
		, adventurer.CurrentEquip.Profession
		, "You don't have any other professions yet!\n"
		, "You're currently assigned as a %s level %u.\n"
		, "Select your favorite employee for the next mission"
		, "You can't access to %s employees until you have researched them!\n"	
		, "You can't access to %s until you have researched it!\n"	
		, "You give some free days to your %s level %u.\n"
		, "You call your %s level %u for notifying about the next assignment.\n"
		, "Rank"
		);
};

void equipVehicle	(klib::CCharacter& adventurer) 
{ 
	equipEntityMenu
		( adventurer.Inventory.Vehicles
		, adventurer.Researched.Vehicle
		, klib::definitionsVehicle
		, klib::modifiersVehicle
		, adventurer.CurrentEquip.Vehicle
		, "You don't have any other vehicles yet!\n"
		, "You're currently piloting a %s level %u.\n"
		, "Select your vehicle of choice"
		, "You can't access to %s vehicles until you have researched them!\n"	
		, "You can't access to %s vehicles until you have researched them!\n"	
		, "You leave your %s level %u.\n"
		, "You get ready to pilot your %s level %u.\n"
		, "Mechanics"
		);
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
	{ {  1,		"Select agent for the next mission"		}
	, {  2,		"Equip weapon"							}
	, {  3,		"Equip armor"							}
	, {  4,		"Select vehicle"						}
	, {  5,		"Inspect active agent"					}
	, {  6,		"Inspect equipped weapon"				}
	, {  7,		"Inspect equipped armor"				}
	, {  8,		"Inspect current vehicle"				}
	, {  9,		"Show inventory"						}
	, {  10,	"Back to main menu"						}
	};

	while (true)  // Wait for exit request
	{
		int tavernChoice = klib::displayMenu("You wonder about what to do next..", tavernOptions);

		// Interpret user input.
			 if( 1 == tavernChoice )	{	equipProfession					(adventurer);											}	// 
		else if( 2 == tavernChoice )	{	equipWeapon						(adventurer);											}	// 
		else if( 3 == tavernChoice )	{	equipArmor						(adventurer);											}	// 
		else if( 4 == tavernChoice )	{	equipVehicle					(adventurer);											}	// 
		else if( 5 == tavernChoice )	{	displayProfession				(adventurer);											}	// 
		else if( 6 == tavernChoice )	{	displayWeapon					(adventurer);											}	// 
		else if( 7 == tavernChoice )	{	displayArmor					(adventurer);											}	// 
		else if( 8 == tavernChoice )	{	displayVehicle					(adventurer);											}	// 
		else if( 9 == tavernChoice )	{	displayInventory				(adventurer.Inventory.Items, adventurer.Name.c_str());	}	// 
		else if( 10 == tavernChoice )	{	break;	}	// Exit the main loop, which effectively closes the game.
		else {	
			printf("Option not supported yet.\n");
		}	// Exit the main loop, which effectively closes the game.
	}
}

void tavern(klib::CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const klib::SMenuItem<int> tavernOptions[] =
	{ {	1, "Rest"						}
	, {	2, "Look for a mercenary job"	}
	, {	3, "Review arsenal"				}
	, {	4, "Visit research labs"		}
	, {	5, "Go for a drink"				}
	, {	6, "Display score"				}
	, {	7, "Exit game"					}
	};

	while (true)  // Wait for exit request
	{
		int tavernChoice = klib::displayMenu("You wonder about what to do next..", tavernOptions);

			 if( 1	== tavernChoice )	{	rest							(adventurer);	}	// Rest and ask again for the action.
		else if( 2	== tavernChoice )	{	mercenaryJob					(adventurer);	}	// Go for a mercenary job and ask again for action once it's done
		else if( 3	== tavernChoice )	{	arsenal							(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 4	== tavernChoice )	{	labs							(adventurer);	}	// Go to the shop and ask again for action once it's done.
		else if( 5	== tavernChoice )	{	bar								(adventurer);	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 6	== tavernChoice )	{	displayScore					(adventurer.Score);	}	// Display score and player points and ask again for action once it's done.
		else if( 7	== tavernChoice )	{	break;	}	// Exit the main loop, which effectively closes the game.
		else {	
			printf("Option not supported yet.\n");
		}	// Exit the main loop, which effectively closes the game.
	}
}

void mercenaryJob(klib::CCharacter& adventurer)
{
	// Build menu items.
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
			else if(adventurer.Inventory.Items.AddElement(selectedItem))	// addItem() returns false if the inventory is full.
			{
				printf("You spend %u coins buying %s.\n", itemPrice, itemName.c_str());
				adventurer.Points.Coins		-= itemPrice;
				adventurer.Score.MoneySpent			+= itemPrice;
			}
			else
				printf("Not enough space in inventory!\n");
		}
	}
	displayInventory(adventurer.Inventory.Items, adventurer.Name.c_str());
}

void displayWeapon(const klib::CCharacter& adventurer) 
{
	const klib::SWeapon& currentWeapon = adventurer.CurrentEquip.Weapon;
	const klib::SEntityPoints	weaponPoints		= klib::getWeaponPoints(currentWeapon);
	const klib::SEntityFlags	weaponFlags			= klib::getWeaponFlags(currentWeapon);

	printf("\n-- %s is carrying %s level %u:\n", adventurer.Name.c_str(), klib::getWeaponName(currentWeapon).c_str(), currentWeapon.Level);
	weaponPoints.Print();
	weaponFlags.Print();
}

void displayArmor(const klib::CCharacter& adventurer) 
{
	const klib::SArmor& currentArmor = adventurer.CurrentEquip.Armor;
	const klib::SEntityPoints	armorPoints		= klib::getArmorPoints(currentArmor);
	const klib::SEntityFlags	armorFlags			= klib::getArmorFlags(currentArmor);

	printf("\n-- %s is wearing %s level %u:\n", adventurer.Name.c_str(), klib::getArmorName(currentArmor).c_str(), currentArmor.Level);
	armorPoints.Print();
	armorFlags.Print();
}

void displayProfession(const klib::CCharacter& adventurer) 
{
	const klib::SProfession&	currentProfession = adventurer.CurrentEquip.Profession;
	const klib::SEntityPoints	professionPoints		= klib::getProfessionPoints(currentProfession);
	const klib::SEntityFlags	professionFlags			= klib::getProfessionFlags(currentProfession);

	printf("\n-- %s is commanding a %s level %u:\n", adventurer.Name.c_str(), klib::getProfessionName(currentProfession).c_str(), currentProfession.Level);
	professionPoints.Print();
	professionFlags.Print();
}

void displayVehicle(const klib::CCharacter& adventurer) 
{
	const klib::SVehicle& currentVehicle = adventurer.CurrentEquip.Vehicle;
	const klib::SEntityPoints	vehiclePoints		= klib::getVehiclePoints(currentVehicle);
	const klib::SEntityFlags	vehicleFlags			= klib::getVehicleFlags(currentVehicle);

	printf("\n-- %s is on a %s level %u:\n", adventurer.Name.c_str(), klib::getVehicleName(currentVehicle).c_str(), currentVehicle.Level);
	vehiclePoints.Print();
	vehicleFlags.Print();
}

void displayResume(const klib::CCharacter& adventurer) 
{
	const klib::SEntityPoints&	basePoints		= adventurer.Points;
	const klib::SEntityPoints	finalPoints		= klib::calculateFinalPoints(adventurer);
	const klib::SEntityFlags	finalFlags		= klib::calculateFinalFlags(adventurer);

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
}

void displayEquip(const klib::CCharacter& adventurer) 
{
	displayResume		(adventurer);
	displayProfession	(adventurer);
	displayWeapon		(adventurer);
	displayArmor		(adventurer);
	displayVehicle		(adventurer);
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
