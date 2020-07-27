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
	RegistQuests,
	HaveCharacters,
	HaveEquipments,
	HaveItems,
	SavePlayerInfo,
	AddNewCharacters,
	AddNewEquipments,
	AddNewItems,
	DestroyCharacters,
	DestroyEquipments,
	DestroyItems,
	UpdateCharacters,
	UpdateEquipments,
	SaveQuests,
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

	TArray<FInitEquipmentParams> NewEquipmentsInfos;

	TArray<FInitItemParams> NewItemsInfos;

	TArray<int32> DestoryCharacterIds;

	TArray<int32> DestoryEquipmentIds;

	TMap<int32, int32> DestoryItemKeys;

	TArray<int32> UpdateCharacterIds;

	TArray<int32> UpdateEquipmentIds;

	TArray<FSaveQuestParams> SaveQuestInfos;
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
	FOnHttpResponseReceived OnHttpRegistQuestsResponseReceived;
	FOnHttpResponseReceived OnHttpHaveCharactersResponseReceived;
	FOnHttpResponseReceived OnHttpHaveEquipmentsResponseReceived;
	FOnHttpResponseReceived OnHttpHaveItemsResponseReceived;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void HttpRequest(const FHttpRequestParams& InRequestParams);
	void HttpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void RequestLogin(const FString& InLoginId);
	void RequestRegist(const FString& InRegistId);
	void RequestRegistQuests(const FString& InRegistId);
	void RequestHaveCharacters(const FString& InUserId);
	void RequestHaveEquipments(const FString& InUserId);
	void RequestHaveItems(const FString& InUserId);
	void RequestSavePlayerInfo(const FString& InUserId, const ESavePlayerInfo& InColumnName, const FInitPlayerParams& Params);
	void RequestAddNewCharacters(const FString& InUserId, const TArray<FInitCharacterParams>& NewCharactersInfos);
	void RequestAddNewEquipments(const FString& InUserId, const TArray<FInitEquipmentParams>& NewEquipmentsInfos);
	void RequestAddNewItems(const FString& InUserId, const TArray<FInitItemParams>& NewItemsInfos);
	void RequestDestroyCharacters(const FString& InUserId, const TArray<int32>& DestroyCharacterIds);
	void RequestDestroyEquipments(const FString& InUserId, const TArray<int32>& DestroyEquipmentIds);
	void RequestDestroyItems(const FString& InUserId, const TMap<int32, int32>& DestroyItemKeys);
	void RequestUpdateCharacters(const FString& InUserId, const TArray<int32>& UpdateCharacterIds);
	void RequestUpdateEquipments(const FString& InUserId, const TArray<int32>& UpdateEquipmentIds);
	void RequestSaveQuests(const FString& InUserId, const TArray<FSaveQuestParams>& SaveQuestInfos);

	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnRegistResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnRegistQuestsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHaveCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHaveEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHaveItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnSavePlayerInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnAddNewCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnAddNewEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnAddNewItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnDestroyCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnDestroyEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnDestroyItemsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnUpdateCharactersResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnUpdateEquipmentsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnSaveQuestsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FHttpModule* Http;

	FString AuthToken;

	TQueue<FHttpRequestParams> Requests;

	bool bRequesting = false;
};
