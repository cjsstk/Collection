// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	const TArray<class APokeCharacter*> GetFirstPartyCharacters() const;

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

	UPROPERTY()
	TArray<class APokeCharacter*> HaveCharacters;
	
	/** 
	 * Character Party
	 */
	// Key: SlotNum, Value: ID
	UPROPERTY(Transient)
	TArray<int32> FirstParty;



	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APokeCharacter> TempCharacter;

};

