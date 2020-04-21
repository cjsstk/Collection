// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterHealthComponent.h"

#include "BattleCharacterActor.h"

// Sets default values for this component's properties
UBattleCharacterHealthComponent::UBattleCharacterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBattleCharacterHealthComponent::TakeDamage(int32 InDamage)
{
	SetHealthPoint(CurrentHealthPoint - InDamage);
}

void UBattleCharacterHealthComponent::SetHealthPoint(int32 InHealthPoint)
{
	CurrentHealthPoint = FMath::Clamp(InHealthPoint, 0, MaxHealthPoint);

	if (CurrentHealthPoint <= 0)
	{
		bIsDead = true;
		GetOwner()->Destroy();
	}
}

void UBattleCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ABattleCharacterActor* BattleCharacter = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(BattleCharacter))
	{
		return;
	}

	MaxHealthPoint = BattleCharacter->GetFinalStatus().HealthPoint;
	CurrentHealthPoint = MaxHealthPoint;
}

void UBattleCharacterHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ABattleCharacterActor* BattleCharacter = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(BattleCharacter))
	{
		return;
	}

	BattleCharacter->AddDebugString(FString::Printf(TEXT("HP : %d/%d"), CurrentHealthPoint, MaxHealthPoint), true);
}

