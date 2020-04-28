// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillProjectileActor.generated.h"

UCLASS()
class POKECOLLECTION_API ASkillProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillProjectileActor();

	void SetProjectileDamage(int32 InDamage) { ProjectileDamage = InDamage; }
	void SetTargetCharacter(class ABattleCharacterActor& InTargetCharacter) { TargetCharacter = &InTargetCharacter; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnHitTarget();

private:
	void TickMove(float DeltaTime);
	void TickCheckHitTarget();

	UPROPERTY(VisibleAnywhere)
	class UPaperFlipbookComponent* RenderComponent = nullptr;

	UPROPERTY(Transient)
	class ABattleCharacterActor* TargetCharacter = nullptr;

	int32 ProjectileDamage = 0;
};

UCLASS()
class POKECOLLECTION_API ASkillProjectile_Ember : public ASkillProjectileActor
{
	GENERATED_BODY()

public:


private:

};