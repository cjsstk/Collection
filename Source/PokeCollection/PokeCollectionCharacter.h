// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.generated.h"


/**
 * 플레이어 클래스
 */
UCLASS(config=Game)
class APokeCollectionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APokeCollectionCharacter();

	int32 GetCurrentSelectedStageNum() const { return CurrentSelectedStageNum; }

	const TMap<int32, class APokeCharacter*> GetPartyCharacters(int32 InPartyNum) const;

	int32 GetBerryAmount() const { return BerryAmount; }
	int32 GetMoneyAmount() const { return MoneyAmount; }
	int32 GetStardustAmount() const { return StardustAmount; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent = nullptr;

	/*
	 * 
	 */
	int32 NextCharacterID = 0;
	int32 CurrentSelectedStageNum = 1;

	UPROPERTY()
	TArray<class APokeCharacter*> HaveCharacters;
	
	/** 
	 * Character Party
	 */
	// Key: SlotNum, Value: ID
	UPROPERTY(Transient)
	//TArray<int32> FirstParty;
	TMap<int32, int32> FirstParty;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APokeCharacter> TempCharacter;

	/** 
	 * Resource
	 */
	int32 BerryAmount = 1000;		// Need for start battle
	int32 MoneyAmount = 2000;		// Need for buy items..
	int32 StardustAmount = 3000;	// Charged resource
};

