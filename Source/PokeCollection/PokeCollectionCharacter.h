// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CMSType.h"
#include "Net/HttpActor.h"
#include "PokeCollectionCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddedNewCharacter);


enum class EPlayerMode
{
	BattleMode,
	MakePartyMode,
	UIMode,
};


/**
 * 플레이어(유저) 클래스
 */
UCLASS(config=Game)
class APokeCollectionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APokeCollectionCharacter();

	void SetPlayerMode(EPlayerMode NewPlayerMode);

	void AddNewCharacters(TArray<FInitCharacterParams>& InInitCharacterParams);
	void AddNewEquipments(TArray<FInitEquipmentParams>& InInitEquipmentParams);
	void AddNewItems(TArray<FInitItemParams>& InInitItemParams);

	void DeleteCharacters(TArray<int32>& InCharacterIDs);
	void DeleteEquipments(TArray<int32>& InEquipmentIDs);
	//void DeleteItemsByID(TMap<int32, int32>& InItemIDs);
	void DeleteItemsByKey(TMap<int32, int32>& InItemKeys);

	void UpdateCharacters(TArray<int32>& InCharacterIDs);
	void UpdateEquipments(TArray<int32>& InEquipmentIDs);

	void GetReward(FBattleReward& InBattleReward);

	void PutOnEquipment(int32 InCharacterID, int32 InEquipmentID);
	void TakeOffEquipment(int32 InCharacterID);

	bool SaveQuests();

	/** 
	 * Player Info
	 */
	void SetPlayerNickName(FString& InNickname);
	FName GetPlayerNickName() const { return PlayerNickName; }
	
	int32 GetPlayerLevel() const { return PlayerLevel; }
	void SetPlayerLevel(int32 NewLevel);

	int32 GetPlayerCurrentExp() const { return PlayerCurrentExp; }
	int32 GetPlayerMaxExp() const { return PlayerMaxExp; }

	void SetCurrentSelectedBattleStageKey(battleStageKey InBattleStageKey);
	battleStageKey GetCurrentSelectedBattleStageKey() const { return CurrentSelectedBattleStageKey; }

	void SetCurrentSelectedChapterNum(int32 NewSelectedChapterNum);
	int32 GetCurrentSelectedChapterNum() const { return CurrentSelectedChapterNum; }

	void SetMaxOpenedChapterNum(int32 NewMaxOpenedChapterNum);

	void SetMaxClearBattleStage(battleStageKey InBattleStageKey);

	void SetCurrentSelectedPartyNum(int32 NewSelectedPartyNum);
	int32 GetCurrentSelectedPartyNum() const { return CurrentSelectedParty; }

	void SetMaxHaveCharactersNum(int32 NewMaxHaveCharactersNum);
	int32 GetMaxHaveCharactersNum() const { return MaxHaveCharactersNum; }

	void SetMaxHaveEquipmentsNum(int32 NewMaxHaveEquipmentsNum);
	int32 GetMaxHaveEquipmentsNum() const { return MaxHaveEquipmentsNum; }

	void SetMainCharacterID(int32 NewMainCharacterID);
	int32 GetMainCharacterID() const { return MainCharacterID; }

	bool IsCompleteIndexCharacter(characterKey InCharacterKey);
	bool IsClearBattleStage(int32 InBattleStageKey);

	int32 GetBattleClearCount() { return BattleClearCount; }
	int32 GetTotalCharacterCount() { return GetCharacterCount; }

	void ChangeBattleSpeedMultiplier();
	int32 GetBattleSpeedMultiplier() const { return BattleSpeedMultiplier; }

	const TArray<class APokeCharacter*>& GetHaveCharacters() const;
	/** Key: SlotNum, Value: Character */
	const TMap<int32, class APokeCharacter*> GetPartyCharacters(int32 InPartyNum) const;
	class APokeCharacter* GetCharacterByID(int32 InCharacterID) const;
	class APokeCharacter* GetCharacterBySlotNum(int32 InPartyNum, int32 InSlotNum) const;

	const TArray<class UPokeEquipment*>& GetHaveEquipments() const;
	class UPokeEquipment* GetEquipmentByID(int32 InEquipmentID) const;

	const TArray<class UPokeItem*>& GetHaveItems() const;
	class UPokeItem* GetItemByKey(int32 InItemKey);
	class UPokeItem* GetItemByID(int32 InItemID);

	int32 GetBerryAmount() const { return BerryAmount; }
	int32 GetMoneyAmount() const { return MoneyAmount; }
	int32 GetStardustAmount() const { return StardustAmount; }
	int32 GetBerryChargingAmount() const { return BerryChargingAmount; }
	int32 GetMoneyChargingAmount() const { return MoneyChargingAmount; }
	int32 GetBerryChargingIntervalMinutes() const { return BerryChargingIntervalSeconds / 60; }
	int32 GetMoneyChargingIntervalMinutes() const { return MoneyChargingIntervalSeconds / 60; }

	void ConsumeBerry(int32 InConsumeBerryAmount);
	void AddMoney(int32 InAddAmout);
	void SpendMoney(int32 InSpendAmout);

	class UPlayerQuestComponent* GetQuestComponent() const { return QuestComponent; }

	FOnAddedNewCharacter OnAddedNewCharacter;

	TArray<int32> SeletedDestroyCharacterIDs;
	TArray<int32> SeletedDestroyEquipmentIDs;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool AddNewCharacter(FInitCharacterParams& InInitCharacterParams);
	bool AddNewEquipment(FInitEquipmentParams& InInitEquipmentParams);
	bool AddNewItem(FInitItemParams& InInitItemParams);

	void InitHaveCharacters();
	void InitHaveEquipments();
	void InitHaveItems();
	void InitHaveQuests();

	void InitPlayerInfo(FInitPlayerParams& Params);
	void InitMainCharacter();

	void SetPlayerExp(int32 InExp);
	void SetPlayerMaxExp(int32 InMaxExp);
	void SetBerryAmount(int32 NewBerryAmount);
	void SetStardustAmount(int32 NewStardustAmount);
	void SetBattleClearCount(int32 NewBattleClearCount);
	void SetGetCharacterCount(int32 NewGetCharacterCount);
	void SetIndex(FString& NewIndex);
	void SetMoneyAmount(int32 NewMoneyAmount);

	FString GetIndex();

	void TickResourceCharge(float DeltaSeconds);
	void AddCharacterToIndex(characterKey InCharacterKey);
	
	int32 GetUsableCharacterID();
	int32 GetUsableEquipmentID();
	int32 GetUsableItemID();

	/** 
	 * Http
	 */
	void OnLoginResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);
	void OnHaveCharactersResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);
	void OnHaveEquipmentsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);
	void OnHaveItemsResponsed(FHttpRequestPtr Request, TSharedPtr<FJsonObject> ResponceJson, bool bWasSuccessful);

	void SavePlayerInfo(ESavePlayerInfo InSaveInfo);

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent = nullptr;

	UPROPERTY()
	class UPlayerQuestComponent* QuestComponent = nullptr;

	/** 
	 * Player Info
	 */
	UPROPERTY()
	FName PlayerNickName = FName("Moong");
	int32 PlayerLevel = 1;
	int32 PlayerCurrentExp = 0;
	int32 PlayerMaxExp = 1000;

	battleStageKey CurrentSelectedBattleStageKey = 0;
	int32 MaxClearBattleStageNum = 0;

	int32 CurrentSelectedChapterNum = 1;
	int32 MaxOpenedChapterNum = 2;

	int32 CurrentSelectedParty = 1;	// 선택한 파티 번호

	int32 BattleClearCount = 0;		// 총 배틀 클리어 횟수
	int32 GetCharacterCount = 0;	// 총 얻은 캐릭터 수

	int32 BattleSpeedMultiplier = 1;

	/**
	 * Characters
	 */
	int32 MaxHaveCharactersNum = 100;

	UPROPERTY(Transient)
	TArray<class APokeCharacter*> HaveCharacters;

	int32 MainCharacterID = -1;		// 메인캐릭터 아이디

	TMap<int32, bool> CharacterIndex;

	/** 
	 * Equipments
	 */
	int32 MaxHaveEquipmentsNum = 100;

	UPROPERTY(Transient)
	TArray<class UPokeEquipment*> HaveEquipments;

	/** 
	 * Items
	 */
	UPROPERTY(Transient)
	TArray<class UPokeItem*> HaveItems;

	/** 
	 * Character Party
	 */
	// Key: SlotNum, Value: ID
	//UPROPERTY(Transient, EditDefaultsOnly)
	//TMap<int32, int32> FirstParty;

	UPROPERTY(EditDefaultsOnly)
	TArray<int32> SavedCharacterKeys;

	UPROPERTY(EditDefaultsOnly)
	TArray<int32> SavedEquipmentKeys;

	UPROPERTY(EditDefaultsOnly)
	TMap<int32, int32> SavedItemKeys;

	/** 
	 * Resource
	 */
	int32 BerryAmount = 1000;		// Need for start battle
	int32 MoneyAmount = 2000;		// Need for buy items..
	int32 StardustAmount = 3000;	// Charged resource

	float BerryChargingIntervalAgeSeconds = 0.0f;
	float MoneyChargingIntervalAgeSeconds = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float BerryChargingIntervalSeconds = 60.0f;

	UPROPERTY(EditDefaultsOnly)
	float MoneyChargingIntervalSeconds = 60.0f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 BerryChargingAmount = 10;

	UPROPERTY(EditDefaultsOnly)
	int32 MoneyChargingAmount = 10;
	
	/** Loaded */
	bool bPlayerLoaded = false;
};

