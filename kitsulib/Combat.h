#include "Character.h"

#ifndef __COMBAT_H__923409826342897346920__
#define __COMBAT_H__923409826342897346920__

namespace klib
{
#pragma pack(push, 1)
	SLifePoints			applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName);
	SLifePoints			applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName);
	COMBAT_STATUS		applyAttackStatus(CCharacter& target, COMBAT_STATUS weaponStatus, int32_t absorbChance, int32_t turnCount, const std::string& sourceName);
	COMBAT_STATUS		applyAttackStatus(CCharacter& target, COMBAT_STATUS weaponStatus, int32_t turnCount, const std::string& sourceName);
	int32_t				applyArmorReflect(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName);
	void				applyCombatBonus(CCharacter& character, const SEntityPoints& combatBonus, const std::string& sourceName);
	void				applyWeaponLeechEffects(CCharacter& attacker, CCharacter& targetReflecting, const SLifePoints& finalDamage, const std::string& sourceName);
	SLifePoints			applySuccessfulHit(CCharacter& thrower, CCharacter& target, int32_t damage, int32_t absorptionRate, bool bAddStatus, COMBAT_STATUS grenadeStatus, int32_t statusTurns, const std::string& sourceName);
	SLifePoints			applySuccessfulHit(CCharacter& thrower, CCharacter& target, int32_t damage, bool bAddStatus, COMBAT_STATUS grenadeStatus, int32_t statusTurns, const std::string& sourceName);
	void				applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName);
	void				applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName);
	bool				attack(CCharacter& attacker, CCharacter& target);
	void				applyTurnStatus(CCharacter& character);
	void				applyTurnStatusAndBonusesAndSkipTurn(CCharacter& character);

	bool				useGrenade(const SItem& itemGrenade, CCharacter& thrower, CCharacter& target);
	bool				usePotion(const SItem& itemPotion, CCharacter& potionDrinker);
	bool				executeItem(int32_t indexInventory, CCharacter& user, CCharacter& target);
#pragma pack(pop)
}


#endif // __COMBAT_H__923409826342897346920__