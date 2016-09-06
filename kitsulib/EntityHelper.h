#include "Entity.h"

#ifndef __ENTITYHELPER_H__98234092340283749023874920834234__
#define __ENTITYHELPER_H__98234092340283749023874920834234__

namespace klib
{
#pragma pack(push, 1)
	template<typename _EntityType>
	struct SResearchGroup
	{
		_EntityType						MaxResearch;
		SEntityContainer<int16_t, 256>	Definitions;
		SEntityContainer<int16_t, 256>	Modifiers;
	};


	// Combines two record tables to get the names and combine them as one for display purposes.
	template<typename _EntityType, size_t _DefinitionCount, size_t _ModifierCount> 
	std::string getEntityName(const _EntityType& entity
		, const SEntityRecord<_EntityType>(&tableDefinitions)[_DefinitionCount]
		, const SEntityRecord<_EntityType>(&tableModifiers	)[_ModifierCount]
	)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, tableModifiers[entity.Modifier].Name.c_str(), tableDefinitions[entity.Definition].Name.c_str());
		return formattedName;
	}

	// removes an entity from a container and sets it to zero
	template<typename _EntityType, size_t _DefinitionCount>
	void unequipEntity(SEntityContainer<_EntityType, _DefinitionCount>& container, _EntityType& currentEntity) 
	{	
		if( 0 == currentEntity.Definition && 0 == currentEntity.Modifier && 0 == currentEntity.Level )
			return; 
		container.AddElement(currentEntity);	
		currentEntity = {0,0,0};
	};

	// Looks into a research container for the requested technology and equips the entity in equippedEntity if the research is complete.
	template<typename _EntityType, size_t _EntityContainerSize, size_t _DefinitionCount, size_t _ModifierCount>
	bool equipEntityIfResearched			
		( size_t slotIndex
		, SEntityContainer<_EntityType, _EntityContainerSize>& entityContainer
		, const SResearchGroup<_EntityType>& completedResearch 
		, const SEntityRecord<_EntityType>(&tableDefinitions)[_DefinitionCount]
		, const SEntityRecord<_EntityType>(&tableModifiers	)[_ModifierCount]
		, _EntityType& equippedEntity
		, const std::string& cantAccessDefinitionError
		, const std::string& cantAccessModifierError
		, const std::string& storeOldWeaponMessage
		, const std::string& equipNewWeaponMessage
		, const std::string& researchTypeString
		)	
	{	
		if(slotIndex >= entityContainer.Count) //invalid index do nothing.
			return false;
	
		bool bCancel = false; 
		if( 0 < entityContainer.Slots[slotIndex].Entity.Definition	&& (-1) == completedResearch.Definitions.FindElement(entityContainer.Slots[slotIndex].Entity.Definition)) { 
			bCancel = true;
			printf(cantAccessDefinitionError.c_str(), tableDefinitions[entityContainer.Slots[slotIndex].Entity.Definition].Name.c_str());	//
		} 

		if( 0 < entityContainer.Slots[slotIndex].Entity.Modifier	&& (-1) == completedResearch.Modifiers.FindElement(entityContainer.Slots[slotIndex].Entity.Modifier)) { 
			bCancel = true;
			static char itemText[128] = {}; 
			sprintf_s(itemText, tableModifiers[entityContainer.Slots[slotIndex].Entity.Modifier].Name.c_str(), researchTypeString.c_str());
			printf(cantAccessModifierError.c_str(), itemText);
		}	

		if(bCancel)
			return false;

		printf(storeOldWeaponMessage.c_str(), getEntityName(equippedEntity, tableDefinitions, tableModifiers).c_str(), equippedEntity.Level); 
		unequipEntity(entityContainer, equippedEntity);
		equippedEntity = entityContainer.Slots[slotIndex].Entity;	// Assign selected entity.
		entityContainer.DecreaseEntity(slotIndex);
		printf(equipNewWeaponMessage.c_str(), getEntityName(equippedEntity, tableDefinitions, tableModifiers).c_str(), equippedEntity.Level); 
		return true;
	};

	template<typename _EntityType, size_t _definitionCount, size_t _modifierCount>
	static SEntityPoints getEntityPoints(const _EntityType& entity,const SEntityRecord<_EntityType> (&definitions)[_definitionCount], const SEntityRecord<_EntityType> (&modifiers)[_modifierCount]) {
		return (definitions[entity.Definition].Points + modifiers[entity.Modifier].Points)*(entity.getMultipliers()*entity.Level);
	}

	template<typename _EntityType, size_t _definitionCount, size_t _modifierCount>
	static SEntityFlags getEntityFlags(const _EntityType& entity,const SEntityRecord<_EntityType> (&definitions)[_definitionCount], const SEntityRecord<_EntityType> (&modifiers)[_modifierCount]) {
		return (definitions[entity.Definition].Flags | modifiers[entity.Modifier].Flags);
	}

#pragma pack(pop)
}

#endif __ENTITYHELPER_H__98234092340283749023874920834234__
