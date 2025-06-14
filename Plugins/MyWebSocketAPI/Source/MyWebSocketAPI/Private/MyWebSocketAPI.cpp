// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyWebSocketAPI.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "FMyWebSocketAPIModule"

void FMyWebSocketAPIModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if(!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	// WebSocket = FWebSocketsModule::Get().CreateWebSocket(TEXT("ws://localhost:8080"));  // instantiate a new web socket for current platform
	//
	// WebSocket->OnConnected().AddLambda([]()
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("WebSocket is Connected"));
	// 	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("WebSocket is Connected"));
	// });
	//
	// WebSocket->Connect();
}

void FMyWebSocketAPIModule::ShutdownModule()
{
	// // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// // we call this function before unloading the module.
	//
	// if (WebSocket.IsValid() &&WebSocket->IsConnected())
	// {
	// 	WebSocket->Close();
	// }
	// WebSocket.Reset();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyWebSocketAPIModule, MyWebSocketAPI)