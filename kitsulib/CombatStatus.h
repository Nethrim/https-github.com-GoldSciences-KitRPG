#include <cstdint>

#include <string>

#ifndef	__COMBATSTATUS_H__2386498236498213469817263421__
#define	__COMBATSTATUS_H__2386498236498213469817263421__


namespace klib
{
#pragma pack(push, 1)

#define MAX_COMBAT_STATUS_COUNT 32
	enum COMBAT_STATUS : int32_t
	{	COMBAT_STATUS_NONE		= 0x00000
	,	COMBAT_STATUS_BLIND		= 0x00001
	,	COMBAT_STATUS_STUN		= 0x00002
	,	COMBAT_STATUS_SHOCK		= 0x00004
	,	COMBAT_STATUS_BLEEDING	= 0x00008
	,	COMBAT_STATUS_BURN		= 0x00010
	,	COMBAT_STATUS_POISON	= 0x00020
	,	COMBAT_STATUS_FREEZING	= 0x00040
	,	COMBAT_STATUS_PETRIFY	= 0x00080
	,	COMBAT_STATUS_FROZEN	= 0x00100
	,	COMBAT_STATUS_BLACKOUT	= 0x00200
	,	COMBAT_STATUS_CHARMED	= 0x00400
	,	COMBAT_STATUS_DRUNK		= 0x00800
	,	COMBAT_STATUS_SLEEP		= 0x01000
	,	COMBAT_STATUS_SLOW		= 0x02000
	,	COMBAT_STATUS_WEAKNESS	= 0x04000
	,	COMBAT_STATUS_PANIC		= 0x08000
	,	COMBAT_STATUS_BERSERK	= 0x10000
	,	COMBAT_STATUS_INVISIBLE	= 0x20000
	};

#define MAX_ATTACK_EFFECT_COUNT 2
	enum ATTACK_EFFECT : uint16_t
	{	ATTACK_EFFECT_NONE			= 0x00
	,	ATTACK_EFFECT_LEECH_HEALTH	= 0x01
	,	ATTACK_EFFECT_LEECH_MANA	= 0x02
	,	ATTACK_EFFECT_LEECH_SHIELD	= 0x03
	,	ATTACK_EFFECT_STEAL			= 0x04
	};

#define MAX_DEFEND_EFFECT_COUNT 3
	enum DEFEND_EFFECT : uint16_t
	{	DEFEND_EFFECT_NONE			= 0x00
	,	DEFEND_EFFECT_REFLECT		= 0x01	// Reflects damage to the attacker
	,	DEFEND_EFFECT_BLIND			= 0x02	// Blinds the attacker when inflicting 1 to 1 damage.
	,	DEFEND_EFFECT_IMPENETRABLE	= 0x04	// Reflects damage to the attacker
	,	DEFEND_EFFECT_DEFLECT		= 0x08	// Reflects damage to the attacker
	};

#define MAX_PASSIVE_EFFECT_COUNT 3
	enum PASSIVE_EFFECT : uint16_t
	{	PASSIVE_EFFECT_NONE				= 0x00
	,	PASSIVE_EFFECT_LIFE_REGEN		= 0x01
	,	PASSIVE_EFFECT_MANA_REGEN		= 0x02
	,	PASSIVE_EFFECT_SHIELD_REPAIR	= 0x04
	};

#define MAX_EQUIP_TECHNOLOGY_COUNT 13
	enum EQUIP_TECHNOLOGY : uint16_t
	{	EQUIP_TECHNOLOGY_NONE		= 0x0000
	,	EQUIP_TECHNOLOGY_BASIC		= 0x0001
	,	EQUIP_TECHNOLOGY_MECHANIC	= 0x0002
	,	EQUIP_TECHNOLOGY_DIGITAL	= 0x0004
	,	EQUIP_TECHNOLOGY_GENETIC	= 0x0008
	//,	EQUIP_TECHNOLOGY_CHEMICAL	= 0x0000
	//,	EQUIP_TECHNOLOGY_BALLISTIC	= 0x0000
	//,	EQUIP_TECHNOLOGY_LAUNCHER	= 0x0000
	//,	EQUIP_TECHNOLOGY_SOUND		= 0x0000
	//,	EQUIP_TECHNOLOGY_MICROWAVE	= 0x0000
	//,	EQUIP_TECHNOLOGY_RADIACTIVE	= 0x0000
	//,	EQUIP_TECHNOLOGY_RAY		= 0x0000
	//,	EQUIP_TECHNOLOGY_SATELLITE	= 0x0000
	,	EQUIP_TECHNOLOGY_MAGIC		= 0x0010
	};

#define MAX_EQUIP_TECHNOLOGY_COUNT 13
	enum EQUIP_MATERIAL : uint16_t
	{	EQUIP_MATERIAL_WOOD
	,	EQUIP_MATERIAL_METAL
	,	EQUIP_MATERIAL_BIO
	,	EQUIP_MATERIAL_CLOTH
	,	EQUIP_MATERIAL_POWER
	};


	std::string getStringFromBit(COMBAT_STATUS		bitStatus);
	std::string getStringFromBit(ATTACK_EFFECT		bitStatus);
	std::string getStringFromBit(DEFEND_EFFECT		bitStatus);
	std::string getStringFromBit(PASSIVE_EFFECT		bitStatus);
	std::string getStringFromBit(EQUIP_TECHNOLOGY	bitStatus);

#pragma pack(pop)
}

#endif // __COMBATSTATUS_H__2386498236498213469817263421__