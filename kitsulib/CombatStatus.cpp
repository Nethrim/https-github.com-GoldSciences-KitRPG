#include "CombatStatus.h"

std::string klib::getStringFromBit(COMBAT_STATUS bitStatus)
{
	switch(bitStatus) {
	case COMBAT_STATUS_NONE				:	return "No combat status"	; 
	case COMBAT_STATUS_BLIND			:	return "Blind"				; 
	case COMBAT_STATUS_STUN				:	return "Stun"				; 
	case COMBAT_STATUS_SHOCK			:	return "Shock"				; 
	case COMBAT_STATUS_BLEEDING			:	return "Bleeding"			; 
	case COMBAT_STATUS_BURN				:	return "Burn"				; 
	case COMBAT_STATUS_POISON			:	return "Poison"				; 
	case COMBAT_STATUS_FREEZING			:	return "Freezing"			; 
	case COMBAT_STATUS_PETRIFY			:	return "Petrify"			; 
	case COMBAT_STATUS_FROZEN			:	return "Frozen"				; 
	case COMBAT_STATUS_BLACKOUT			:	return "Blackout"			; 
	case COMBAT_STATUS_CHARMED			:	return "Charmed	"			; 
	case COMBAT_STATUS_DRUNK			:	return "Drunk"				; 
	case COMBAT_STATUS_SLEEP			:	return "Sleep"				; 
	case COMBAT_STATUS_SLOW				:	return "Slow"				; 
	case COMBAT_STATUS_WEAKNESS			:	return "Weakness"			; 
	case COMBAT_STATUS_BERSERK			:	return "Berserk"			; 
	default:						
		return "Unknown combat status"	;	
	}
}	

std::string klib::getStringFromBit(ATTACK_EFFECT bitStatus)
{
	switch(bitStatus) {
	case ATTACK_EFFECT_NONE				:	return "No defend effect"	;
	case ATTACK_EFFECT_LEECH_HEALTH		:	return "Health Drain"		;
	case ATTACK_EFFECT_LEECH_MANA		:	return "Mana Drain"			;
	case ATTACK_EFFECT_LEECH_SHIELD		:	return "Shield Steal"		;
	case ATTACK_EFFECT_STEAL			:	return "Steal"				;
	default:						
		return "Unknown attack effect"	;	
	};
}	

std::string klib::getStringFromBit(DEFEND_EFFECT bitStatus)
{
	switch(bitStatus) {
	case DEFEND_EFFECT_NONE				: return "No defend effect"	;
	case DEFEND_EFFECT_REFLECT			: return "Reflect"			;
	case DEFEND_EFFECT_BLIND			: return "Blind"			;
	case DEFEND_EFFECT_IMPENETRABLE		: return "Impenetrable"		;
	case DEFEND_EFFECT_DEFLECT			: return "Deflect"			;
	default:						
		return "Unknown defend effect"	;	
	};
}

std::string klib::getStringFromBit(PASSIVE_EFFECT bitStatus)
{
	switch(bitStatus) {
	case PASSIVE_EFFECT_NONE			: return "No passive effect"	;
	case PASSIVE_EFFECT_LIFE_REGEN		: return "Life regeneration"	;
	case PASSIVE_EFFECT_MANA_REGEN		: return "Mana regeneration"	;
	case PASSIVE_EFFECT_SHIELD_REPAIR	: return "Shield repair"		;
	default:						
		return "Unknown passive effect"	;	
	};
}

std::string klib::getStringFromBit(ENTITY_TECHNOLOGY bitStatus)
{	
	switch(bitStatus) {
	case ENTITY_TECHNOLOGY_UNKNOWN		:	return "Undefined equipment technology"	;
	case ENTITY_TECHNOLOGY_BASIC			:	return "Basic"							;
	case ENTITY_TECHNOLOGY_MECHANIC		:	return "Mechanic"						;
	//case ENTITY_TECHNOLOGY_CHEMICAL		:	return "Chemical"						;
	//case ENTITY_TECHNOLOGY_BALLISTIC		:	return "Ballistic"						;
	//case ENTITY_TECHNOLOGY_LAUNCHER		:	return "Launcher"						;
	case ENTITY_TECHNOLOGY_DIGITAL		:	return "Digital"						;
	case ENTITY_TECHNOLOGY_GENETIC		:	return "Genetic"						;
	//case ENTITY_TECHNOLOGY_SOUND			:	return "Sound"							;
	//case ENTITY_TECHNOLOGY_MICROWAVE		:	return "Microwave"						;
	//case ENTITY_TECHNOLOGY_RADIACTIVE	:	return "Radiactive"						;
	//case ENTITY_TECHNOLOGY_RAY			:	return "Ray"							;
	//case ENTITY_TECHNOLOGY_SATELLITE		:	return "Satellite"						;
	default:						
		return "Unknown equipment technology"	;	
	};
}

std::string klib::getStringFromBit(EQUIP_MATERIAL		bitStatus)
{	
	switch(bitStatus) {
	case EQUIP_MATERIAL_UNKNOWN	:return "Unknown material"	;
	case EQUIP_MATERIAL_WOOD	:return "Wood"	;
	case EQUIP_MATERIAL_METAL	:return "Metal"	;
	case EQUIP_MATERIAL_BIO		:return "Bio"	;
	case EQUIP_MATERIAL_CLOTH	:return "Cloth"	;
	case EQUIP_MATERIAL_POWER	:return "Power"	;
	default:						
		return "Unknown equipment material"	;	
	};
}

std::string klib::getStringFromBit(ENTITY_GRADE				bitStatus)
{	
	switch(bitStatus) {
	case ENTITY_GRADE_ILLUSION		:	return "Illusion"	;
	case ENTITY_GRADE_LIGHT		:	return "Light	"	;
	case ENTITY_GRADE_MEDIUM		:	return "Medium	"	;
	case ENTITY_GRADE_HEAVY		:	return "Heavy	"	;
	default:						
		return "Unknown grade"	;	
	};
}

std::string klib::getStringFromBit(ATTACK_TYPE			bitStatus)
{	
	switch(bitStatus) {
	case ATTACK_TYPE_NONE			: return "No attack type"	;
	case ATTACK_TYPE_MELEE			: return "Melee"			;
	case ATTACK_TYPE_RANGED			: return "Ranged"			;
	case ATTACK_TYPE_MAGIC			: return "Magic"			;
	case ATTACK_TYPE_PSYCHIC		: return "Psychic"			;
	case ATTACK_TYPE_SATELLITE		: return "Satellite"		;
	case ATTACK_TYPE_ANTI_PERSONNEL	: return "Antipersonnel"	;
	case ATTACK_TYPE_ANTI_TANK		: return "Anti-tank"		;
	case ATTACK_TYPE_ANTI_AIR		: return "Anti-air"			;
	case ATTACK_TYPE_ANTI_SHIP		: return "Anti-ship"		;
	default:						
		return "Unknown attack type"	;	
	};
}
