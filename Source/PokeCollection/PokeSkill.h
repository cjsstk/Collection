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
	UPROPERTY(Transient)
	class ABattleCharacterActor* SourceCharacter = nullptr;

	UPROPERTY(Transient)
	class ABattleCharacterActor* TargetCharacter = nullptr;

	UPROPERTY()
	int32 SkillDamage = 0;

	UPROPERTY(Transient)
	TSubclassOf<class AActor> SpawnActor = nullptr;
};


/**
 * Skill
 */
UCLASS()
class POKECOLLECTION_API UPokeSkill : public UObject
{
	GENERATED_BODY()

public:
	void UseSkill(const FPokeUseSkillParams& Params);

protected:
	virtual void OnUseSkill(const FPokeUseSkillParams& Params);

};


UCLASS()
class POKECOLLECTION_API UPokeSkill_Ember : public UPokeSkill
{
	GENERATED_BODY()

protected:
	virtual void OnUseSkill(const FPokeUseSkillParams& Params) override;

};
