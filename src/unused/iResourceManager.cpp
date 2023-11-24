#include "Common.h"
#include "iResourceManager.h"

#include <typeinfo>

iResource* iResourceManager::GetResource(const std::string& id)
{
	auto it = m_resourceMap.find(id);
	ASSERT_MSG(it != m_resourceMap.end(), "Resource with id \"%s\" doesn't exist", id.c_str());

	return it == m_resourceMap.end() ? nullptr : it->second;
}

void iResourceManager::UnloadResources()
{
	printf("%s Unloading resources\n", typeid(this).name());
	printf("--------------------------------------------------------\n");
	for (auto& it : m_resourceMap)
	{
		SAFE_DELETE(it.second);
		printf("Unloaded resource: %s\n", it.first.c_str());
	}
	printf("--------------------------------------------------------\n\n");
}
