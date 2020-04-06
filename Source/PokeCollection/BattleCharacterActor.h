// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "PokeCharacter.h"
#include "BattleCharacterActor.generated.h"


UCLASS()
class POKECOLLECTION_API ABattleCharacterActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABattleCharacterActor();

	void InitBattleCharacter(class APokeCharacter& InPokeCharacter);

	const TArray<AActor*>& GetAttackOverlapActors() const { return AttackOverlapActors; };
	const FStatus& GetFinalStatus() const { return CurrentFinalStatus; }
	bool IsEnemy() const { return bIsEnemy; }

	void AddDebugString(const FString& InDebugString, bool bAddNewLine = true);

	class UPaperFlipbookComponent* GetRenderComponent() const { return RenderComponent; }

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	void SetFinalStatus(FStatus& InFinalStatus);

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BodyBoxComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AttackRangeSphereComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UPaperFlipbookComponent* RenderComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBattleCharacterMovementComponent* MovementComponent = nullptr;

	UPROPERTY(Transient)
	TArray<AActor*> AttackOverlapActors;

	FStatus CurrentFinalStatus;

	bool bIsEnemy = false;

	FString DebugString;
};
