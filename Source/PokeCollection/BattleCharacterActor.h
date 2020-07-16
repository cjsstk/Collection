// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "PokeCharacter.h"
#include "BattleCharacterActor.generated.h"

enum class ESpriteType
{
	Idle,
	Attack,
	Move,
	Skill,
};

UCLASS()
class POKECOLLECTION_API ABattleCharacterActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABattleCharacterActor();

	void InitBattleCharacter(class APokeCharacter& InPokeCharacter);

	void TakeBattleDamage(int32 InDamage);
	void ChangeSprite(ESpriteType InSpriteType, int32 InSkillIndex = 0);

	FName GetCharacterName() const { return CharacterName; }
	UTexture2D* GetBattleProfile() const { return CharacterBattleProfile; }
	const TArray<AActor*>& GetAttackOverlapActors() const { return AttackOverlapActors; };
	const TArray<class UPokeSkill*>& GetSkills() const { return Skills; }
	const FStatus& GetFinalStatus() const { return CurrentFinalStatus; }
	bool IsEnemy() const { return bIsEnemy; }
	bool IsDead() const;
	bool IsAttacking() const;
	int32 GetCurrentBattleSpeedMultiplier() const { return CurrentBattleSpeed; }
	int32 GetJoinedSlotNum() const { return JoinedSlotNum; }
	int32 GetCharacterLevel() const { return CharacterLevel; }

	UFUNCTION()
	void OnBattleStarted();

	UFUNCTION()
	void OnBattleEnded();

	UFUNCTION()
	void OnFlipbookPlayingEnd();

	UFUNCTION()
	void OnBattleSpeedChange(int32 NewBattleSpeed);

	void AddDebugString(const FString& InDebugString, bool bAddNewLine = true);

	class UPaperFlipbookComponent* GetRenderComponent() const { return RenderComponent; }
	class UBattleCharacterMovementComponent* GetCharacterMovementComponent() const { return MovementComponent; }
	class UBattleCharacterCombatComponent* GetCombatComponent() const { return CombatComponent; }
	class UBattleCharacterHealthComponent* GetHealthComponent() const { return HealthPointComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void TickUpdateAttackOverlapActors();
	void SetFinalStatus(FStatus& InFinalStatus);
	void SetBattleSpeed(int32 NewBattleSpeed);

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BodyBoxComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UPaperFlipbookComponent* RenderComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBattleCharacterMovementComponent* MovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBattleCharacterCombatComponent* CombatComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBattleCharacterHealthComponent* HealthPointComponent = nullptr;

	UPROPERTY(Transient)
	UTexture2D* CharacterBattleProfile;

	UPROPERTY(Transient)
	TArray<AActor*> AttackOverlapActors;

	UPROPERTY(Transient)
	TArray<class UPokeSkill*> Skills;

	UPROPERTY(Transient)
	UPaperFlipbook* CharacterSprite_Idle;

	UPROPERTY(Transient)
	UPaperFlipbook* CharacterSprite_Attack;

	UPROPERTY(Transient)
	UPaperFlipbook* CharacterSprite_Move;

	UPROPERTY(Transient)
	TArray<UPaperFlipbook*> CharacterSprite_Skills;

	FStatus CurrentFinalStatus;

	bool bIsEnemy = false;
	float AttackRange = 0.0f;

	int32 CurrentBattleSpeed = 1;

	FName CharacterName;
	FString DebugString;

	int32 JoinedSlotNum = 0;

	int32 CharacterLevel = 0;
};
