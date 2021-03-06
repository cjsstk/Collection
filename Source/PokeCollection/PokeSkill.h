// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PokeSkill.generated.h"

USTRUCT()
struct FPokeUseSkillParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 CharacterStat = 0;

	UPROPERTY()
	int32 SkillLevel = 1;

	UPROPERTY(Transient)
	class ABattleCharacterActor* TargetCharacter = nullptr;
};

struct InitSkillParams
{
public:
	int32 SkillKey = 0;
	int32 SkillIndex = 0;
	int32 SkillLevel = 0;
};

/**
 * Skill
 */
UCLASS()
class POKECOLLECTION_API UPokeSkill : public UObject
{
	GENERATED_BODY()

public:
	void InitSkill(InitSkillParams InSkillParams);
	void SetSourceCharacter(class ABattleCharacterActor* InSourceCharacter);

	void UseSkill(const FPokeUseSkillParams& Params);

	bool CanUseSkill();
	int32 GetMaxAttackCount() const { return AttackCount; }
	int32 GetCurrentAttackCount() const { return CurrentAttackCount; }
	FText GetSkillName() const { return SkillName; }

	UFUNCTION()
	void OnCharacterAttack();

protected:
	virtual void OnUseSkill(const FPokeUseSkillParams& Params);

	UPROPERTY(Transient)
	class ABattleCharacterActor* SourceCharacter = nullptr;

	FText SkillName;
	int32 SkillDamage = 0;
	int32 AttackCount = 0;
	int32 CurrentAttackCount = 0;
	TSubclassOf<class ASkillProjectileActor> SpawnProjectileActor = nullptr;

};

UCLASS()
class POKECOLLECTION_API UPokeSkill_Ember : public UPokeSkill
{
	GENERATED_BODY()


protected:
	virtual void OnUseSkill(const FPokeUseSkillParams& Params) override;


};