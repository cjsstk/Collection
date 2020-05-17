// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacterCombatComponent.h"

#include "BattleCharacterActor.h"
#include "PokeCore.h"
#include "PokeCollectionCharacter.h"
#include "PokeSkill.h"

static TAutoConsoleVariable<int32> CVarCanEnemyAttack
(
	TEXT("poke.canEnemyAttack"),
	1,
	TEXT("1: Enemy can attack, 0: Enemy can't attack")
);

UBattleCharacterCombatComponent::UBattleCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBattleCharacterCombatComponent::AttackTarget()
{
	if (!TargetCharacter)
	{
		return;
	}

	ABattleCharacterActor* Character = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(Character))
	{
		return;
	}

	FString BattleLog = Character->IsEnemy() ? TEXT("적 ") : FString();
	BattleLog += Character->GetCharacterName().ToString() + TEXT("의 ");

	const FStatus& Status = Character->GetFinalStatus();
	bool bUseSkill = false;

	const TArray<UPokeSkill*> Skills = Character->GetSkills();
	for (int32 Index = 0; Index < 3; ++Index)
	{
		if (Skills.IsValidIndex(Index))
		{
			UPokeSkill* Skill = Skills[Index];
			if (Skill && Skill->CanUseSkill())
			{
				Character->ChangeSprite(ESpriteType::Skill, Index);

				bUseSkill = true;

				FPokeUseSkillParams Params;
				Params.TargetCharacter = TargetCharacter;
				Params.CharacterStat = Status.SpecialAttack;
				Skill->UseSkill(Params);

				BattleLog += Skill->GetSkillName().ToString() + TEXT(" 스킬!");

				break;
			}
		}
	}

	// Normal Attack
	if (!bUseSkill)
	{
		Character->ChangeSprite(ESpriteType::Attack);

		int32 AttackDamage = Status.Attack;

		if (AttackDamage <= 0)
		{
			AttackDamage = 1;
		}

		TargetCharacter->TakeBattleDamage(AttackDamage / 3);

		OnCharacterAttack.Broadcast();

		BattleLog += TEXT("공격!");
	}

	PokeCore::AddBattleLog(GetWorld(), BattleLog);

}

void UBattleCharacterCombatComponent::UseActiveSkill(FPokeUseSkillParams& InParams)
{
	ABattleCharacterActor* Character = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(Character))
	{
		return;
	}

	const TArray<UPokeSkill*> Skills = Character->GetSkills();

	if (!Skills.IsValidIndex(3))
	{
		return;
	}

	UPokeSkill* ActiveSkill = Skills[3];
	ActiveSkill->UseSkill(InParams);

	AttackDelayAgeSeconds = 0.0f;
	Character->ChangeSprite(ESpriteType::Skill, 3);
}

void UBattleCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBattleCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ABattleCharacterActor* BattleCharacter = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(BattleCharacter))
	{
		return;
	}

	if (BattleCharacter->IsDead())
	{
		return;
	}

	TickFindNewTarget();
	TickAttackTarget(DeltaTime);
}

void UBattleCharacterCombatComponent::TickFindNewTarget()
{
	if (TargetCharacter)
	{
		if (TargetCharacter->IsDead())
		{
			TargetCharacter = nullptr;
		}
		return;
	}

	ABattleCharacterActor* BattleCharacter = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(BattleCharacter))
	{
		return;
	}

	if (CVarCanEnemyAttack.GetValueOnGameThread() < 1 && BattleCharacter->IsEnemy())
	{
		return;
	}

	const TArray<AActor*> AttackOverlapActors = BattleCharacter->GetAttackOverlapActors();
	for (AActor* AttackOverlapActor : AttackOverlapActors)
	{
		ABattleCharacterActor* OverlapCharacter = Cast<ABattleCharacterActor>(AttackOverlapActor);
		if (OverlapCharacter)
		{
			// TODO : Need Target Rule
			TargetCharacter = OverlapCharacter;
			break;
		}
	}

}

void UBattleCharacterCombatComponent::TickAttackTarget(float DeltaTime)
{
	ABattleCharacterActor* BattleCharacter = Cast<ABattleCharacterActor>(GetOwner());
	if (!ensure(BattleCharacter))
	{
		return;
	}

	if (BattleCharacter->IsAttacking())
	{
		return;
	}

	AttackDelayAgeSeconds += DeltaTime * BattleCharacter->GetCurrentBattleSpeedMultiplier();

	if (AttackDelayAgeSeconds < AttackDelaySeconds)
	{
		return;
	}

	if (!TargetCharacter)
	{
		return;
	}

	if (TargetCharacter->IsDead())
	{
		TargetCharacter = nullptr;
		return;
	}

	AttackTarget();
	AttackDelayAgeSeconds = 0.0f;
	
}

