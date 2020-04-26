// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterCombatComponent.h"

#include "BattleCharacterActor.h"

UBattleCharacterCombatComponent::UBattleCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBattleCharacterCombatComponent::AttackTarget()
{
	if (!TargetCharacter)
	{
		return;
	}

	ABattleCharacterActor* Character = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(Character))
	{
		return;
	}

	Character->ChangeSprite(ESpriteType::Attack);

	const FStatus& Status = Character->GetFinalStatus();
	int32 AttackDamage = Status.Attack;
	
	if (AttackDamage <= 0)
	{
		AttackDamage = 1;
	}

	TargetCharacter->TakeBattleDamage(AttackDamage);
}

void UBattleCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBattleCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickFindNewTarget();
	TickAttackTarget(DeltaTime);
}

void UBattleCharacterCombatComponent::TickFindNewTarget()
{
	if (TargetCharacter)
	{
		if (TargetCharacter->IsDead())
		{
			TargetCharacter = nullptr;
		}
		return;
	}

	ABattleCharacterActor* BattleCharacter = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(BattleCharacter))
	{
		return;
	}

	// Temp
	if (BattleCharacter->IsEnemy())
	{
		return;
	}
	//

	const TArray<AActor*> AttackOverlapActors = BattleCharacter->GetAttackOverlapActors();
	for (AActor* AttackOverlapActor : AttackOverlapActors)
	{
		ABattleCharacterActor* OverlapCharacter = Cast<ABattleCharacterActor>(AttackOverlapActor);
		if (OverlapCharacter)
		{
			// TODO : Need Target Rule
			TargetCharacter = OverlapCharacter;
			break;
		}
	}

}

void UBattleCharacterCombatComponent::TickAttackTarget(float DeltaTime)
{
	AttackDelayAgeSeconds += DeltaTime;

	if (AttackDelayAgeSeconds < AttackDelaySeconds)
	{
		return;
	}

	if (!TargetCharacter)
	{
		return;
	}

	if (TargetCharacter->IsDead())
	{
		TargetCharacter = nullptr;
		return;
	}

	AttackTarget();
	AttackDelayAgeSeconds = 0.0f;
	
}

