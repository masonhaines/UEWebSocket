// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"


class FMyWebSocketAPIModule : public IModuleInterface
{
public:
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FMyWebSocketAPIModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FMyWebSocketAPIModule>("MyWebSocketAPI");
	}

	// maybe adding a reconnection logic and or message routing 
};
