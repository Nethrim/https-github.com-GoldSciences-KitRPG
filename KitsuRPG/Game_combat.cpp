#include "Game.h"

#include "Profession.h"
#include "Weapon.h"
#include "Item.h"
#include "Enemy.h"
#include "Armor.h"

#include "Menu.h"

bool useItems(klib::CCharacter& adventurer, klib::CCharacter& target);	// While in combat, displays a list of the available items to use.

bool escape(const std::string& escaperName, klib::SCharacterScore& escaperScore)
{
	printf("%s tries to escape!\n", escaperName.c_str());
	if ((rand() % 100) < 30) {
		escaperScore.EscapesSucceeded++;
		printf("%s fled from the battle!", escaperName.c_str());
		return true;
	}

	escaperScore.EscapesFailed++;
	printf("%s failed to escape!\n\n", escaperName.c_str());
	return false;
}

void assignDrops(klib::CCharacter& winner, klib::CCharacter& loser)
{
	printf("%s is dead!\n", loser.Name.c_str());
	int drop = rand() % (std::max(1,loser.Points.Coins>>2));

	if(loser.Type == klib::CHARACTER_TYPE_ENEMY)
		drop = loser.Points.Coins-drop;

	printf("\n%s dropped %u coins!!\n", loser.Name.c_str(), drop);
	winner.Points.Coins += drop;
	loser.Points.Coins	-= drop;
	for(uint32_t i=0; i<loser.Inventory.ItemCount; i++) 
		if( 0 == (rand()%2) )
		{
			const klib::SInventorySlot& itemDrop = loser.Inventory.Slots[i];
			std::string itemDropName = klib::getItemName(itemDrop.Item);
			if(klib::addItem(winner.Inventory, itemDrop.Item)) {
				printf("\n%s dropped %s!!\n", loser.Name.c_str(), itemDropName.c_str());
			}
			else {
				printf("%s can't pick up %s by %s because the inventory is full!\n", winner.Name.c_str(), itemDropName.c_str(), loser.Name.c_str());
			}

			klib::removeItem(loser.Inventory, i, loser.Name);
		}

	std::string loserWeaponName = klib::getWeaponName(loser.CurrentWeapon);
	if( 0 == (rand()%2) )
	{
		printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserWeaponName.c_str(), loser.CurrentArmor.Level, loser.Name.c_str());
		klib::SWeapon oldWinnerWeapon	= winner.CurrentWeapon;
		winner.Weapons.AddElement(loser.CurrentWeapon);

		klib::SWeapon loserNewWeapon = 
		{	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerWeapon.Index	)	))
		,	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerWeapon.Modifier	)	))
		,	1+(rand() % std::max((int16_t)2, (int16_t)(oldWinnerWeapon.Level	)	))
		};

		if(loserNewWeapon.Index || loserNewWeapon.Modifier || loserNewWeapon.Level > 1)
			printf("%s recovers a used %s level %u from the battlefield.\n", loser.Name.c_str(), klib::getWeaponName(loserNewWeapon).c_str(), loserNewWeapon.Level);
		loser.Weapons.AddElement(loserNewWeapon);
		loser.CurrentWeapon = {0,0,1};
	}
	else
		printf("%s doesn't get to recover %s from %s.\n", winner.Name.c_str(), loserWeaponName.c_str(), loser.Name.c_str());

	std::string loserArmorName = klib::getArmorName(loser.CurrentArmor);
	if( 0 == (rand()%2) )
	{
		printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserArmorName.c_str(), loser.CurrentArmor.Level, loser.Name.c_str());
		klib::SArmor oldWinnerArmor	= winner.CurrentArmor;
		winner.Armors.AddElement(loser.CurrentArmor);

		klib::SArmor loserNewArmor = 
		{	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerArmor.Index		)	))
		,	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerArmor.Modifier	)	))
		,	1+(rand() % std::max((int16_t)2, (int16_t)(oldWinnerArmor.Level		)	))
		};

		if(loserNewArmor.Index || loserNewArmor.Modifier || loserNewArmor.Level > 1)
			printf("%s recovers a used %s level %u from the battlefield.\n", loser.Name.c_str(), klib::getArmorName(loserNewArmor).c_str(), loserNewArmor.Level);
		loser.Armors.AddElement(loserNewArmor);
		loser.CurrentArmor = {0,0,1};
	}
	else
		printf("%s doesn't get to recover %s from %s.\n", winner.Name.c_str(), loserArmorName.c_str(), loser.Name.c_str());


	std::string loserProfessionName = klib::getProfessionName(loser.CurrentProfession);
	if(rand()%2)
	{
		printf("%s takes some notes about the %s level %u moves displayed by from %s.\n", winner.Name.c_str(), loserProfessionName.c_str(), loser.CurrentProfession.Level, loser.Name.c_str());
		winner.Professions.AddElement(loser.CurrentProfession);
	}
	else
		printf("%s doesn't get to learn %s from %s because of the poor performance displayed in combat.\n", winner.Name.c_str(), loserProfessionName.c_str(), loser.Name.c_str());

	bool bResigned=false;
	klib::SProfession oldLoserProfession	= loser.CurrentProfession;
	//if(loser.CurrentProfession.Modifier	)	{ bResigned=true; --loser.CurrentProfession.Modifier;	};
	if(loser.CurrentProfession.Level > 1)	{ bResigned=true; --loser.CurrentProfession.Level	;	};
		if( 1 == oldLoserProfession.Level )
		 //&& 0 == oldLoserProfession.Modifier )
			printf("%s couldn't fall any lower!!", loser.Name.c_str());
		else
			printf("%s loses his job as %s level %u and tries to get a new job as %s level %u instead.\n", loser.Name.c_str(), loserProfessionName.c_str(), oldLoserProfession.Level, klib::getProfessionName(loser.CurrentProfession).c_str(), loser.CurrentProfession.Level);

	//winner.Points.MaxLife.Health += winner.Points.Attack;
	winner.CurrentProfession.Level++;
	winner.CurrentArmor.Level++;
	winner.CurrentWeapon.Level++;

	winner.Score.BattlesWon++;
	winner.Score.EnemiesKilled++;
	winner.Score.MoneyEarned += drop;

	loser.Score.BattlesLost++;

}

void determineOutcome(klib::CCharacter& adventurer, klib::CCharacter& enemy, int32_t enemyType)
{
		// Determine the outcome of the battle and give rewards if applicable.
	if (adventurer.Points.CurrentLife.Health <= 0) 
		assignDrops(enemy, adventurer);
	if (enemy.Points.CurrentLife.Health <= 0)
		assignDrops(adventurer, enemy);
}

enum TURN_ACTION
{	TURN_ACTION_ATTACK
,	TURN_ACTION_INVENTORY
,	TURN_ACTION_SKILL
,	TURN_ACTION_SENSE
,	TURN_ACTION_RUN
};

enum TURN_OUTCOME
{	TURN_OUTCOME_CONTINUE
,	TURN_OUTCOME_ESCAPE
,	TURN_OUTCOME_CANCEL
};

bool useSkills(klib::CCharacter& attacker, klib::CCharacter& target)
{
	printf("\n");
	printf("Skills are not implemented yet.\n");
	return false;
}

TURN_OUTCOME characterTurn(TURN_ACTION combatOption, klib::CCharacter& attacker, klib::CCharacter& target)
{
	// If the action is valid then we execute it and break the current while() so the attack turn executes.
	TURN_OUTCOME outcome = TURN_OUTCOME_CANCEL;
	if(TURN_ACTION_ATTACK == combatOption) {
		if( !klib::attack(attacker, target) )
			outcome = TURN_OUTCOME_CONTINUE;
	}
	else if(TURN_ACTION_INVENTORY == combatOption) { 
		if( !useItems(attacker, target) )
			outcome = TURN_OUTCOME_CONTINUE;
	}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
	else if(TURN_ACTION_SKILL == combatOption) { 
		if( !useSkills(attacker, target) )
			outcome = TURN_OUTCOME_CONTINUE;
	}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
	else if(TURN_ACTION_SENSE == combatOption) { 
		displayEquip(target);
		outcome = TURN_OUTCOME_CONTINUE;
	}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
	else if(TURN_ACTION_RUN == combatOption) {
		if( escape(attacker.Name, attacker.Score) )
			outcome = TURN_OUTCOME_ESCAPE; // Escape: if we succeed we just exit this combat() function, otherwise cancel this loop and execute the enemy turn.
	}

	if(outcome == TURN_OUTCOME_CANCEL && target.Points.CurrentLife.Health > 0 && attacker.Points.CurrentLife.Health > 0)
		klib::applyTurnStatusAndBonusesAndSkipTurn(attacker);

	return outcome;
}

void printStatuses(const klib::CCharacter& character)
{
	for(uint32_t i=0; i<character.CombatStatus.Count; ++i)
	{
		const std::string text = klib::getStringFromBit(character.CombatStatus.Status[i]);
		printf("%s is affected by \"%s\" for the next %u turn(s).\n", character.Name.c_str(), text.c_str(), character.CombatStatus.TurnsLeft[i]);	
	}
}


void printBonuses(const klib::CCharacter& character)
{
	if( character.CombatBonus.TurnsLeft.Attack.Hit			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Hit.\n"				, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Attack.Hit			,	character.CombatBonus.Points.Attack.Hit			);
	if( character.CombatBonus.TurnsLeft.Attack.Damage		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Damage.\n"			, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Attack.Damage		,	character.CombatBonus.Points.Attack.Damage		);
	if( character.CombatBonus.TurnsLeft.MaxLife.Health		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max HP.\n"			, character.Name.c_str(),	character.CombatBonus.TurnsLeft.MaxLife.Health		,	character.CombatBonus.Points.MaxLife.Health		);
	if( character.CombatBonus.TurnsLeft.MaxLife.Mana		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max Mana.\n"			, character.Name.c_str(),	character.CombatBonus.TurnsLeft.MaxLife.Mana		,	character.CombatBonus.Points.MaxLife.Mana		);
	if( character.CombatBonus.TurnsLeft.MaxLife.Shield		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max Shield.\n"		, character.Name.c_str(),	character.CombatBonus.TurnsLeft.MaxLife.Shield		,	character.CombatBonus.Points.MaxLife.Shield		);
	if( character.CombatBonus.TurnsLeft.CurrentLife.Health	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Health Recovery.\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.CurrentLife.Health	,	character.CombatBonus.Points.CurrentLife.Health	);
	if( character.CombatBonus.TurnsLeft.CurrentLife.Mana	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Mana Recovery.\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.CurrentLife.Mana	,	character.CombatBonus.Points.CurrentLife.Mana	);
	if( character.CombatBonus.TurnsLeft.CurrentLife.Shield	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Shield Recovery.\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.CurrentLife.Shield	,	character.CombatBonus.Points.CurrentLife.Shield	);
	if( character.CombatBonus.TurnsLeft.Coins				> 0 ) printf("%s has an additional bonus for the next %u turns: %i Coin Earning.\n"		, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Coins				,	character.CombatBonus.Points.Coins				);

	// The following messages should be improved by taking every possible status into account.
	if( character.CombatBonus.TurnsLeft.Effect.Attack	> 0 ) printf("%s has additional flags for the next %u turn(s): Attack Effect   (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Effect.Attack	, character.CombatBonus.Points.Effect.Attack	);
	if( character.CombatBonus.TurnsLeft.Effect.Defend	> 0 ) printf("%s has additional flags for the next %u turn(s): Defend Effect   (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Effect.Defend	, character.CombatBonus.Points.Effect.Defend	);
	if( character.CombatBonus.TurnsLeft.Effect.Passive	> 0 ) printf("%s has additional flags for the next %u turn(s): Passive Effect  (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Effect.Passive	, character.CombatBonus.Points.Effect.Passive	);
	if( character.CombatBonus.TurnsLeft.Status.Immunity	> 0 ) printf("%s has additional flags for the next %u turn(s): Status Immunity (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Status.Immunity	, character.CombatBonus.Points.Status.Immunity	);
	if( character.CombatBonus.TurnsLeft.Status.Inflict	> 0 ) printf("%s has additional flags for the next %u turn(s): Status Inflict  (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Status.Inflict	, character.CombatBonus.Points.Status.Inflict	);
	if( character.CombatBonus.TurnsLeft.Tech.Tech		> 0 ) printf("%s has additional flags for the next %u turn(s): Technology      (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Tech.Tech		, character.CombatBonus.Points.Tech.Tech		);
	if( character.CombatBonus.TurnsLeft.Tech.Level		> 0 ) printf("%s has additional level for the next %u turn(s): Level           (%.05i).\n"		, character.Name.c_str(),	character.CombatBonus.TurnsLeft.Tech.Level		, character.CombatBonus.Points.Tech.Level		);
}

void printCharacterShortInfo(klib::CCharacter& character)
{
	klib::SCharacterPoints characterPoints = calculateFinalPoints(character);
	printf("\n----------------------- %s is a %s level %u.\nWeapon: %s level %u.\nArmor: %s level %u.\n",  
		character.Name.c_str(), 
		klib::getProfessionName	(character.CurrentProfession)	.c_str(),	character.CurrentProfession.Level,	
		klib::getWeaponName		(character.CurrentWeapon)		.c_str(),	character.CurrentWeapon.Level,	
		klib::getArmorName		(character.CurrentArmor )		.c_str(),	character.CurrentArmor.Level);
		
	printf("-------------- Max points:\n");
	characterPoints.MaxLife.Print();
	printf("-------------- Current points:\n");
	character.Points.CurrentLife.Print();
	characterPoints.Attack.Print();
}

TURN_OUTCOME playerTurn(klib::CCharacter& adventurer, klib::CCharacter& currentEnemy)
{
	static const klib::SMenuItem<TURN_ACTION> combatOptions[] =
	{ { TURN_ACTION_ATTACK		, "Attack"		}
	, { TURN_ACTION_INVENTORY	, "Inventory"	}
	, { TURN_ACTION_SKILL		, "Skills"		}
	, { TURN_ACTION_SENSE		, "Sense"		}
	, { TURN_ACTION_RUN			, "Run"			}
	};

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;

	klib::SCharacterPoints playerPoints = calculateFinalPoints(adventurer);
	klib::SCharacterPoints enemyPoints = calculateFinalPoints(currentEnemy);
	while (turnOutcome == TURN_OUTCOME_CONTINUE)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
	{
		printCharacterShortInfo(adventurer);
		printStatuses(adventurer);
		printBonuses(adventurer);
		
		printCharacterShortInfo(currentEnemy);
		printStatuses(currentEnemy);
		printBonuses(currentEnemy);

		const TURN_ACTION actionChoice = displayMenu("It's your turn to make a move", combatOptions);
		turnOutcome = characterTurn(actionChoice, adventurer, currentEnemy);
	}
	return turnOutcome;
}

TURN_ACTION resolveAI(klib::CCharacter& enemy, klib::CCharacter& adventurer)
{
	TURN_ACTION action = TURN_ACTION_ATTACK;
	if(enemy.Inventory.ItemCount)
		action = (rand()%2) ? action : TURN_ACTION_INVENTORY;
	else if(enemy.Points.CurrentLife.Health <= (enemy.Points.MaxLife.Health/9) && 0 == (rand()%7))	// chance of escape attempt if health is less than 11%.
		action = TURN_ACTION_RUN;

	return action;
}

TURN_OUTCOME enemyTurn(klib::CCharacter& enemy, klib::CCharacter& adventurer)
{
	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while (turnOutcome == TURN_OUTCOME_CONTINUE) {	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
		const TURN_ACTION actionChoice = (TURN_ACTION)resolveAI(enemy, adventurer);
		turnOutcome = characterTurn(actionChoice, enemy, adventurer);
	}
	return turnOutcome;
}

bool combatContinues(TURN_OUTCOME turnOutcome, int adventurerHP, int enemyHP)
{
	if( TURN_OUTCOME_ESCAPE == turnOutcome || 0 >= adventurerHP || 0 >= enemyHP )
		return false;
	return true;
}

//5736	// gasty.bellino@gmail.com
void combat(klib::CCharacter& adventurer, int32_t enemyType)
{
	if(adventurer.Points.CurrentLife.Health <= 1)
	{
		printf("You don't have enough health to engage in combat. Please go rest and come back later.\n");
		return;
	}

	klib::CCharacter currentEnemy = klib::enemyDefinitions[enemyType];	// Request the enemy data.
	klib::setupEnemy(adventurer, currentEnemy, enemyType);

	adventurer.CombatStatus.Count	= 0;	// We need to clear the combat status before starting the combat.

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while(combatContinues(turnOutcome, adventurer.Points.CurrentLife.Health, currentEnemy.Points.CurrentLife.Health))	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		adventurer.Score.TurnsPlayed++;
		currentEnemy.Score.TurnsPlayed++;

		if(0 < adventurer.CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_STUN) || 0 < adventurer.CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_SLEEP) || 0 < adventurer.CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_FROZEN))
		{
			printf("%s is stunned/asleep and loses his turn!\n", adventurer.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
			klib::applyTurnStatusAndBonusesAndSkipTurn(adventurer);
		}
		else
			turnOutcome = playerTurn(adventurer, currentEnemy);

		if(!combatContinues(turnOutcome, adventurer.Points.CurrentLife.Health, currentEnemy.Points.CurrentLife.Health))
			break;

		// Execute enemy attack turn
		if(0 < currentEnemy.CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_STUN) || 0 < currentEnemy.CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_SLEEP) || 0 < currentEnemy.CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_FROZEN))
		{
			printf("%s is stunned/asleep and loses his turn!\n", currentEnemy.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
			klib::applyTurnStatusAndBonusesAndSkipTurn(currentEnemy);
		}
		else
			turnOutcome = enemyTurn(currentEnemy, adventurer);
	}

	determineOutcome(adventurer, currentEnemy, enemyType);
}


// This function returns true if an item was used or false if the menu was exited without doing anything.
bool useItems(klib::CCharacter& user, klib::CCharacter& target)
{
	bool bUsedItem = false;
	int32_t indexInventory = ~0U;
	static const size_t inventorySize = size(user.Inventory.Slots);
	if(0 == user.Inventory.ItemCount)
	{
		printf("%s has no items in the inventory.\n", user.Name.c_str());
		return false;
	}

	klib::SMenuItem<int32_t> itemOptions[MAX_INVENTORY_SLOTS+1];
	char itemOption[128] = {};
	for(uint32_t i=0; i<user.Inventory.ItemCount; ++i)
	{
		std::string itemName = klib::getItemName(user.Inventory.Slots[i].Item);
		sprintf_s(itemOption, "- x%.2u %s", user.Inventory.Slots[i].ItemCount, itemName.c_str());
		itemOptions[i].ReturnValue	= i;
 		itemOptions[i].Text			= itemOption;
	}
	itemOptions[user.Inventory.ItemCount].ReturnValue	= user.Inventory.ItemCount;
	itemOptions[user.Inventory.ItemCount].Text			= "Back to combat options";

	if(user.Type == klib::CHARACTER_TYPE_PLAYER) 
	{
		indexInventory = displayMenu(user.Inventory.ItemCount+1, "Select an item to use", itemOptions);

		if(indexInventory == user.Inventory.ItemCount) // exit option
			bUsedItem = false;
		else if (user.Inventory.Slots[indexInventory].ItemCount <= 0)
			printf("You don't have anymore of that. Use something else...\n"); 
		else {
			// if we reached here it means that the input was valid so we select the description and exit the loop
			bUsedItem = true;
		}
	}
	else // not a player so execute choice by AI
	{
		indexInventory = (int32_t)(rand() % user.Inventory.ItemCount);	// this should be improved.
		const klib::CItem& itemDescription = klib::itemDescriptions[user.Inventory.Slots[indexInventory].Item.Index];
		
		// Only use potions if we have less than 80% HP
		if	 ( klib::ITEM_TYPE_POTION		!= itemDescription.Type
			|| klib::PROPERTY_TYPE_HEALTH != itemDescription.Property
			|| user.Points.CurrentLife.Health < ((user.Points.MaxLife.Health+user.CombatBonus.Points.MaxLife.Health)*.7f)
			)
			bUsedItem = true;
	}

	if(bUsedItem)
	{
		const klib::SCharacterPoints finalPoints = calculateFinalPoints(user);
		const klib::CItem& itemDescription = klib::itemDescriptions[user.Inventory.Slots[indexInventory].Item.Index];
		if( klib::ITEM_TYPE_POTION == itemDescription.Type 
		 && klib::PROPERTY_TYPE_HEALTH == itemDescription.Property 
		 && user.Points.CurrentLife.Health == finalPoints.MaxLife.Health)
		{
			bUsedItem = false;
			printf("Your HP is full!");
		}
		else
			bUsedItem = klib::executeItem(indexInventory, user, target);
	}
	
	return bUsedItem;
}