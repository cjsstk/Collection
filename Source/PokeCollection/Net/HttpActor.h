// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMSType.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Dom/JsonObject.h"
#include "Containers/Queue.h"
#include "HttpActor.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnHttpResponseReceived, FHttpRequestPtr, TSharedPtr<FJsonObject>, bool);

enum class EHttpRequestType
{
	Invalid,
	Login,
	Regist,
	HaveCharacters,
	HaveEquipments,
	SavePlayerInfo,
	AddNewCharacters,
};

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

struct FHttpRequestParams
{
public:
	EHttpRequestType RequestType = EHttpRequestType::Invalid;

	FString MemberID;

	ESavePlayerInfo SaveColumn;

	FInitPlayerParams SavePlayerInfos;

	TArray<FInitCharacterParams> NewCharactersInfos;
};

UCLASS()
class POKECOLLECTION_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHttpActor(const class FObjectInitializer& ObjectInitializer);

	bool IsRequesting() const { return bRequesting; }
	bool HasWaitingRequests() const { return !Requests.IsEmpty(); }

	void Request(const FHttpRequestParams& InRequestParams);

	FOnHttpResponseReceived OnHttpLoginResponseReceived;
	FOnHttpResponseReceived OnHttpRegistResponseReceived;
	FOnHttpResponseReceived OnHttpHaveCharactersResponseReceived;
	FOnHttpResponseReceived OnHttpHaveEquipmentsResponseReceived;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void HttpRequest(const FHttpRequestParams& InRequestParams);
	void HttpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void RequestLogin(const FString& InLoginId);
	void RequestRegist(const FString& InRegistId);
	void RequestHaveCharacters(const FString& InUserId);
	void RequestHaveEquipments(const FString& InUserId);
	void RequestSavePlayerInfo(const FString& InUserId, const ESavePlayerInfo& InColumnName, const FInitPlayerParams& Params);
	void RequestAddNewCharacters(const FString& InUserId, const TArray<FInitCharacterParams>& NewCharactersInfos);

	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnRegistResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHaveCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHaveEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnSavePlayerInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnAddNewCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FHttpModule* Http;

	FString AuthToken;

	TQueue<FHttpRequestParams> Requests;

	bool bRequesting = false;
};
