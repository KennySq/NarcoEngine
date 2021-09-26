#include"inc/Component.h"

namespace NARCO
{
	Component::Component(const char* name)
		: mName(name), mComponentID(MakeHash(mName))
	{
		
	}

}
