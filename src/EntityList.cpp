#include "EntityList.h"

#include "Entity.h"

EntityList::EntityList()
{
	for (int i = 0; i < NUM_ENTITIES; i++)
	{
		m_entityInfoPtrArray[i].m_entity = nullptr;
	}
}

Entity* EntityList::GetNextEntity(Entity* current)
{
	if (current)
	{
		return m_entityInfoPtrArray[current->m_index].m_next->m_entity;

	}

	return m_entityInfoPtrArray[0].m_entity;

	//return nullptr;
}

void EntityList::AddEntity(Entity* entity)
{
	m_entityList.push_back(entity);
	// notify any listeners of entity added

	// linked list add
	EntityInfo* entityInfo = new EntityInfo();
	entityInfo->m_entity = entity;
	entity->m_index = m_lastIndex;
	m_lastIndex++;
	entityInfo->m_next = m_head;
	entityInfo->m_prev = nullptr;

	if (m_head != nullptr)
	{
		m_head->m_prev = entityInfo;
	}

	m_head = entityInfo;
}

void EntityList::CleanUpDeleteList()
{

	for (Entity* entity : m_entityDeleteList)
	{
		m_entityList.erase(std::remove_if(m_entityList.begin(), m_entityList.end(),
			[entity](Entity* e) { return e == entity; }),
			m_entityList.end());
	}

	m_entityDeleteList.clear();
}

Entity* EntityList::FindEntityInRadius(Entity* startEntity, const glm::vec2& position, float radius)
{
	// Start from the given entity or the first entity in the list
	Entity* current = (startEntity != nullptr) ? GetNextEntity(startEntity) : GetNextEntity(nullptr);

	//while (current != nullptr) {
	//	if (IsEntityValid(current) && IsWithinRadius(current, position, radius)) {
	//		return current;
	//	}
	//	current = GetNextEntity(current);
	//}

	return nullptr;

	// USAGE
	//Entity* entity = NULL;
	//while ((entity = gEntList.FindEntityInRadius(entity, origin, radius)) != NULL)
	//{
	//	// do stuff
	//}
}
