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
	int drop = rand() % (std::max(1,loser.Points.Points.Coins>>2));

	if(loser.Type == klib::CHARACTER_TYPE_ENEMY)
		drop = loser.Points.Points.Coins-drop;

	printf("\n%s dropped %u coins!!\n", loser.Name.c_str(), drop);
	winner.Points.Points.Coins += drop;
	loser.Points.Points.Coins	-= drop;
	for(uint32_t i=0; i<loser.Inventory.Count; i++) 
		if( 0 == (rand()%2) )
		{
			const klib::SEntitySlot<klib::SItem>& itemDrop = loser.Inventory.Slots[i];
			std::string itemDropName = klib::getItemName(itemDrop.Entity);
			if(winner.Inventory.AddElement(itemDrop.Entity)) {
				printf("\n%s dropped %s!!\n", loser.Name.c_str(), itemDropName.c_str());
				loser.Inventory.DecreaseEntity(i);
			}
			else {
				printf("%s can't pick up %s by %s because the inventory is full!\n", winner.Name.c_str(), itemDropName.c_str(), loser.Name.c_str());
			}

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

	//winner.Points.LifeMax.Health += winner.Points.Attack;
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
	if (adventurer.Points.Points.LifeCurrent.Health <= 0) 
		assignDrops(enemy, adventurer);
	if (enemy.Points.Points.LifeCurrent.Health <= 0)
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

	if(outcome == TURN_OUTCOME_CANCEL && target.Points.Points.LifeCurrent.Health > 0 && attacker.Points.Points.LifeCurrent.Health > 0)
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
	if( character.CombatBonus.TurnsLeftPoints.Attack.Hit			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Hit.\n"						, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.Attack.Hit			,	character.CombatBonus.Points.Attack.Hit			);
	if( character.CombatBonus.TurnsLeftPoints.Attack.Damage			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Damage.\n"					, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.Attack.Damage			,	character.CombatBonus.Points.Attack.Damage		);
	if( character.CombatBonus.TurnsLeftPoints.LifeMax.Health		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max HP.\n"					, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.LifeMax.Health		,	character.CombatBonus.Points.LifeMax.Health		);
	if( character.CombatBonus.TurnsLeftPoints.LifeMax.Mana			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max Mana.\n"					, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.LifeMax.Mana			,	character.CombatBonus.Points.LifeMax.Mana		);
	if( character.CombatBonus.TurnsLeftPoints.LifeMax.Shield		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max Shield.\n"				, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.LifeMax.Shield		,	character.CombatBonus.Points.LifeMax.Shield		);
	if( character.CombatBonus.TurnsLeftPoints.LifeCurrent.Health	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Health Recovery.\n"			, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.LifeCurrent.Health	,	character.CombatBonus.Points.LifeCurrent.Health	);
	if( character.CombatBonus.TurnsLeftPoints.LifeCurrent.Mana		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Mana Recovery.\n"			, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.LifeCurrent.Mana		,	character.CombatBonus.Points.LifeCurrent.Mana	);
	if( character.CombatBonus.TurnsLeftPoints.LifeCurrent.Shield	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Shield Recovery.\n"			, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.LifeCurrent.Shield	,	character.CombatBonus.Points.LifeCurrent.Shield	);
	if( character.CombatBonus.TurnsLeftPoints.Coins					> 0 ) printf("%s has an additional bonus for the next %u turns: %i Coin Earning.\n"				, character.Name.c_str(),	character.CombatBonus.TurnsLeftPoints.Coins					,	character.CombatBonus.Points.Coins				);

	// The following messages should be improved by taking every possible status into account.
	if( character.CombatBonus.TurnsLeftFlags.Effect.Attack			> 0 ) printf("%s has additional flags for the next %u turn(s): Attack Effect   (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeftFlags.Effect.Attack			,	character.CombatBonus.Flags.Effect.Attack		);
	if( character.CombatBonus.TurnsLeftFlags.Effect.Defend			> 0 ) printf("%s has additional flags for the next %u turn(s): Defend Effect   (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeftFlags.Effect.Defend			,	character.CombatBonus.Flags.Effect.Defend		);
	if( character.CombatBonus.TurnsLeftFlags.Effect.Passive			> 0 ) printf("%s has additional flags for the next %u turn(s): Passive Effect  (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeftFlags.Effect.Passive			,	character.CombatBonus.Flags.Effect.Passive		);
	if( character.CombatBonus.TurnsLeftFlags.Status.Immunity		> 0 ) printf("%s has additional flags for the next %u turn(s): Status Immunity (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeftFlags.Status.Immunity		,	character.CombatBonus.Flags.Status.Immunity		);
	if( character.CombatBonus.TurnsLeftFlags.Status.Inflict			> 0 ) printf("%s has additional flags for the next %u turn(s): Status Inflict  (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeftFlags.Status.Inflict			,	character.CombatBonus.Flags.Status.Inflict		);
	if( character.CombatBonus.TurnsLeftFlags.Tech.Tech				> 0 ) printf("%s has additional flags for the next %u turn(s): Technology      (0x%.04X).\n"	, character.Name.c_str(),	character.CombatBonus.TurnsLeftFlags.Tech.Tech				,	character.CombatBonus.Flags.Tech.Tech			);
	if( character.CombatBonus.TurnsLeftFlags.Tech.Grade				> 0 ) printf("%s has additional level for the next %u turn(s): Level           (%.05i).\n"		, character.Name.c_str(),	character.CombatBonus.TurnsLeftFlags.Tech.Grade				,	character.CombatBonus.Flags.Tech.Grade			);
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
	characterPoints.Points.LifeMax.Print();
	printf("-------------- Current points:\n");
	character.Points.Points.LifeCurrent.Print();
	characterPoints.Points.Attack.Print();
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
	if(enemy.Inventory.Count)
		action = (rand()%2) ? action : TURN_ACTION_INVENTORY;
	else if(enemy.Points.Points.LifeCurrent.Health <= (enemy.Points.Points.LifeMax.Health/9) && 0 == (rand()%7))	// chance of escape attempt if health is less than 11%.
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
	if(adventurer.Points.Points.LifeCurrent.Health <= 1)
	{
		printf("You don't have enough health to engage in combat. Please go rest and come back later.\n");
		return;
	}

	klib::CCharacter currentEnemy = klib::enemyDefinitions[enemyType];	// Request the enemy data.
	klib::setupEnemy(adventurer, currentEnemy, enemyType);

	adventurer.CombatStatus.Count	= 0;	// We need to clear the combat status before starting the combat.

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while(combatContinues(turnOutcome, adventurer.Points.Points.LifeCurrent.Health, currentEnemy.Points.Points.LifeCurrent.Health))	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		adventurer.Score.TurnsPlayed++;
		currentEnemy.Score.TurnsPlayed++;

		if(adventurer.DidLoseTurn())
		{
			printf("%s is stunned/asleep and loses his turn!\n", adventurer.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
			klib::applyTurnStatusAndBonusesAndSkipTurn(adventurer);
		}
		else
			turnOutcome = playerTurn(adventurer, currentEnemy);

		if(!combatContinues(turnOutcome, adventurer.Points.Points.LifeCurrent.Health, currentEnemy.Points.Points.LifeCurrent.Health))
			break;

		// Execute enemy attack turn
		if(currentEnemy.DidLoseTurn())
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
	if(0 == user.Inventory.Count)
	{
		printf("%s has no items in the inventory.\n", user.Name.c_str());
		return false;
	}

	klib::SMenuItem<int32_t> itemOptions[MAX_INVENTORY_SLOTS+1];
	char itemOption[128] = {};
	for(uint32_t i=0; i<user.Inventory.Count; ++i)
	{
		std::string itemName = klib::getItemName(user.Inventory.Slots[i].Entity);
		sprintf_s(itemOption, "- x%.2u %s", user.Inventory.Slots[i].Count, itemName.c_str());
		itemOptions[i].ReturnValue	= i;
 		itemOptions[i].Text			= itemOption;
	}
	itemOptions[user.Inventory.Count].ReturnValue	= user.Inventory.Count;
	itemOptions[user.Inventory.Count].Text			= "Back to combat options";

	if(user.Type == klib::CHARACTER_TYPE_PLAYER) 
	{
		indexInventory = displayMenu(user.Inventory.Count+1, "Select an item to use", itemOptions);

		if(indexInventory == user.Inventory.Count) // exit option
			bUsedItem = false;
		else if (user.Inventory.Slots[indexInventory].Count <= 0)
			printf("You don't have anymore of that. Use something else...\n"); 
		else {
			// if we reached here it means that the input was valid so we select the description and exit the loop
			bUsedItem = true;
		}
	}
	else // not a player so execute choice by AI
	{
		indexInventory = (int32_t)(rand() % user.Inventory.Count);	// this should be improved.
		const klib::CItem& itemDescription = klib::itemDescriptions[user.Inventory.Slots[indexInventory].Entity.Index];
		
		// Only use potions if we have less than 80% HP
		if	 ( klib::ITEM_TYPE_POTION		!= itemDescription.Type
			|| klib::PROPERTY_TYPE_HEALTH != itemDescription.Property
			|| user.Points.Points.LifeCurrent.Health < ((user.Points.Points.LifeMax.Health+user.CombatBonus.Points.LifeMax.Health)*.7f)
			)
			bUsedItem = true;
	}

	if(bUsedItem)
	{
		const klib::SCharacterPoints finalPoints = calculateFinalPoints(user);
		const klib::CItem& itemDescription = klib::itemDescriptions[user.Inventory.Slots[indexInventory].Entity.Index];
		if( klib::ITEM_TYPE_POTION == itemDescription.Type 
		 && klib::PROPERTY_TYPE_HEALTH == itemDescription.Property 
		 && user.Points.Points.LifeCurrent.Health == finalPoints.Points.LifeMax.Health)
		{
			bUsedItem = false;
			printf("Your HP is full!");
		}
		else
			bUsedItem = klib::executeItem(indexInventory, user, target);
	}
	
	return bUsedItem;
}