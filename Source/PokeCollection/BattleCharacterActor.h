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
};

UCLASS()
class POKECOLLECTION_API ABattleCharacterActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABattleCharacterActor();

	void InitBattleCharacter(class APokeCharacter& InPokeCharacter);

	void TakeBattleDamage(int32 InDamage);
	void ChangeSprite(ESpriteType InSpriteType);


	UTexture2D* GetBattleProfile() const { return CharacterBattleProfile; }
	const TArray<AActor*>& GetAttackOverlapActors() const { return AttackOverlapActors; };
	const TArray<class UPokeSkill*>& GetSkills() const { return Skills; }
	const FStatus& GetFinalStatus() const { return CurrentFinalStatus; }
	bool IsEnemy() const { return bIsEnemy; }
	bool IsDead() const;
	bool IsAttacking() const;

	UFUNCTION()
	void OnBattleEnded();

	UFUNCTION()
	void OnFlipbookPlayingEnd();

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

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BodyBoxComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AttackRangeSphereComponent = nullptr;

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

	FStatus CurrentFinalStatus;

	bool bIsEnemy = false;

	FString DebugString;
};
