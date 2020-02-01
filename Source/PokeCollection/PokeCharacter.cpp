// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCharacter.h"

#include "InBattleCharacterPanel.h"

#include "DrawDebugHelpers.h"

APokeCharacter::APokeCharacter()
{
	RootComponent->SetWorldRotation(FRotator(0, 90, 0));
}

void APokeCharacter::Attack(APokeCharacter* TargetCharacter)
{
	if (!TargetCharacter)
	{
		// Buff?
		ensure(0);
		return;
	}

	CurrentTargetCharacter = TargetCharacter;
	bAttacking = true;
}

void APokeCharacter::AddDebugString(const FString& InDebugString, bool bAddNewLine/* = true*/)
{
	DebugString += InDebugString;

	if (bAddNewLine)
	{
		DebugString += TEXT("\n");
	}
}

void APokeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bAttacking)
	{
		const FVector TargetLocation = CurrentTargetCharacter->GetActorLocation();
		const FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, 1.0f);
		SetActorLocation(NewLocation);
	}
	else
	{
		if (ensure(CurrentBattlePanel))
		{
			const FVector NewLocation = FMath::Lerp(GetActorLocation(), CurrentBattlePanel->GetActorLocation(), 1.0f);
			SetActorLocation(NewLocation);
		}
	}


	if (!DebugString.IsEmpty())
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 100), DebugString, this, FColor::White, 0, true);

		DebugString.Empty();
	}
}
