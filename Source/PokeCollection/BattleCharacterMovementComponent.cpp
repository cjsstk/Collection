// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterMovementComponent.h"

#include "BattleCharacterActor.h"

UBattleCharacterMovementComponent::UBattleCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBattleCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBattleCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ABattleCharacterActor* BattleCharacter = GetBattleCharacter();
	if (!ensure(BattleCharacter))
	{
		return;
	}

	if (BattleCharacter->IsAttacking())
	{
		return;
	}

	const TArray<AActor*>& AttackOverlapCharacters = BattleCharacter->GetAttackOverlapActors();
	if (AttackOverlapCharacters.Num() > 0)
	{
		//BattleCharacter->AddDebugString(FString::Printf(TEXT("%d"), AttackOverlapCharacters.Num()), true);
		return;
	}

	float MoveSpeed = DeltaTime * BattleCharacter->GetFinalStatus().Speed * 50;
	MoveSpeed = BattleCharacter->IsEnemy() ? MoveSpeed * -1 : MoveSpeed;
	FVector NewLocation = BattleCharacter->GetActorLocation() + FVector(0, MoveSpeed, 0);

	BattleCharacter->SetActorLocation(NewLocation);

	//BattleCharacter->AddDebugString(FString::Printf(TEXT("MoveSpeed : %f"), MoveSpeed), true);
}

class ABattleCharacterActor* UBattleCharacterMovementComponent::GetBattleCharacter()
{
	return Cast<ABattleCharacterActor>(GetOwner());
}

