// #include "E:\Unreal Projects\WebSocketAPI\Intermediate\Build\Win64\x64\WebSocketAPIEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"


#include "MyWebSocketManager.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "Engine/Engine.h"

// 
UMyWebSocketManager* UMyWebSocketManager::CreateWebSocketManager(UObject* BluePrintObject)
{
	return NewObject<UMyWebSocketManager>(BluePrintObject);
}

// 
void UMyWebSocketManager::SendMessage(const FString& Message) const
{
	if (!WebSocket.IsValid() || !WebSocket->IsConnected())
	{
		UE_LOG(LogTemp, Display, TEXT("WebSocket is NOT Connected"));
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("WebSocket is NOT Connected"));
		return;
	}
	
	WebSocket->Send(Message);
}

void UMyWebSocketManager::Connect(const FString URL)
{
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(URL); // "ws://localhost:8080" will use this for testing hard coded
	
	if (!WebSocket.IsValid() )
	{
		UE_LOG(LogTemp, Display, TEXT("WebSocket is NOT VALID - ERROR:CONNECT()"));
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("WebSocket is NOT VALID - ERROR:CONNECT()"));
		return;
	}
	
	WebSocket->OnConnected().AddLambda([this]()
	{
		UE_LOG(LogTemp, Display, TEXT("WebSocket is Connected"));
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, "Cpp - WebSocket is Connected");
	});

	WebSocket->OnConnectionError().AddLambda([this](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, "WebSocket is NOT Connected: " + Error);
		UE_LOG(LogTemp, Display, TEXT("WebSocket is NOT Connected"));
	});

	WebSocket->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean)
	{
		UE_LOG(LogTemp, Warning, TEXT("WebSocket is now closed because: %s"), *Reason);
		if (bAutoReconnect)
		{
			Reconnect(LastURL);
		}
	});

	WebSocket->OnMessage().AddLambda([this](const FString& Message)
	{
		UE_LOG(LogTemp, Display, TEXT("Cpp - Received Message: %s"), *Message);
		GEngine->AddOnScreenDebugMessage
		(-1, 15.f, FColor::Green, FString::Printf(TEXT("Cpp - Received Message: %s"), *Message));
		OnMessage.Broadcast(Message);
	});

	WebSocket->OnMessageSent().AddLambda([this](const FString& Message)
	{
		UE_LOG(LogTemp, Display, TEXT("Cpp - Received Message: %s"), *Message);
		GEngine->AddOnScreenDebugMessage
		(-1, 15.f, FColor::Green, FString::Printf(TEXT("Cpp - Sent Message: %s"), *Message));
		OnMessage.Broadcast(Message);
	});
	
	WebSocket->Connect();
	// UE_LOG(LogTemp, Warning, TEXT("Called WebSocket->Connect()"));
}

void UMyWebSocketManager::Close()
{
	if (WebSocket.IsValid() &&WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
	WebSocket.Reset();
}

bool UMyWebSocketManager::IsConnected()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("Cpp CHECK - WebSocket is Connected"));
		return true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("Cpp CHECK - WebSocket is NOT Connected"));
	return false;
}

void UMyWebSocketManager::Reconnect(FString URL)
{
	if(IsConnected())
	{
		Close();
	}
	
	Connect(URL);
}

void UMyWebSocketManager::SetAutoReconnect(bool AutoReconnect)
{
	bAutoReconnect = AutoReconnect;
}


