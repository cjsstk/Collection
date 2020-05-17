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
#include "PokeCollectionCharacter.h"
#include "PokeSkill.h"


ABattleCharacterActor::ABattleCharacterActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyBoxComponent"));
	SetRootComponent(BodyBoxComponent);

	RenderComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	RenderComponent->SetupAttachment(RootComponent);

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
		CharacterSprite_Idle = CharacterInfo->CharacterSprite_Idle.LoadSynchronous();
		CharacterSprite_Attack = CharacterInfo->CharacterSprite_Attack.LoadSynchronous();
		CharacterSprite_Move = CharacterInfo->CharacterSprite_Move.LoadSynchronous();

		CharacterSprite_Skills.Add(CharacterInfo->CharacterSprite_Skill1.LoadSynchronous());
		CharacterSprite_Skills.Add(CharacterInfo->CharacterSprite_Skill2.LoadSynchronous());
		CharacterSprite_Skills.Add(CharacterInfo->CharacterSprite_Skill3.LoadSynchronous());
		CharacterSprite_Skills.Add(CharacterInfo->CharacterSprite_Skill4.LoadSynchronous());

		RenderComponent->SetFlipbook(CharacterSprite_Idle);
	}

	CharacterBattleProfile = CharacterInfo->CharacterBattleProfile.LoadSynchronous();
	CharacterName = CharacterInfo->CharacterName;

	FStatus FinalStatus = InPokeCharacter.GetFinalStatus();
	SetFinalStatus(FinalStatus);

	bIsEnemy = InPokeCharacter.IsEnemy();

	AttackRange = InPokeCharacter.GetAttackRange();

	const TArray<int32> SkillKeys = CharacterInfo->SkillKeys;
	for (int32 SkillIndex = 0; SkillIndex < 4; ++SkillIndex)
	{
		if (!SkillKeys.IsValidIndex(SkillIndex))
		{
			continue;
		}

		int32 SkillKey = SkillKeys[SkillIndex];

		const FSkillInfo* SkillInfo = CMS::GetSkillDataTable(SkillKey);
		if (!ensure(SkillInfo))
		{
			continue;
		}

		if (!SkillInfo->SkillClass.Get())
		{
			continue;
		}

		UPokeSkill* Skill = NewObject<UPokeSkill>(SkillInfo->SkillClass.Get());
		if (ensure(Skill))
		{
			Skill->InitSkill(SkillKey);
			Skill->SetSourceCharacter(this);
			Skills.Add(Skill);
		}
	}

	APokeCollectionCharacter* Player = PokeCore::GetPokePlayer(GetWorld());
	if (ensure(Player))
	{
		SetBattleSpeed(Player->GetBattleSpeedMultiplier());
	}
}

void ABattleCharacterActor::TakeBattleDamage(int32 InDamage)
{
	if (HealthPointComponent)
	{
		HealthPointComponent->TakeDamage(InDamage);
	}
}

void ABattleCharacterActor::ChangeSprite(ESpriteType InSpriteType, int32 InSkillIndex)
{
	if (!ensure(RenderComponent))
	{
		return;
	}

	switch (InSpriteType)
	{
	case ESpriteType::Idle:
		RenderComponent->SetFlipbook(CharacterSprite_Idle);
		RenderComponent->SetLooping(true);
		break;
	case ESpriteType::Attack:
		RenderComponent->SetFlipbook(CharacterSprite_Attack);
		RenderComponent->SetLooping(false);
		break;
	case ESpriteType::Move:
		RenderComponent->SetFlipbook(CharacterSprite_Move);
		RenderComponent->SetLooping(true);
		break;
	case ESpriteType::Skill:
		if (CharacterSprite_Skills.IsValidIndex(InSkillIndex))
		{
			RenderComponent->SetFlipbook(CharacterSprite_Skills[InSkillIndex]);
		}
		else
		{
			RenderComponent->SetFlipbook(CharacterSprite_Attack);
		}
		RenderComponent->SetLooping(false);
		break;
	default:
		break;
	}
}

bool ABattleCharacterActor::IsDead() const
{
	return HealthPointComponent && HealthPointComponent->IsDead();
}

bool ABattleCharacterActor::IsAttacking() const
{
	if (!RenderComponent)
	{
		return false;
	}

	return (RenderComponent->GetFlipbook() == CharacterSprite_Attack || CharacterSprite_Skills.Contains(RenderComponent->GetFlipbook()));
}

void ABattleCharacterActor::OnBattleEnded()
{
	MovementComponent->SetComponentTickEnabled(false);
}

void ABattleCharacterActor::OnFlipbookPlayingEnd()
{
	if (!ensure(RenderComponent))
	{
		return;
	}

	RenderComponent->SetFlipbook(CharacterSprite_Idle);
	RenderComponent->SetLooping(true);
	RenderComponent->Play();
}

void ABattleCharacterActor::OnBattleSpeedChange(int32 NewBattleSpeed)
{
	SetBattleSpeed(NewBattleSpeed);
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

	if (RenderComponent)
	{
		RenderComponent->OnFinishedPlaying.AddUniqueDynamic(this, &ABattleCharacterActor::OnFlipbookPlayingEnd);
	}
}

void ABattleCharacterActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TickUpdateAttackOverlapActors();

	if (!DebugString.IsEmpty())
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 300), DebugString, this, FColor::White, 0.01f);

		DebugString.Empty();
	}

}

void ABattleCharacterActor::TickUpdateAttackOverlapActors()
{
	if (IsDead())
	{
		return;
	}

	ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
	if (!BattleManager)
	{
		return;
	}

	AttackOverlapActors.Empty();

	const TArray<ABattleCharacterActor*> Enemies = BattleManager->GetBattleCharacters(!bIsEnemy);
	for (ABattleCharacterActor* Enemy : Enemies)
	{
		if (!Enemy || Enemy->IsDead())
		{
			continue;
		}

		if (FMath::Square(AttackRange) > FVector::DistSquared(GetActorLocation(), Enemy->GetActorLocation()))
		{
			AttackOverlapActors.AddUnique(Enemy);
		}
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

void ABattleCharacterActor::SetBattleSpeed(int32 NewBattleSpeed)
{
	CurrentBattleSpeed = NewBattleSpeed;

	if (RenderComponent)
	{
		RenderComponent->SetPlayRate(CurrentBattleSpeed);
	}
}
