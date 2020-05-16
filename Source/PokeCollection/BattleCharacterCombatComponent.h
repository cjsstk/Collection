// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleCharacterCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterAttack);

struct FPokeUseSkillParams;

UCLASS()
class POKECOLLECTION_API UBattleCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattleCharacterCombatComponent();

	void AttackTarget();
	void UseActiveSkill(FPokeUseSkillParams& InParams);

	class ABattleCharacterActor* GetCurrentTargetCharacter() const { return TargetCharacter; };

	FOnCharacterAttack OnCharacterAttack;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void TickFindNewTarget();
	void TickAttackTarget(float DeltaTime);

	UPROPERTY(Transient)
	class ABattleCharacterActor* TargetCharacter = nullptr;
		
	float AttackDelaySeconds = 2.0f;
	float AttackDelayAgeSeconds = 0.0f;

};
