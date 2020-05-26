// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMSType.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Dom/JsonObject.h"
#include "HttpActor.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnHttpResponseReceived, FHttpRequestPtr, TSharedPtr<FJsonObject>, bool);

enum class ESavePlayerInfo
{
	Nickname,
	Level,
	Exp,
	Money,
	Berry,
	Stardust,
	MaxClearBattleStage,
	LastSelectedChapter,
	MaxOpenedChapter,
	LastSelectedParty,
	BattleClearCount,
	GetCharacterCount,
	MaxHaveCharactersNum,
	MaxHaveEquipmentsNum,
	Index,
	BattleSpeed
};


UCLASS()
class POKECOLLECTION_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHttpActor(const class FObjectInitializer& ObjectInitializer);

	void RequestLogin(const FString& InLoginId);
	void RequestRegist(const FString& InRegistId);
	void RequestHaveCharacters(const FString& InUserId);
	void RequestHaveEquipments(const FString& InUserId);
	void RequestSavePlayerInfo(const FString& InUserId, ESavePlayerInfo& InColumnName, const FInitPlayerParams& Params);

	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnRegistResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHaveCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHaveEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnSavePlayerInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FOnHttpResponseReceived OnHttpLoginResponseReceived;
	FOnHttpResponseReceived OnHttpRegistResponseReceived;
	FOnHttpResponseReceived OnHttpHaveCharactersResponseReceived;
	FOnHttpResponseReceived OnHttpHaveEquipmentsResponseReceived;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FHttpModule* Http;

	FString AuthToken;
};
