#include "Common.h"
#include "iResourceManager.h"

#include <typeinfo>

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
