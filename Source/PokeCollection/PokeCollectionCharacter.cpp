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

static TAutoConsoleVariable<int32> CVarShowInViewportWidgetsDebug
(
	TEXT("poke.showInViewportWidgetsDebug"),
	0, 
	TEXT("1: Enable debug, 0: Disable debug")
);

//////////////////////////////////////////////////////////////////////////
// APokeCollectionCharacter

APokeCollectionCharacter::APokeCollectionCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

}

void APokeCollectionCharacter::InitHaveCharacters()
{
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
		}

		HaveCharacters.AddUnique(PokeCharacter);
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

void APokeCollectionCharacter::BeginPlay()
{
	Super::BeginPlay();

	/** Temp Init HaveCharacters */
	InitHaveCharacters();

	/*if (ensure(TempCharacter.Get()))
	{
		APokeCharacter* PokeCharacter = NewObject<APokeCharacter>(TempCharacter.Get());
		if (PokeCharacter)
		{
			PokeCharacter->SetJoinedPartyNum(1);
			PokeCharacter->SetJoinedSlotNum(1);
			HaveCharacters.AddUnique(PokeCharacter);
		}
	}*/
	//

	NextCharacterID = 0;
	for (APokeCharacter* Character : HaveCharacters)
	{
		if (!ensure(Character))
		{
			continue;
		}

		// Temp add party
		FirstParty.Add(0, NextCharacterID);

		Character->SetCharacterID(NextCharacterID);
		++NextCharacterID;
	}

}

void APokeCollectionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);


}
