#include "Game.h"

#include "Profession.h"
#include "Weapon.h"
#include "Item.h"
#include "Enemy.h"
#include "Armor.h"
#include "Vehicle.h"

#include "Menu.h"

#include <algorithm>

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
	for(uint32_t i=0; i<loser.Inventory.Items.Count; i++) 
		if( 0 == (rand()%2) )
		{
			const klib::SEntitySlot<klib::SItem>& itemDrop = loser.Inventory.Items.Slots[i];
			std::string itemDropName = klib::getItemName(itemDrop.Entity);
			if(winner.Inventory.Items.AddElement(itemDrop.Entity)) {
				printf("\n%s dropped %s!!\n", loser.Name.c_str(), itemDropName.c_str());
				loser.Inventory.Items.DecreaseEntity(i);
			}
			else {
				printf("%s can't pick up %s by %s because the inventory is full!\n", winner.Name.c_str(), itemDropName.c_str(), loser.Name.c_str());
			}

		}

	std::string loserWeaponName = klib::getWeaponName(loser.CurrentEquip.Weapon);
	if( 0 == (rand()%2) )
	{
		printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserWeaponName.c_str(), loser.CurrentEquip.Armor.Level, loser.Name.c_str());
		klib::SWeapon oldWinnerWeapon	= winner.CurrentEquip.Weapon;
		winner.Inventory.Weapons.AddElement(loser.CurrentEquip.Weapon);

		klib::SWeapon loserNewWeapon = 
		{	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerWeapon.Index	)	))
		,	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerWeapon.Modifier	)	))
		,	1+(rand() % std::max((int16_t)2, (int16_t)(oldWinnerWeapon.Level	)	))
		};

		if(loserNewWeapon.Index || loserNewWeapon.Modifier || loserNewWeapon.Level > 1)
			printf("%s recovers a used %s level %u from the battlefield.\n", loser.Name.c_str(), klib::getWeaponName(loserNewWeapon).c_str(), loserNewWeapon.Level);
		loser.Inventory.Weapons.AddElement(loserNewWeapon);
		loser.CurrentEquip.Weapon = {0,0,1};
	}
	else
		printf("%s doesn't get to recover %s from %s.\n", winner.Name.c_str(), loserWeaponName.c_str(), loser.Name.c_str());

	std::string loserArmorName = klib::getArmorName(loser.CurrentEquip.Armor);
	if( 0 == (rand()%2) )
	{
		printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserArmorName.c_str(), loser.CurrentEquip.Armor.Level, loser.Name.c_str());
		klib::SArmor oldWinnerArmor	= winner.CurrentEquip.Armor;
		winner.Inventory.Armors.AddElement(loser.CurrentEquip.Armor);

		klib::SArmor loserNewArmor = 
		{	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerArmor.Index		)	))
		,	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerArmor.Modifier	)	))
		,	1+(rand() % std::max((int16_t)2, (int16_t)(oldWinnerArmor.Level		)	))
		};

		if(loserNewArmor.Index || loserNewArmor.Modifier || loserNewArmor.Level > 1)
			printf("%s recovers a used %s level %u from the battlefield.\n", loser.Name.c_str(), klib::getArmorName(loserNewArmor).c_str(), loserNewArmor.Level);
		loser.Inventory.Armors.AddElement(loserNewArmor);
		loser.CurrentEquip.Armor = {0,0,1};
	}
	else
		printf("%s doesn't get to recover %s from %s.\n", winner.Name.c_str(), loserArmorName.c_str(), loser.Name.c_str());

	std::string loserVehicleName = klib::getVehicleName(loser.CurrentEquip.Vehicle);
	if( 0 == (rand()%2) )
	{
		printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserVehicleName.c_str(), loser.CurrentEquip.Vehicle.Level, loser.Name.c_str());
		klib::SVehicle oldWinnerVehicle	= winner.CurrentEquip.Vehicle;
		winner.Inventory.Vehicles.AddElement(loser.CurrentEquip.Vehicle);

		klib::SVehicle loserNewVehicle = 
		{	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerVehicle.Index		)	))
		,	1+(rand() %	std::max((int16_t)2, (int16_t)(oldWinnerVehicle.Modifier	)	))
		,	1+(rand() % std::max((int16_t)2, (int16_t)(oldWinnerVehicle.Level		)	))
		};

		if(loserNewVehicle.Index || loserNewVehicle.Modifier || loserNewVehicle.Level > 1)
			printf("%s recovers a used %s level %u from the battlefield.\n", loser.Name.c_str(), klib::getVehicleName(loserNewVehicle).c_str(), loserNewVehicle.Level);
		loser.Inventory.Vehicles.AddElement(loserNewVehicle);
		loser.CurrentEquip.Vehicle = {0,0,1};
	}
	else
		printf("%s doesn't get to recover %s from %s.\n", winner.Name.c_str(), loserVehicleName.c_str(), loser.Name.c_str());


	std::string loserProfessionName = klib::getProfessionName(loser.CurrentEquip.Profession);
	if(rand()%2)
	{
		printf("%s recovers a %s level %u from %s.\n", winner.Name.c_str(), loserProfessionName.c_str(), loser.CurrentEquip.Profession.Level, loser.Name.c_str());
		winner.Inventory.Professions.AddElement(loser.CurrentEquip.Profession);
	}
	else
		printf("%s doesn't get to recover %s from %s because it's too damaged.\n", winner.Name.c_str(), loserProfessionName.c_str(), loser.Name.c_str());

	klib::SProfession oldLoserProfession	= loser.CurrentEquip.Profession;

	bool bLowestFall=true;
	if(loser.CurrentEquip.Profession.Modifier	)	{ bLowestFall=false; --loser.CurrentEquip.Profession.Modifier;	};
	if(loser.CurrentEquip.Profession.Level > 1)	{ bLowestFall=false; --loser.CurrentEquip.Profession.Level	;	};
	if( bLowestFall )
		printf("%s couldn't fall any lower!!\n", loser.Name.c_str());
	else
		printf("%s loses his job as %s level %u and tries to get a new job as %s level %u instead.\n", loser.Name.c_str(), loserProfessionName.c_str(), oldLoserProfession.Level, klib::getProfessionName(loser.CurrentEquip.Profession).c_str(), loser.CurrentEquip.Profession.Level);

	if(	winner.CurrentEquip.Profession.Modifier < winner.MaxEquip.Profession.Modifier ) {
		winner.CurrentEquip.Profession.Modifier++;
		printf("%s gets promoted to %s!!\n", winner.Name.c_str(), getProfessionName(winner.CurrentEquip.Profession).c_str());
	}
		
		//winner.Points.LifeMax.Health += winner.Points.Attack;
	winner.CurrentEquip.Profession.Level++;
	winner.CurrentEquip.Armor		.Level++;
	winner.CurrentEquip.Weapon	.Level++;

	winner.Score.BattlesWon++;
	winner.Score.EnemiesKilled++;
	winner.Score.MoneyEarned += drop;

	loser.Score.BattlesLost++;

}

void determineOutcome(klib::CCharacter& adventurer, klib::CCharacter& enemy, int32_t enemyType)
{
		// Determine the outcome of the battle and give rewards if applicable.
	if (adventurer.Points.LifeCurrent.Health <= 0) 
		assignDrops(enemy, adventurer);
	if (enemy.Points.LifeCurrent.Health <= 0)
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

	if(outcome == TURN_OUTCOME_CANCEL && target.Points.LifeCurrent.Health > 0 && attacker.Points.LifeCurrent.Health > 0)
		klib::applyTurnStatusAndBonusesAndSkipTurn(attacker);

	return outcome;
}

void printStatuses(const klib::CCharacter& character)
{
	for(uint32_t i=0; i<character.ActiveBonus.Status.Count; ++i)
	{
		const std::string text = klib::getStringFromBit(character.ActiveBonus.Status.Status[i]);
		printf("%s is affected by \"%s\" for the next %u turn(s).\n", character.Name.c_str(), text.c_str(), character.ActiveBonus.Status.TurnsLeft[i]);	
	}
}


void printBonuses(const klib::CCharacter& character)
{
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.Hit					> 0 ) printf("%s has an additional bonus for the next %u turns: %i Hit.\n"						, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.Hit					,	character.ActiveBonus.Points.Points.Attack.Hit					);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.Damage				> 0 ) printf("%s has an additional bonus for the next %u turns: %i Damage.\n"					, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.Damage				,	character.ActiveBonus.Points.Points.Attack.Damage				);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.Speed.Attack		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Attack Speed.\n"				, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.Speed.Attack		,	character.ActiveBonus.Points.Points.Attack.Speed.Attack			);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.Speed.Movement		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Movement Speed.\n"			, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.Speed.Movement		,	character.ActiveBonus.Points.Points.Attack.Speed.Movement		);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.Speed.Reflexes		> 0 ) printf("%s has an additional bonus for the next %u turns: %i Reflexes.\n"					, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.Speed.Reflexes		,	character.ActiveBonus.Points.Points.Attack.Speed.Reflexes		);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.Absorption			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Absorption.\n"				, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.Absorption			,	character.ActiveBonus.Points.Points.Attack.Absorption			);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.ExtraDamage.Health	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Health Damage.\n"			, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.ExtraDamage.Health	,	character.ActiveBonus.Points.Points.Attack.ExtraDamage.Health	);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.ExtraDamage.Mana	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Mana Damage.\n"				, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.ExtraDamage.Mana	,	character.ActiveBonus.Points.Points.Attack.ExtraDamage.Mana	);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Attack.ExtraDamage.Shield	> 0 ) printf("%s has an additional bonus for the next %u turns: %i Shield Damage.\n"			, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Attack.ExtraDamage.Shield	,	character.ActiveBonus.Points.Points.Attack.ExtraDamage.Shield	);
	if( character.ActiveBonus.Points.TurnsLeftPoints.LifeMax.Health				> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max Health.\n"				, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.LifeMax.Health				,	character.ActiveBonus.Points.Points.LifeMax.Health				);
	if( character.ActiveBonus.Points.TurnsLeftPoints.LifeMax.Mana				> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max Mana.\n"					, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.LifeMax.Mana				,	character.ActiveBonus.Points.Points.LifeMax.Mana				);
	if( character.ActiveBonus.Points.TurnsLeftPoints.LifeMax.Shield				> 0 ) printf("%s has an additional bonus for the next %u turns: %i Max Shield.\n"				, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.LifeMax.Shield				,	character.ActiveBonus.Points.Points.LifeMax.Shield				);
	if( character.ActiveBonus.Points.TurnsLeftPoints.LifeCurrent.Health			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Health Recovery.\n"			, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.LifeCurrent.Health			,	character.ActiveBonus.Points.Points.LifeCurrent.Health			);
	if( character.ActiveBonus.Points.TurnsLeftPoints.LifeCurrent.Mana			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Mana Recovery.\n"			, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.LifeCurrent.Mana			,	character.ActiveBonus.Points.Points.LifeCurrent.Mana			);
	if( character.ActiveBonus.Points.TurnsLeftPoints.LifeCurrent.Shield			> 0 ) printf("%s has an additional bonus for the next %u turns: %i Shield Recovery.\n"			, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.LifeCurrent.Shield			,	character.ActiveBonus.Points.Points.LifeCurrent.Shield			);
	if( character.ActiveBonus.Points.TurnsLeftPoints.Coins						> 0 ) printf("%s has an additional bonus for the next %u turns: %i Coin Earning.\n"				, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftPoints.Coins						,	character.ActiveBonus.Points.Points.Coins						);

	// The following messages should be improved by taking every possible status into account.
	if( character.ActiveBonus.Points.TurnsLeftFlags.Effect.Attack				> 0 ) printf("%s has additional flags for the next %u turn(s): Attack Effect   (0x%.04X).\n"	, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftFlags.Effect.Attack				,	character.ActiveBonus.Points.Flags.Effect.Attack				);
	if( character.ActiveBonus.Points.TurnsLeftFlags.Effect.Defend				> 0 ) printf("%s has additional flags for the next %u turn(s): Defend Effect   (0x%.04X).\n"	, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftFlags.Effect.Defend				,	character.ActiveBonus.Points.Flags.Effect.Defend				);
	if( character.ActiveBonus.Points.TurnsLeftFlags.Effect.Passive				> 0 ) printf("%s has additional flags for the next %u turn(s): Passive Effect  (0x%.04X).\n"	, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftFlags.Effect.Passive				,	character.ActiveBonus.Points.Flags.Effect.Passive				);
	if( character.ActiveBonus.Points.TurnsLeftFlags.Status.Immunity				> 0 ) printf("%s has additional flags for the next %u turn(s): Status Immunity (0x%.04X).\n"	, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftFlags.Status.Immunity				,	character.ActiveBonus.Points.Flags.Status.Immunity				);
	if( character.ActiveBonus.Points.TurnsLeftFlags.Status.Inflict				> 0 ) printf("%s has additional flags for the next %u turn(s): Status Inflict  (0x%.04X).\n"	, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftFlags.Status.Inflict				,	character.ActiveBonus.Points.Flags.Status.Inflict				);
	if( character.ActiveBonus.Points.TurnsLeftFlags.Tech.Tech					> 0 ) printf("%s has additional flags for the next %u turn(s): Technology      (0x%.04X).\n"	, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftFlags.Tech.Tech					,	character.ActiveBonus.Points.Flags.Tech.Tech					);
	if( character.ActiveBonus.Points.TurnsLeftFlags.Tech.Grade					> 0 ) printf("%s has additional level for the next %u turn(s): Level           (%.05i).\n"		, character.Name.c_str(),	character.ActiveBonus.Points.TurnsLeftFlags.Tech.Grade					,	character.ActiveBonus.Points.Flags.Tech.Grade					);
}

void printCharacterShortInfo(klib::CCharacter& character)
{
	klib::SEntityPoints characterPoints = calculateFinalPoints(character);
	printf("\n----------------------- %s is a %s level %u.\nWeapon: %s level %u.\nArmor: %s level %u.\n",  
		character.Name.c_str(), 
		klib::getProfessionName	(character.CurrentEquip.Profession)	.c_str(),	character.CurrentEquip.Profession.Level,	
		klib::getWeaponName		(character.CurrentEquip.Weapon)		.c_str(),	character.CurrentEquip.Weapon.Level,	
		klib::getArmorName		(character.CurrentEquip.Armor )		.c_str(),	character.CurrentEquip.Armor.Level);
		
	printf("-------------- Max points:\n");
	characterPoints.LifeMax.Print();
	printf("-------------- Current points:\n");
	character.Points.LifeCurrent.Print();
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

	while (turnOutcome == TURN_OUTCOME_CONTINUE)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
	{
		printCharacterShortInfo	(adventurer);
		printStatuses			(adventurer);
		printBonuses			(adventurer);
		
		printCharacterShortInfo	(currentEnemy);
		printStatuses			(currentEnemy);
		printBonuses			(currentEnemy);

		const TURN_ACTION actionChoice = displayMenu("It's your turn to make a move", combatOptions);
		turnOutcome = characterTurn(actionChoice, adventurer, currentEnemy);
	}
	return turnOutcome;
}

TURN_ACTION resolveAI(klib::CCharacter& enemy, klib::CCharacter& adventurer)
{
	TURN_ACTION action = TURN_ACTION_ATTACK;
	if(enemy.Inventory.Items.Count)
		action = (rand()%2) ? action : TURN_ACTION_INVENTORY;
	else if(enemy.Points.LifeCurrent.Health <= (enemy.Points.LifeMax.Health/9) && 0 == (rand()%7))	// chance of escape attempt if health is less than 11%.
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
	if(adventurer.Points.LifeCurrent.Health <= 1)
	{
		printf("You don't have enough health to engage in combat. Please go rest and come back later.\n");
		return;
	}

	klib::CCharacter currentEnemy = klib::enemyDefinitions[enemyType];	// Request the enemy data.
	klib::setupEnemy(adventurer, currentEnemy, enemyType);

	adventurer.ActiveBonus.Status.Count	= 0;	// We need to clear the combat status before starting the combat.

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while(combatContinues(turnOutcome, adventurer.Points.LifeCurrent.Health, currentEnemy.Points.LifeCurrent.Health))	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
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

		if(!combatContinues(turnOutcome, adventurer.Points.LifeCurrent.Health, currentEnemy.Points.LifeCurrent.Health))
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
	static const size_t inventorySize = size(user.Inventory.Items.Slots);
	if(0 == user.Inventory.Items.Count)
	{
		printf("%s has no items in the inventory.\n", user.Name.c_str());
		return false;
	}

	klib::SMenuItem<int32_t> itemOptions[MAX_INVENTORY_SLOTS+1];
	char itemOption[128] = {};
	for(uint32_t i=0; i<user.Inventory.Items.Count; ++i)
	{
		std::string itemName = klib::getItemName(user.Inventory.Items.Slots[i].Entity);
		sprintf_s(itemOption, "- x%.2u %s", user.Inventory.Items.Slots[i].Count, itemName.c_str());
		itemOptions[i].ReturnValue	= i;
 		itemOptions[i].Text			= itemOption;
	}
	itemOptions[user.Inventory.Items.Count].ReturnValue	= user.Inventory.Items.Count;
	itemOptions[user.Inventory.Items.Count].Text			= "Back to combat options";

	if(user.Type == klib::CHARACTER_TYPE_PLAYER) 
	{
		indexInventory = displayMenu(user.Inventory.Items.Count+1, "Select an item to use", itemOptions);

		if(indexInventory == user.Inventory.Items.Count) // exit option
			bUsedItem = false;
		else if (user.Inventory.Items.Slots[indexInventory].Count <= 0)
			printf("You don't have anymore of that. Use something else...\n"); 
		else {
			// if we reached here it means that the input was valid so we select the description and exit the loop
			bUsedItem = true;
		}
	}
	else // not a player so execute choice by AI
	{
		indexInventory = (int32_t)(rand() % user.Inventory.Items.Count);	// this should be improved.
		const klib::CItem& itemDescription = klib::itemDescriptions[user.Inventory.Items.Slots[indexInventory].Entity.Index];
		
		klib::SEntityPoints	userFinalPoints = klib::calculateFinalPoints(user);
		// Only use potions if we have less than 60% HP
		if	 ( klib::ITEM_TYPE_POTION		!= itemDescription.Type
			|| klib::PROPERTY_TYPE_HEALTH	!= itemDescription.Property
			|| user.Points.LifeCurrent.Health < ((userFinalPoints.LifeMax.Health)*.6f)
			)
			bUsedItem = true;
	}

	if(bUsedItem)
	{
		const klib::SEntityPoints finalPoints = calculateFinalPoints(user);
		const klib::CItem& itemDescription = klib::itemDescriptions[user.Inventory.Items.Slots[indexInventory].Entity.Index];
		if( klib::ITEM_TYPE_POTION == itemDescription.Type 
		 && klib::PROPERTY_TYPE_HEALTH == itemDescription.Property 
		 && user.Points.LifeCurrent.Health == finalPoints.LifeMax.Health)
		{
			bUsedItem = false;
			printf("Your Health is full!");
		}
		else
			bUsedItem = klib::executeItem(indexInventory, user, target);
	}
	
	return bUsedItem;
}