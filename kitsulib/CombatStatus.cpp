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
	case ATTACK_EFFECT_NONE				:	return "No attack effect"	;
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
	case ENTITY_TECHNOLOGY_BASIC		:	return "Basic"							;
	case ENTITY_TECHNOLOGY_MECHANIC		:	return "Mechanic"						;
	//case ENTITY_TECHNOLOGY_CHEMICAL	:	return "Chemical"						;
	//case ENTITY_TECHNOLOGY_BALLISTIC	:	return "Ballistic"						;
	//case ENTITY_TECHNOLOGY_LAUNCHER	:	return "Launcher"						;
	case ENTITY_TECHNOLOGY_DIGITAL		:	return "Digital"						;
	case ENTITY_TECHNOLOGY_GENETIC		:	return "Genetic"						;
	//case ENTITY_TECHNOLOGY_SOUND		:	return "Sound"							;
	//case ENTITY_TECHNOLOGY_MICROWAVE	:	return "Microwave"						;
	//case ENTITY_TECHNOLOGY_RADIACTIVE	:	return "Radiactive"						;
	//case ENTITY_TECHNOLOGY_RAY		:	return "Ray"							;
	//case ENTITY_TECHNOLOGY_SATELLITE	:	return "Satellite"						;
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
	static std::string stringTable[] =
	{	"Illusion"	
	,	"Light"	
	,	"Medium"	
	,	"Heavy"	
	};

	switch(bitStatus) {
	case ENTITY_GRADE_ILLUSION	:	return stringTable[0];
	case ENTITY_GRADE_LIGHT		:	return stringTable[1];
	case ENTITY_GRADE_MEDIUM	:	return stringTable[2];
	case ENTITY_GRADE_HEAVY		:	return stringTable[3];
	default:						
		return "Unknown grade"	;	
	};
}

std::string klib::getStringFromBit(ATTACK_TYPE			bitStatus)
{	
	static std::string stringTable[] =
	{	"No attack"
	,	"Melee"				
	,	"Ranged"				
	,	"Magic"				
	,	"Psychic"				
	,	"Satellite"			
	,	"Antipersonnel"		
	,	"Anti-tank"			
	,	"Anti-air"				
	,	"Anti-ship"		
	};
	switch(bitStatus) {
	case ATTACK_TYPE_NONE			: return stringTable[0];
	case ATTACK_TYPE_MELEE			: return stringTable[1];
	case ATTACK_TYPE_RANGED			: return stringTable[2];
	case ATTACK_TYPE_MAGIC			: return stringTable[3];
	case ATTACK_TYPE_PSYCHIC		: return stringTable[4];
	case ATTACK_TYPE_SATELLITE		: return stringTable[5];
	case ATTACK_TYPE_ANTIPERSONNEL	: return stringTable[6];
	case ATTACK_TYPE_ANTITANK		: return stringTable[7];
	case ATTACK_TYPE_ANTIAIR		: return stringTable[8];
	case ATTACK_TYPE_ANTISHIP		: return stringTable[9];
	default:						
		return "Unknown attack type"	;	
	};
}
