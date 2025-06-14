// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IWebSocket.h"
#include "MyWebSocketManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebSocketMessageEvent, const FString&, Message);

UCLASS(BlueprintType)
class MYWEBSOCKETAPI_API UMyWebSocketManager : public UObject
{
	GENERATED_BODY()

private:
	bool bIsConnected;
	bool bAutoReconnect = false;

public:

	TSharedPtr<IWebSocket> WebSocket;

	FString LastURL;


	UFUNCTION(BlueprintCallable, category = "WebSocketManager")
	static UMyWebSocketManager* CreateWebSocketManager(UObject* BluePrintObject);

	UFUNCTION(BlueprintCallable, Category= "WebSocketManager")
	void SendMessage(const FString& Message) const;

	// Connects the WebSocket to chosen URL
	UFUNCTION(BlueprintCallable, Category = "WebSocketManager")
	void Connect(const FString URL);

	// Closes the WebSocket connection if it's open
	UFUNCTION(BlueprintCallable, Category = "WebSocketManager")
	void Close();

	// Returns true if the WebSocket is currently connected
	UFUNCTION(BlueprintCallable, Category = "WebSocketManager")
	bool IsConnected();

	// Called when a message is received from the server,
	// is essentially an event dispatcher for when a message is called or sent using the delegate broadcast
	UPROPERTY(BlueprintAssignable, Category = "WebSocketManager")
	FWebSocketMessageEvent OnMessage;

	// Enables or disables automatic reconnection if the WebSocket connection drops
	UFUNCTION(BlueprintCallable, Category = "WebSocketManager")
	void SetAutoReconnect(bool AutoReconnect);

	UFUNCTION(BlueprintCallable, Category = "WebSocketManager")
	void Reconnect(FString URL);
	
	
};
