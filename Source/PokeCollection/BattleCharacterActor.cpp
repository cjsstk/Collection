// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterActor.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "DrawDebugHelpers.h"

#include "BattleCharacterMovementComponent.h"
#include "BattleCharacterCombatComponent.h"
#include "BattleCharacterHealthComponent.h"
#include "BattleManager.h"
#include "CMS.h"
#include "PokeCore.h"


ABattleCharacterActor::ABattleCharacterActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyBoxComponent"));
	SetRootComponent(BodyBoxComponent);

	RenderComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	RenderComponent->SetupAttachment(RootComponent);

	AttackRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeComponent"));
	AttackRangeSphereComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UBattleCharacterMovementComponent>(TEXT("MovementComponent"));

	CombatComponent = CreateDefaultSubobject<UBattleCharacterCombatComponent>(TEXT("CombatComponent"));

	HealthPointComponent = CreateDefaultSubobject<UBattleCharacterHealthComponent>(TEXT("HealthPointComponent"));
	//RootComponent->SetWorldRotation(FRotator(0, 90, 0));
}

void ABattleCharacterActor::InitBattleCharacter(class APokeCharacter& InPokeCharacter)
{
	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InPokeCharacter.GetCharacterKey());
	if (!ensure(CharacterInfo))
	{
		return;
	}

	if (ensure(RenderComponent))
	{
		RenderComponent->SetFlipbook(CharacterInfo->CharacterSprite);
	}

	FStatus FinalStatus = InPokeCharacter.GetFinalStatus();
	SetFinalStatus(FinalStatus);

	bIsEnemy = InPokeCharacter.IsEnemy();

	float AttackRange = InPokeCharacter.GetAttackRange();
	AttackRangeSphereComponent->SetSphereRadius(AttackRange);
}

void ABattleCharacterActor::TakeBattleDamage(int32 InDamage)
{
	if (HealthPointComponent)
	{
		HealthPointComponent->TakeDamage(InDamage);
	}
}

bool ABattleCharacterActor::IsDead() const
{
	return HealthPointComponent && HealthPointComponent->IsDead();
}

void ABattleCharacterActor::OnBattleEnded()
{
	MovementComponent->SetComponentTickEnabled(false);
}

void ABattleCharacterActor::AddDebugString(const FString& InDebugString, bool bAddNewLine/* = true*/)
{
	DebugString += InDebugString;

	if (bAddNewLine)
	{
		DebugString += TEXT("\n");
	}
}

void ABattleCharacterActor::BeginPlay()
{
	Super::BeginPlay();

	ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
	if (ensure(BattleManager))
	{
		BattleManager->OnBattleEnd.AddUniqueDynamic(this, &ABattleCharacterActor::OnBattleEnded);
	}
}

void ABattleCharacterActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AttackRangeSphereComponent)
	{
		AttackOverlapActors.Empty();

		TArray<AActor*> OverlapActors;
		AttackRangeSphereComponent->GetOverlappingActors(OverlapActors, ABattleCharacterActor::StaticClass());

		for (AActor* OverlapActor : OverlapActors)
		{
			if (!OverlapActor || OverlapActor == this)
			{
				continue;
			}

			ABattleCharacterActor* OverlapBattleCharacter = Cast<ABattleCharacterActor>(OverlapActor);
			if (!OverlapBattleCharacter || OverlapBattleCharacter->IsEnemy() == bIsEnemy)
			{
				continue;
			}

			AttackOverlapActors.AddUnique(OverlapActor);
		}
	}

	if (!DebugString.IsEmpty())
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 200), DebugString, this, FColor::White, 0.01f);

		DebugString.Empty();
	}

}

void ABattleCharacterActor::SetFinalStatus(FStatus& InFinalStatus)
{
	CurrentFinalStatus.HealthPoint = InFinalStatus.HealthPoint;
	CurrentFinalStatus.Attack = InFinalStatus.Attack;
	CurrentFinalStatus.Defense = InFinalStatus.Defense;
	CurrentFinalStatus.SpecialAttack = InFinalStatus.SpecialAttack;
	CurrentFinalStatus.SpecialDefense = InFinalStatus.SpecialDefense;
	CurrentFinalStatus.Speed = InFinalStatus.Speed;

	if (ensure(HealthPointComponent))
	{
		HealthPointComponent->InitHP(CurrentFinalStatus.HealthPoint);
	}
}
