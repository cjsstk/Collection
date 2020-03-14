// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PokeCollectionCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PokeCollectionGameMode.h"
#include "CMS.h"

//////////////////////////////////////////////////////////////////////////
// APokeCollectionCharacter

APokeCollectionCharacter::APokeCollectionCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

}

void APokeCollectionCharacter::InitHaveCharacters()
{
	NextCharacterID = 0;

	for (characterKey Key : SavedCharacterKeys)
	{
		const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(Key);
		if (!CharacterInfo)
		{
			ensure(0);
			continue;
		}

		APokeCharacter* PokeCharacter = NewObject<APokeCharacter>();
		if (PokeCharacter)
		{
			PokeCharacter->Init(Key);
			PokeCharacter->SetCharacterID(NextCharacterID);
			++NextCharacterID;
		}

		HaveCharacters.AddUnique(PokeCharacter);
	}
}

void APokeCollectionCharacter::SetPlayerMode(EPlayerMode NewPlayerMode)
{
	switch (NewPlayerMode)
	{
	case EPlayerMode::BattleMode:
		CameraComponent->SetRelativeRotation(FRotator(0, 0, 0));
		break;
	case EPlayerMode::MakePartyMode:
		CameraComponent->SetRelativeRotation(FRotator(0, 180, 0));
		break;
	default:
		break;
	}
}

void APokeCollectionCharacter::SetCurrentSelectedBattleStageKey(battleStageKey InBattleStageKey)
{
	if (InBattleStageKey == INVALID_BATTLESTAGEKEY)
	{
		return;
	}

	CurrentSelectedBattleStageKey = InBattleStageKey;
}

const TMap<int32, class APokeCharacter*> APokeCollectionCharacter::GetPartyCharacters(int32 InPartyNum) const
{
	TMap<int32, class APokeCharacter*> FoundPartyCharacters;

	for (auto&& Character : HaveCharacters)
	{
		if (Character->GetJoinedPartyNum() == InPartyNum)
		{
			FoundPartyCharacters.Add(Character->GetJoinedSlotNum(), Character);
		}
	}

	return FoundPartyCharacters;
}

APokeCharacter* APokeCollectionCharacter::GetCharacterByID(int32 InCharacterID) const
{
	for (APokeCharacter* Character : HaveCharacters)
	{
		if (Character && Character->GetCharacterID() == InCharacterID)
		{
			return Character;
		}
	}

	return nullptr;
}

APokeCharacter* APokeCollectionCharacter::GetCharacterBySlotNum(int32 InPartyNum, int32 InSlotNum) const
{
	return *HaveCharacters.FindByPredicate([InPartyNum, InSlotNum](APokeCharacter* PC)
		{ 
			return (PC->GetJoinedPartyNum() == InPartyNum && PC->GetJoinedSlotNum() == InSlotNum);
		}
	);
}

void APokeCollectionCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitHaveCharacters();

	// Temp slot setting
	for (int32 i = 1; i < 5; i++)
	{
		HaveCharacters[i * 2]->SetJoinedPartyNum(1);
		HaveCharacters[i * 2]->SetJoinedSlotNum(i);
	}
}

void APokeCollectionCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickResourceCharge(DeltaSeconds);
}

void APokeCollectionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);


}

void APokeCollectionCharacter::TickResourceCharge(float DeltaSeconds)
{
	BerryChargingIntervalAgeSeconds += DeltaSeconds;
	MoneyChargingIntervalAgeSeconds += DeltaSeconds;

	while (BerryChargingIntervalAgeSeconds >= BerryChargingIntervalSeconds)
	{
		BerryChargingIntervalAgeSeconds -= BerryChargingIntervalSeconds;
		BerryAmount += BerryChargingAmount;
	}

	while (MoneyChargingIntervalAgeSeconds >= MoneyChargingIntervalSeconds)
	{
		MoneyChargingIntervalAgeSeconds -= MoneyChargingIntervalSeconds;
		MoneyAmount += MoneyChargingAmount;
	}
}
