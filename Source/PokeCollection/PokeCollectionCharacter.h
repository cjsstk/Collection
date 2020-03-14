// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.generated.h"

enum class EPlayerMode
{
	BattleMode,
	MakePartyMode,
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

	void InitHaveCharacters();
	void SetPlayerMode(EPlayerMode NewPlayerMode);

	FName GetPlayerNickName() const { return PlayerNickName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	int32 GetPlayerCurrentExp() const { return PlayerCurrentExp; }
	int32 GetPlayerMaxExp() const { return PlayerMaxExp; }

	void SetCurrentSelectedBattleStageKey(battleStageKey InBattleStageKey);
	battleStageKey GetCurrentSelectedBattleStageKey() const { return CurrentSelectedBattleStageKey; }

	void SetCurrentSelectedStageNum(int32 NewSelectedStageNum) { CurrentSelectedStageNum = FMath::Clamp(NewSelectedStageNum, 1, MaxOpenedStageNum); }
	int32 GetCurrentSelectedStageNum() const { return CurrentSelectedStageNum; }

	int32 GetMaxHaveCharactersNum() const { return MaxHaveCharactersNum; }

	const TArray<class APokeCharacter*>& GetHaveCharacters() const { return HaveCharacters; }
	const TMap<int32, class APokeCharacter*> GetPartyCharacters(int32 InPartyNum) const;
	class APokeCharacter* GetCharacterByID(int32 InCharacterID) const;
	class APokeCharacter* GetCharacterBySlotNum(int32 InPartyNum, int32 InSlotNum) const;

	int32 GetBerryAmount() const { return BerryAmount; }
	int32 GetMoneyAmount() const { return MoneyAmount; }
	int32 GetStardustAmount() const { return StardustAmount; }
	int32 GetBerryChargingAmount() const { return BerryChargingAmount; }
	int32 GetMoneyChargingAmount() const { return MoneyChargingAmount; }
	int32 GetBerryChargingIntervalMinutes() const { return BerryChargingIntervalSeconds / 60; }
	int32 GetMoneyChargingIntervalMinutes() const { return MoneyChargingIntervalSeconds / 60; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void TickResourceCharge(float DeltaSeconds);

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

	/*
	 * 
	 */
	int32 NextCharacterID = 0;
	battleStageKey CurrentSelectedBattleStageKey = 0;
	int32 CurrentSelectedStageNum = 1;
	int32 MaxOpenedStageNum = 2;

	int32 MaxHaveCharactersNum = 100;

	UPROPERTY(Transient)
	TArray<class APokeCharacter*> HaveCharacters;
	
	/** 
	 * Character Party
	 */
	// Key: SlotNum, Value: ID
	//UPROPERTY(Transient, EditDefaultsOnly)
	//TMap<int32, int32> FirstParty;

	UPROPERTY(EditDefaultsOnly)
	TArray<int32> SavedCharacterKeys;

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

