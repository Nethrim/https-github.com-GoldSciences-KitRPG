#include <cstdint>
#include <string>

#ifndef	__COMBATSTATUS_H__2386498236498213469817263421__
#define	__COMBATSTATUS_H__2386498236498213469817263421__


namespace klib
{
#pragma pack(push, 1)

#define MAX_COMBAT_STATUS_COUNT 32
	enum COMBAT_STATUS : int32_t
	{	COMBAT_STATUS_NONE				= 0x00000
	,	COMBAT_STATUS_BLIND				= 0x00001
	,	COMBAT_STATUS_STUN				= 0x00002
	,	COMBAT_STATUS_SHOCK				= 0x00004
	,	COMBAT_STATUS_BLEEDING			= 0x00008
	,	COMBAT_STATUS_BURN				= 0x00010
	,	COMBAT_STATUS_POISON			= 0x00020
	,	COMBAT_STATUS_FREEZING			= 0x00040
	,	COMBAT_STATUS_PETRIFY			= 0x00080
	,	COMBAT_STATUS_FROZEN			= 0x00100
	,	COMBAT_STATUS_BLACKOUT			= 0x00200
	,	COMBAT_STATUS_CHARMED			= 0x00400
	,	COMBAT_STATUS_DRUNK				= 0x00800
	,	COMBAT_STATUS_SLEEP				= 0x01000
	,	COMBAT_STATUS_SLOW				= 0x02000
	,	COMBAT_STATUS_WEAKNESS			= 0x04000
	,	COMBAT_STATUS_PANIC				= 0x08000
	,	COMBAT_STATUS_BERSERK			= 0x10000
	,	COMBAT_STATUS_INVISIBLE			= 0x20000
	};

#define MAX_ATTACK_EFFECT_COUNT 16
	enum ATTACK_EFFECT : uint16_t
	{	ATTACK_EFFECT_NONE				= 0x00
	,	ATTACK_EFFECT_LEECH_HEALTH		= 0x01
	,	ATTACK_EFFECT_LEECH_MANA		= 0x02
	,	ATTACK_EFFECT_LEECH_SHIELD		= 0x04
	,	ATTACK_EFFECT_STEAL				= 0x08
	};

#define MAX_DEFEND_EFFECT_COUNT 16
	enum DEFEND_EFFECT : uint16_t
	{	DEFEND_EFFECT_NONE				= 0x00
	,	DEFEND_EFFECT_REFLECT			= 0x01	// Reflects damage to the attacker
	,	DEFEND_EFFECT_BLIND				= 0x02	// Blinds the attacker when inflicting 1 to 1 damage.
	,	DEFEND_EFFECT_IMPENETRABLE		= 0x04	// Reflects damage to the attacker
	,	DEFEND_EFFECT_DEFLECT			= 0x08	// Reflects damage to the attacker
	};

#define MAX_PASSIVE_EFFECT_COUNT 16
	enum PASSIVE_EFFECT : uint16_t
	{	PASSIVE_EFFECT_NONE				= 0x00
	,	PASSIVE_EFFECT_LIFE_REGEN		= 0x01
	,	PASSIVE_EFFECT_MANA_REGEN		= 0x02
	,	PASSIVE_EFFECT_SHIELD_REPAIR	= 0x04
	};

#define MAX_ENTITY_TECHNOLOGY_COUNT 16
	enum ENTITY_TECHNOLOGY : uint16_t
	{	ENTITY_TECHNOLOGY_UNKNOWN		= 0x0000
	,	ENTITY_TECHNOLOGY_BASIC			= 0x0001
	,	ENTITY_TECHNOLOGY_MECHANIC		= 0x0002
	,	ENTITY_TECHNOLOGY_DIGITAL		= 0x0004
	,	ENTITY_TECHNOLOGY_GENETIC		= 0x0008
	//,	ENTITY_TECHNOLOGY_CHEMICAL		= 0x0000
	//,	ENTITY_TECHNOLOGY_BALLISTIC		= 0x0000
	//,	ENTITY_TECHNOLOGY_LAUNCHER		= 0x0000
	//,	ENTITY_TECHNOLOGY_SOUND			= 0x0000
	//,	ENTITY_TECHNOLOGY_MICROWAVE		= 0x0000
	//,	ENTITY_TECHNOLOGY_RADIACTIVE		= 0x0000
	//,	ENTITY_TECHNOLOGY_RAY			= 0x0000
	//,	ENTITY_TECHNOLOGY_SATELLITE		= 0x0000
	,	ENTITY_TECHNOLOGY_MAGIC			= 0x0010
	};

#define ENTITY_GRADE_COUNT 8
	enum ENTITY_GRADE : uint8_t
	{	ENTITY_GRADE_ILLUSION			= 0x00
	,	ENTITY_GRADE_LIGHT				= 0x01
	,	ENTITY_GRADE_MEDIUM				= 0x02
	,	ENTITY_GRADE_HEAVY				= 0x03
	};

#define MAX_ATTACK_TYPE_COUNT 16
	enum ATTACK_TYPE : uint16_t
	{	ATTACK_TYPE_NONE				= 0x000
	,	ATTACK_TYPE_MELEE				= 0x001
	,	ATTACK_TYPE_RANGED				= 0x002
	,	ATTACK_TYPE_TELEKINETIC			= 0x004
	,	ATTACK_TYPE_MAGIC				= 0x008
	,	ATTACK_TYPE_PSYCHIC				= 0x010
	,	ATTACK_TYPE_SATELLITE			= 0x020
	,	ATTACK_TYPE_ANTIPERSONNEL		= 0x040
	,	ATTACK_TYPE_ANTITANK			= 0x080
	,	ATTACK_TYPE_ANTIAIR				= 0x100
	,	ATTACK_TYPE_ANTISHIP			= 0x200
	,	ATTACK_TYPE_NUKE				= 0x400
	};

#define MAX_EQUIP_MATERIAL_COUNT 16
	enum EQUIP_MATERIAL : uint16_t
	{	EQUIP_MATERIAL_UNKNOWN			= 0x000
	,	EQUIP_MATERIAL_WOOD				= 0x001
	,	EQUIP_MATERIAL_METAL			= 0x002
	,	EQUIP_MATERIAL_BIO				= 0x004
	,	EQUIP_MATERIAL_CLOTH			= 0x008
	,	EQUIP_MATERIAL_POWER			= 0x010
	};

	std::string getStringFromBit(COMBAT_STATUS		bitStatus);
	std::string getStringFromBit(ATTACK_EFFECT		bitStatus);
	std::string getStringFromBit(DEFEND_EFFECT		bitStatus);
	std::string getStringFromBit(PASSIVE_EFFECT		bitStatus);
	std::string getStringFromBit(ENTITY_TECHNOLOGY	bitStatus);
	std::string getStringFromBit(ENTITY_GRADE		bitStatus);
	std::string getStringFromBit(ATTACK_TYPE		bitStatus);
	std::string getStringFromBit(EQUIP_MATERIAL		bitStatus);

	struct SEntityEffect
	{
		ATTACK_EFFECT	Attack;
		DEFEND_EFFECT	Defend;
		PASSIVE_EFFECT	Passive;

		inline constexpr SEntityEffect operator | (const SEntityEffect& other) const {
			return{ (ATTACK_EFFECT)(Attack | other.Attack), (DEFEND_EFFECT)(Defend | other.Defend), (PASSIVE_EFFECT)(Passive | other.Passive) };
		};

		void Print() const
		{
			printf("- Flags for Attack Effect ...: 0x%.08x.\n"	, (int32_t)	Attack	);
			printf("- Flags for Defend Effect ---: 0x%.08x.\n"	, (int32_t)	Defend	);
			printf("- Flags for Passive Effect ..: 0x%.08x.\n"	, (int32_t)	Passive	);
		}
	};

	struct SEntityStatus
	{
		COMBAT_STATUS		Inflict;
		COMBAT_STATUS		Immunity;

		inline constexpr SEntityStatus operator | (const SEntityStatus& other) const {
			return{ (COMBAT_STATUS)(Inflict | other.Inflict), (COMBAT_STATUS)(Immunity | other.Immunity) };
		};

		void Print() const
		{
			printf("- Flags for Status Inflict ..: 0x%.08x.\n"	, (int32_t)	Inflict		);
			printf("- Flags for Status Immunity -: 0x%.08x.\n"	, (int32_t)	Immunity	);
		}										
	};

	struct SEntityGrade
	{
		ENTITY_TECHNOLOGY	Tech;
		ENTITY_GRADE		Grade;

		inline constexpr SEntityGrade operator | (const SEntityGrade& other) const {
			return{ (ENTITY_TECHNOLOGY)(Tech | other.Tech), (Grade > other.Grade) ? Grade : other.Grade};
		}

		void Print() const
		{
			printf("- Flags for Tech ............: 0x%.08x.\n"	, (int32_t)	Tech	);
			printf("- Grade ---------------------: 0x%.08x.\n"	, (int32_t)	Grade	);
		}
	};

	struct SEntityEffectTurns
	{
		int32_t			Attack;
		int32_t			Defend;
		int32_t			Passive;
	};

	struct SEntityStatusTurns
	{
		int32_t			Inflict;
		int32_t			Immunity;
	};

	struct SEntityTechTurns
	{
		int32_t			Tech;
		int32_t			Grade;
	};

	struct SEntityFlagsTurns
	{
		SEntityEffectTurns	Effect;
		SEntityStatusTurns	Status;
		SEntityTechTurns	Tech;
	};

#pragma pack(pop)
}

#endif // __COMBATSTATUS_H__2386498236498213469817263421__