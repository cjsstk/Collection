// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.generated.h"


/**
 * �÷��̾� Ŭ����
 */
UCLASS(config=Game)
class APokeCollectionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APokeCollectionCharacter();

	const TMap<int32, class APokeCharacter*> GetPartyCharacters(int32 InPartyNum) const;

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
	//TArray<int32> FirstParty;
	TMap<int32, int32> FirstParty;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APokeCharacter> TempCharacter;

};
