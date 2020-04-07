// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterCombatComponent.h"

#include "BattleCharacterActor.h"

UBattleCharacterCombatComponent::UBattleCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBattleCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBattleCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickFindNewTarget();
}

void UBattleCharacterCombatComponent::TickFindNewTarget()
{
	if (TargetCharacter)
	{
		return;
	}

	ABattleCharacterActor* BattleCharacter = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(BattleCharacter))
	{
		return;
	}

	const TArray<AActor*> AttackOverlapActors = BattleCharacter->GetAttackOverlapActors();
	for (AActor* AttackOverlapActor : AttackOverlapActors)
	{
		ABattleCharacterActor* OverlapCharacter = Cast<ABattleCharacterActor>(AttackOverlapActor);
		if (!OverlapCharacter)
		{
			//OverlapCharacter->g
		}
	}

}

