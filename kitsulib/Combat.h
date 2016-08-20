#include "Character.h"

#ifndef __COMBAT_H__923409826342897346920__
#define __COMBAT_H__923409826342897346920__

namespace klib
{
	int32_t			applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName);
	int32_t			applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName);
	STATUS_TYPE		applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t absorbChance, int32_t turnCount, const std::string& sourceName);
	STATUS_TYPE		applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t turnCount, const std::string& sourceName);
	int32_t			applyArmorReflect(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName);
	void			applyCombatBonus(CCharacter& character, const SCharacterPoints& combatBonus, const std::string& sourceName);
	void			applyArmorEffect(CCharacter& character);
	int32_t			applySuccessfulHit(CCharacter& thrower, CCharacter& target, int32_t damage, int32_t absorptionRate, bool bAddStatus, STATUS_TYPE grenadeStatus, int32_t statusTurns, const std::string& sourceName);
	int32_t			applySuccessfulHit(CCharacter& thrower, CCharacter& target, int32_t damage, bool bAddStatus, STATUS_TYPE grenadeStatus, int32_t statusTurns, const std::string& sourceName);
	void			applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName);
	void			applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName);

	int				attack(CCharacter& attacker, CCharacter& target);
	void			applyTurnStatus(CCharacter& character);
	void			applyTurnStatusAndBonusesAndSkipTurn(CCharacter& character);

	bool			useGrenade(const SItem& itemGrenade, CCharacter& thrower, CCharacter& target);
	bool			usePotion(const SItem& itemPotion, CCharacter& potionDrinker);
	bool			executeItem(uint32_t indexInventory, CCharacter& user, CCharacter& target);
}


#endif // __COMBAT_H__923409826342897346920__