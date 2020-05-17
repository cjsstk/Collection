// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleCharacterMovementComponent.generated.h"


UCLASS()
class POKECOLLECTION_API UBattleCharacterMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattleCharacterMovementComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void TickMovement(float DeltaTime);

	class ABattleCharacterActor* GetBattleCharacter();

	bool bIsMoving = false;
};
