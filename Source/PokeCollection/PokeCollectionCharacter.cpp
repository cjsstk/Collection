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

const TArray<class APokeCharacter*> APokeCollectionCharacter::GetFirstPartyCharacters() const
{
	TArray<class APokeCharacter*> FoundPartyCharacters;

	for (int32 ID : FirstParty)
	{
		for (auto&& Character : HaveCharacters)
		{
			if (Character->GetCharacterID() == ID)
			{
				FoundPartyCharacters.Add(Character);
				break;
			}
		}
	}

	return FoundPartyCharacters;
}

void APokeCollectionCharacter::BeginPlay()
{
	Super::BeginPlay();

	/** Temp Init HaveCharacters */
	APokeCharacter* PokeCharacter = NewObject<APokeCharacter>(TempCharacter.Get());
	HaveCharacters.AddUnique(PokeCharacter);


	NextCharacterID = 0;
	for (APokeCharacter* Character : HaveCharacters)
	{
		if (!ensure(Character))
		{
			continue;
		}

		// Temp add party
		FirstParty.Add(NextCharacterID);

		Character->SetCharacterID(NextCharacterID);
		++NextCharacterID;
	}

}

void APokeCollectionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);


}
