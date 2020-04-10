// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleCharacterHealthComponent.generated.h"


UCLASS()
class POKECOLLECTION_API UBattleCharacterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattleCharacterHealthComponent();

	void SetHealthPoint(int32 InHealthPoint);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 CurrentHealthPoint = 0;
	int32 MaxHealthPoint = 0;
		
};