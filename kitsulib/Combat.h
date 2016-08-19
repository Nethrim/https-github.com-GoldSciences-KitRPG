#include "Character.h"

#ifndef __COMBAT_H__923409826342897346920__
#define __COMBAT_H__923409826342897346920__

namespace klib
{
	int32_t			applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName);
	int32_t			applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName);
	void			useGrenade(const SItem& itemGrenade, CCharacter& thrower, CCharacter& target);
	void			usePotion(const SItem& itemPotion, CCharacter& potionDrinker);
	STATUS_TYPE		applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t absorbChance, int32_t turnCount, const std::string& sourceName);
	STATUS_TYPE		applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t turnCount, const std::string& sourceName);
	int32_t			applyArmorReflect(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName);
	void			applyCombatBonus(CCharacter& character, const SCharacterPoints& combatBonus, const std::string& sourceName);
	void			applyArmorEffect(CCharacter& character);
	void			applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName);
	void			applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName);

	int				attack(CCharacter& attacker, CCharacter& target);
	void			applyTurnStatus(CCharacter& character);
	void			applyTurnStatusAndBonusesAndSkipTurn(CCharacter& character);
	void			executeItem(uint32_t indexInventory, CCharacter& user, CCharacter& target);
}


#endif // __COMBAT_H__923409826342897346920__