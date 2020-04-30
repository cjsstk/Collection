// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeSkill.h"

#include "Engine/World.h"

#include "BattleCharacterActor.h"
#include "BattleCharacterCombatComponent.h"
#include "CMS.h"
#include "SkillProjectileActor.h"

void UPokeSkill::InitSkill(int32 InSkillKey)
{
	const FSkillInfo* SkillInfo = CMS::GetSkillDataTable(InSkillKey);
	if (!ensure(SkillInfo))
	{
		return;
	}

	SkillDamage = SkillInfo->SkillDamage;
	AttackCount = SkillInfo->AttackCount;
	SpawnProjectileActor = SkillInfo->SpawnProjectileActor;
}

void UPokeSkill::SetSourceCharacter(ABattleCharacterActor* InSourceCharacter) 
{
	if (!InSourceCharacter)
	{
		return;
	}

	SourceCharacter = InSourceCharacter;

	UBattleCharacterCombatComponent* CombatComp = SourceCharacter->GetCombatComponent();
	if (CombatComp)
	{
		CombatComp->OnCharacterAttack.AddUniqueDynamic(this, &UPokeSkill::OnCharacterAttack);
	}

}

void UPokeSkill::UseSkill(const FPokeUseSkillParams& Params)
{
	OnUseSkill(Params);
}

bool UPokeSkill::CanUseSkill()
{
	return (CurrentAttackCount >= AttackCount);
}

void UPokeSkill::OnCharacterAttack()
{
	CurrentAttackCount += 1;
}

void UPokeSkill::OnUseSkill(const FPokeUseSkillParams& Params)
{
	if (!ensure(SourceCharacter))
	{
		return;
	}

	if (!Params.TargetCharacter)
	{
		ensure(0);
		return;
	}

	int32 TotalSkillDamage = SkillDamage * Params.CharacterStat;

	if (SpawnProjectileActor.Get())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(SourceCharacter);

		UWorld* World = SourceCharacter->GetWorld();
		if (!ensure(World))
		{
			return;
		}

		FVector SpawnLocation = SourceCharacter->GetActorLocation() + FVector(-30, 0, 50);
		ASkillProjectileActor* Projectile = World->SpawnActor<ASkillProjectileActor>(SpawnProjectileActor.Get(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (ensure(Projectile))
		{
			Projectile->SetProjectileDamage(TotalSkillDamage);
			Projectile->SetTargetCharacter(*Params.TargetCharacter);
		}
	}
	else
	{
		Params.TargetCharacter->TakeBattleDamage(TotalSkillDamage);
	}

	CurrentAttackCount = 0;
}
