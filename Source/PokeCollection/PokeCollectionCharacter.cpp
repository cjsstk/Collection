// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PokeCollectionCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PokeCharacter.h"
#include "PokeCollectionGameMode.h"

//////////////////////////////////////////////////////////////////////////
// APokeCollectionCharacter

APokeCollectionCharacter::APokeCollectionCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

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
	if (ensure(TempCharacter.Get()))
	{
		APokeCharacter* PokeCharacter = NewObject<APokeCharacter>(TempCharacter.Get());
		if (PokeCharacter)
		{
			PokeCharacter->SetJoinedPartyNum(1);
			PokeCharacter->SetJoinedSlotNum(1);
			HaveCharacters.AddUnique(PokeCharacter);
		}
	}
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
