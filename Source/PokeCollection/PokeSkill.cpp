// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeSkill.h"

#include "Engine/World.h"

#include "BattleCharacterActor.h"
#include "BattleCharacterCombatComponent.h"
#include "CMS.h"
#include "SkillProjectileActor.h"

void UPokeSkill::InitSkill(InitSkillParams InSkillParams)
{
	const FSkillInfo* SkillInfo = CMS::GetSkillDataTable(InSkillParams.SkillKey);
	if (!ensure(SkillInfo))
	{
		return;
	}

	UCurveFloat* SkillDamageCurve = SkillInfo->SkillDamageUpgradeCurve.LoadSynchronous();
	if (SkillDamageCurve)
	{
		SkillDamage = SkillDamageCurve->GetFloatValue(InSkillParams.SkillLevel);
	}

	SkillName = SkillInfo->SkillName;
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
	if (!ensure(SourceCharacter))
	{
		return;
	}

	if (!Params.TargetCharacter)
	{
		ensure(0);
		return;
	}

	OnUseSkill(Params);

	CurrentAttackCount = 0;
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

}

void UPokeSkill_Ember::OnUseSkill(const FPokeUseSkillParams& Params)
{
	Super::OnUseSkill(Params);
}
