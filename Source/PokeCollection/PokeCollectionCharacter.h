// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CMSType.h"
#include "PokeCollectionCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddedNewCharacter);


enum class EPlayerMode
{
	BattleMode,
	MakePartyMode,
	UIMode,
};


/**
 * 플레이어 클래스
 */
UCLASS(config=Game)
class APokeCollectionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APokeCollectionCharacter();

	void SetPlayerMode(EPlayerMode NewPlayerMode);

	void AddNewCharacter(FInitCharacterParams& InInitCharacterParams);
	void GetReward(FBattleReward InBattleReward);
	void SetMaxClearBattleStage(battleStageKey InBattleStageKey);

	/** 
	 * Player Info
	 */
	FName GetPlayerNickName() const { return PlayerNickName; }
	
	int32 GetPlayerLevel() const { return PlayerLevel; }
	void SetPlayerLevel(int32 NewLevel);

	int32 GetPlayerCurrentExp() const { return PlayerCurrentExp; }
	int32 GetPlayerMaxExp() const { return PlayerMaxExp; }

	void SetCurrentSelectedBattleStageKey(battleStageKey InBattleStageKey);
	battleStageKey GetCurrentSelectedBattleStageKey() const { return CurrentSelectedBattleStageKey; }

	void SetCurrentSelectedStageNum(int32 NewSelectedStageNum) { CurrentSelectedStageNum = FMath::Clamp(NewSelectedStageNum, 1, MaxOpenedStageNum); }
	int32 GetCurrentSelectedStageNum() const { return CurrentSelectedStageNum; }

	void SetCurrentSelectedPartyNum(int32 NewSelectedPartyNum) { CurrentSelectedParty = NewSelectedPartyNum; }
	int32 GetCurrentSelectedPartyNum() const { return CurrentSelectedParty; }

	int32 GetMaxHaveCharactersNum() const { return MaxHaveCharactersNum; }

	bool IsCompleteIndexCharacter(characterKey InCharacterKey);;
	bool IsClearBattleStage(int32 InBattleStageKey);

	const TArray<class APokeCharacter*>& GetHaveCharacters() const;
	const TMap<int32, class APokeCharacter*> GetPartyCharacters(int32 InPartyNum) const;
	class APokeCharacter* GetCharacterByID(int32 InCharacterID) const;
	class APokeCharacter* GetCharacterBySlotNum(int32 InPartyNum, int32 InSlotNum) const;

	const TArray<class UPokeEquipment*>& GetHaveEquipments() const;
	class UPokeEquipment* GetEquipmentByID(int32 InEquipmentID) const;

	int32 GetBerryAmount() const { return BerryAmount; }
	int32 GetMoneyAmount() const { return MoneyAmount; }
	int32 GetStardustAmount() const { return StardustAmount; }
	int32 GetBerryChargingAmount() const { return BerryChargingAmount; }
	int32 GetMoneyChargingAmount() const { return MoneyChargingAmount; }
	int32 GetBerryChargingIntervalMinutes() const { return BerryChargingIntervalSeconds / 60; }
	int32 GetMoneyChargingIntervalMinutes() const { return MoneyChargingIntervalSeconds / 60; }

	void ConsumeBerry(int32 InConsumeBerryAmount);
	void SetMoneyAmount(int32 NewMoneyAmount) { MoneyAmount = FMath::Clamp(NewMoneyAmount, 0, INT32_MAX); }

	FOnAddedNewCharacter OnAddedNewCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InitHaveCharacters();
	void InitHaveEquipments();
	void InitPlayerInfo();

	void SetPlayerMaxExp(int32 InMaxExp);
	void SetBerryAmount(int32 NewBerryAmount) { BerryAmount = FMath::Clamp(NewBerryAmount, 0, INT32_MAX); }

	void TickResourceCharge(float DeltaSeconds);
	void AddCharacterToIndex(characterKey InCharacterKey);
	

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent = nullptr;

	/** 
	 * Player Info
	 */
	UPROPERTY()
	FName PlayerNickName = FName("Moong");
	int32 PlayerLevel = 1;
	int32 PlayerCurrentExp = 0;
	int32 PlayerMaxExp = 1000;

	battleStageKey CurrentSelectedBattleStageKey = 0;
	int32 CurrentSelectedStageNum = 1;
	int32 MaxClearBattleStageNum = 0;
	int32 MaxOpenedStageNum = 2;

	int32 CurrentSelectedParty = 1;

	/**
	 * Characters
	 */
	int32 NextCharacterID = 0;

	int32 MaxHaveCharactersNum = 100;

	UPROPERTY(Transient)
	TArray<class APokeCharacter*> HaveCharacters;

	TMap<int32, bool> CharacterIndex;

	/** 
	 * Equipments
	 */
	int32 NextEquipmentID = 0;

	int32 MaxHaveEquipmentsNum = 100;

	UPROPERTY(Transient)
	TArray<class UPokeEquipment*> HaveEquipments;


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

	/** 
	 * Resource
	 */
	int32 BerryAmount = 1000;		// Need for start battle
	int32 MoneyAmount = 2000;		// Need for buy items..
	int32 StardustAmount = 3000;	// Charged resource

	float BerryChargingIntervalAgeSeconds = 0.0f;
	float MoneyChargingIntervalAgeSeconds = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float BerryChargingIntervalSeconds = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float MoneyChargingIntervalSeconds = 10.0f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 BerryChargingAmount = 10;

	UPROPERTY(EditDefaultsOnly)
	int32 MoneyChargingAmount = 10;

};

