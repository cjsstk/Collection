// Copyright Me. All Rights Reserved.


#include "SkillProjectileActor.h"

#include "BattleCharacterActor.h"
#include "PaperFlipbookComponent.h"

ASkillProjectileActor::ASkillProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RenderComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	RenderComponent->SetupAttachment(RootComponent);
	RenderComponent->SetWorldRotation(FRotator(0, 90, 0));
}

void ASkillProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkillProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickMove(DeltaTime);
	TickCheckHitTarget();
}

void ASkillProjectileActor::OnHitTarget()
{
	if (!TargetCharacter || TargetCharacter->IsDead())
	{
		return;
	}

	TargetCharacter->TakeBattleDamage(ProjectileDamage);
	Destroy();
}

void ASkillProjectileActor::TickMove(float DeltaTime)
{
	float MoveSpeed = DeltaTime * 100;
	FVector NewLocation = GetActorLocation() + FVector(0, MoveSpeed, 0);

	SetActorLocation(NewLocation);
}

void ASkillProjectileActor::TickCheckHitTarget()
{
	if (!TargetCharacter || TargetCharacter->IsDead())
	{
		Destroy();
		return;
	}

	FVector My = GetActorLocation();
	FVector Tar = TargetCharacter->GetActorLocation();

	if (FMath::IsNearlyEqual(GetActorLocation().Y, TargetCharacter->GetActorLocation().Y, 10.0f))
	{
		OnHitTarget();
	}
}

