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

	void InitHP(int32 InHealthPoint);
	void TakeDamage(int32 InDamage);

	bool IsDead() const { return bIsDead; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void SetHealthPoint(int32 InHealthPoint);
	void SetMaxHealthPoint(int32 InMaxHealthPoint);

	int32 CurrentHealthPoint = 0;
	int32 MaxHealthPoint = 0;
	
	bool bIsDead = false;
};
