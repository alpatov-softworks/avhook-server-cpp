//
// Created by nullifiedvlad on 27.12.2022.
//

#include "OnUserDisconnected.h"
#include <cstdio>

namespace Observers
{
	void OnUserDisconnected::HandleEvent(const ObservableObject *pContext)
	{
		printf("[LOG] User Disconnected\n");
	}
} // Obsevers