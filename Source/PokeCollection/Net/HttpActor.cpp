﻿// Copyright Me. All Rights Reserved.


#include "HttpActor.h"

#include "Engine.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

const static FString CollectionURL = FString("http://collection.nupa.moe/api/");

using VerbType = FString;
namespace HTTPVerb
{
	const static VerbType GET = FString("GET");
	const static VerbType POST = FString("POST");
	const static VerbType PUT = FString("PUT");
	const static VerbType DELETE = FString("DELETE");
}

AHttpActor::AHttpActor(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Http = &FHttpModule::Get();

	FString LoadedToken;
	FString FileName = FPaths::ProjectConfigDir() + "/Token.txt";
	if (FFileHelper::LoadFileToString(LoadedToken, *FileName))
	{
		AuthToken = LoadedToken;
	}
	else
	{
		ensure(0);
	}
}

void AHttpActor::RequestLogin(const FString& InLoginId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnLoginResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InLoginId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);

	Request->ProcessRequest();
}

void AHttpActor::RequestRegist(const FString& InRegistId)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField(TEXT("id"), *FString::Printf(TEXT("%s"), *InRegistId));
	JsonObject->SetStringField(TEXT("nickname"), *FString(TEXT("Temp")));
	JsonObject->SetNumberField(TEXT("level"), 1);
	JsonObject->SetNumberField(TEXT("exp"), 0);
	JsonObject->SetNumberField(TEXT("money"), 0);
	JsonObject->SetNumberField(TEXT("berry"), 0);
	JsonObject->SetNumberField(TEXT("stardust"), 0);
	JsonObject->SetNumberField(TEXT("maxClearBattleStage"), 0);
	JsonObject->SetNumberField(TEXT("lastSelectedChapter"), 1);
	JsonObject->SetNumberField(TEXT("maxOpenedChapter"), 1);
	JsonObject->SetNumberField(TEXT("lastSelectedParty"), 1);
	JsonObject->SetNumberField(TEXT("battleClearCount"), 0);
	JsonObject->SetNumberField(TEXT("getCharacterCount"), 0);
	JsonObject->SetNumberField(TEXT("maxHaveCharactersNum"), 100);
	JsonObject->SetNumberField(TEXT("maxHaveEquipmentsNum"), 100);
	JsonObject->SetStringField(TEXT("index"), *FString(TEXT("")));
	JsonObject->SetNumberField(TEXT("battleSpeed"), 1);

	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnRegistResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InRegistId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::POST);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);

	Request->ProcessRequest();
}

void AHttpActor::RequestHaveCharacters(const FString& InUserId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnHaveCharactersResponseReceived);

	FString RequestURL = CollectionURL + FString("haveCharacters/") + InUserId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);

	Request->ProcessRequest();
}

void AHttpActor::RequestHaveEquipments(const FString& InUserId)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnHaveEquipmentsResponseReceived);

	FString RequestURL = CollectionURL + FString("haveEquipments/") + InUserId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::GET);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);

	Request->ProcessRequest();
}

void AHttpActor::RequestSavePlayerInfo(const FString& InUserId, ESavePlayerInfo& InColumnName, const FInitPlayerParams& Params)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField(TEXT("id"), *FString::Printf(TEXT("%s"), *InUserId));

	switch (InColumnName)
	{
	case ESavePlayerInfo::Nickname:
		JsonObject->SetStringField(TEXT("nickname"), *Params.PlayerNickname);
		break;
	case ESavePlayerInfo::Level:
		JsonObject->SetNumberField(TEXT("level"), Params.PlayerLevel);
		break;
	case ESavePlayerInfo::Exp:
		JsonObject->SetNumberField(TEXT("exp"), Params.PlayerCurrentExp);
		break;
	case ESavePlayerInfo::Money:
		JsonObject->SetNumberField(TEXT("money"), Params.PlayerMoney);
		break;
	case ESavePlayerInfo::Berry:
		JsonObject->SetNumberField(TEXT("berry"), Params.PlayerBerry);
		break;
	case ESavePlayerInfo::Stardust:
		JsonObject->SetNumberField(TEXT("stardust"), Params.PlayerStardust);
		break;
	case ESavePlayerInfo::MaxClearBattleStage:
		JsonObject->SetNumberField(TEXT("maxClearBattleStage"), Params.MaxClearBattleStage);
		break;
	case ESavePlayerInfo::LastSelectedChapter:
		JsonObject->SetNumberField(TEXT("lastSelectedChapter"), Params.LastSelectedChapter);
		break;
	case ESavePlayerInfo::MaxOpenedChapter:
		JsonObject->SetNumberField(TEXT("maxOpenedChapter"), Params.MaxOpenedChapter);
		break;
	case ESavePlayerInfo::LastSelectedParty:
		JsonObject->SetNumberField(TEXT("lastSelectedParty"), Params.LastSelectedParty);
		break;
	case ESavePlayerInfo::BattleClearCount:
		JsonObject->SetNumberField(TEXT("battleClearCount"), Params.BattleClearCount);
		break;
	case ESavePlayerInfo::GetCharacterCount:
		JsonObject->SetNumberField(TEXT("getCharacterCount"), Params.GetCharacterCount);
		break;
	case ESavePlayerInfo::MaxHaveCharactersNum:
		JsonObject->SetNumberField(TEXT("maxHaveCharactersNum"), Params.MaxHaveCharactersNum);
		break;
	case ESavePlayerInfo::MaxHaveEquipmentsNum:
		JsonObject->SetNumberField(TEXT("maxHaveEquipmentsNum"), Params.MaxHaveEquipmentsNum);
		break;
	case ESavePlayerInfo::Index:
		JsonObject->SetStringField(TEXT("index"), *Params.Index);
		break;
	case ESavePlayerInfo::BattleSpeed:
		JsonObject->SetNumberField(TEXT("battleSpeed"), Params.BattleSpeed);
		break;
	default:
		break;
	}
	
	FString OutputString;

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnSavePlayerInfoResponseReceived);

	FString RequestURL = CollectionURL + FString("members/") + InUserId;
	Request->SetURL(RequestURL);
	Request->SetVerb(HTTPVerb::PUT);
	Request->SetContentAsString(OutputString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", AuthToken);

	Request->ProcessRequest();
}

void AHttpActor::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		////Get the value of the json object by field name
		//int32 recievedInt = JsonObject->GetIntegerField("id");

		////Output it to the engine
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}

	OnHttpLoginResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
}

void AHttpActor::OnRegistResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnHttpRegistResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}

	OnHttpRegistResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
}

void AHttpActor::OnHaveCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnHttpHaveCharactersResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnHaveEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		OnHttpHaveEquipmentsResponseReceived.ExecuteIfBound(Request, JsonObject, bWasSuccessful);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::OnSavePlayerInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString(TEXT("유저 데이터 저장")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, Reader.Get().GetErrorMessage());
	}
}

void AHttpActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

