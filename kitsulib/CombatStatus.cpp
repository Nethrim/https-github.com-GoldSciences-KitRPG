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

std::string klib::getStringFromBit(EQUIP_TECHNOLOGY bitStatus)
{	
	switch(bitStatus) {
	case EQUIP_TECHNOLOGY_NONE			:	return "Undefined equipment technology"	;
	case EQUIP_TECHNOLOGY_BASIC			:	return "Basic"							;
	case EQUIP_TECHNOLOGY_MECHANIC		:	return "Mechanic"						;
	//case EQUIP_TECHNOLOGY_CHEMICAL		:	return "Chemical"						;
	//case EQUIP_TECHNOLOGY_BALLISTIC		:	return "Ballistic"						;
	//case EQUIP_TECHNOLOGY_LAUNCHER		:	return "Launcher"						;
	case EQUIP_TECHNOLOGY_DIGITAL		:	return "Digital"						;
	case EQUIP_TECHNOLOGY_GENETIC		:	return "Genetic"						;
	//case EQUIP_TECHNOLOGY_SOUND			:	return "Sound"							;
	//case EQUIP_TECHNOLOGY_MICROWAVE		:	return "Microwave"						;
	//case EQUIP_TECHNOLOGY_RADIACTIVE	:	return "Radiactive"						;
	//case EQUIP_TECHNOLOGY_RAY			:	return "Ray"							;
	//case EQUIP_TECHNOLOGY_SATELLITE		:	return "Satellite"						;
	default:						
		return "Unknown equipment technology"	;	
	};
}