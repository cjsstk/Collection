// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCharacter.h"

#include "InBattleCharacterPanel.h"
#include "CMS.h"

#include "DrawDebugHelpers.h"

void APokeCharacter::Init(characterKey InCharacterKey)
{
	if (InCharacterKey == INVALID_CHARACTERKEY)
	{
		return;
	}

	CharacterKey = InCharacterKey;

	InitBaseStatus();

	/*const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InCharacterKey);
	if (ensure(CharacterInfo))
	{
		CharacterName = CharacterInfo->CharacterName;
		CharacterProfileImage = CharacterInfo->CharacterProfile;
		CharacterImage = CharacterInfo->CharacterImage;
	}*/
}

void APokeCharacter::Attack(APokeCharacter* TargetCharacter)
{
	if (!TargetCharacter)
	{
		// Buff?
		ensure(0);
		return;
	}

	CurrentTargetCharacter = TargetCharacter;
	bAttacking = true;
}

void APokeCharacter::SetBattleCharacterActor(ABattleCharacterActor* InBattleCharacterActor)
{
	if (!InBattleCharacterActor)
	{
		return;
	}

	MyBattleCharacter = InBattleCharacterActor;
}

FName APokeCharacter::GetCharacterName() const
{ 
	if (CharacterKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return FName("InvalidKey");
	}

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
	if (!ensure(CharacterInfo))
	{
		return FName("InvalidCMS");
	}

	return CharacterInfo->CharacterName;
}

UTexture2D* APokeCharacter::GetCharacterProfileImage() const
{ 
	if (CharacterKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return nullptr;
	}

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
	if (!ensure(CharacterInfo))
	{
		return nullptr;
	}

	return CharacterInfo->CharacterProfile;
}

UTexture2D* APokeCharacter::GetCharacterImage() const
{
	if (CharacterKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return nullptr;
	}

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
	if (!ensure(CharacterInfo))
	{
		return nullptr;
	}

	return CharacterInfo->CharacterImage;
}

UPaperFlipbook* APokeCharacter::GetCharacterFlipbook() const
{
	if (CharacterKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return nullptr;
	}

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
	if (!ensure(CharacterInfo))
	{
		return nullptr;
	}

	return CharacterInfo->CharacterSprite;
}

ERank APokeCharacter::GetCharacterRank() const
{
	if (CharacterKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return ERank::Normal;
	}

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
	if (!ensure(CharacterInfo))
	{
		return ERank::Normal;
	}

	return CharacterInfo->CharacterRank;
}

float APokeCharacter::GetAttackRange() const
{
	if (CharacterKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return false;
	}

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
	if (!ensure(CharacterInfo))
	{
		return false;
	}

	return CharacterInfo->AttackRange;
}

const FStatus APokeCharacter::GetFinalStatus()
{
	FStatus FinalStatus;

	FinalStatus.HealthPoint = CalcFinalStatus(BaseStats.HealthPoint, EvStats.HealthPoint, true);
	FinalStatus.Attack = CalcFinalStatus(BaseStats.Attack, EvStats.Attack);
	FinalStatus.Defense = CalcFinalStatus(BaseStats.Defense, EvStats.Defense);
	FinalStatus.SpecialAttack = CalcFinalStatus(BaseStats.SpecialAttack, EvStats.SpecialAttack);
	FinalStatus.SpecialDefense = CalcFinalStatus(BaseStats.SpecialDefense, EvStats.SpecialDefense);
	FinalStatus.Speed = CalcFinalStatus(BaseStats.Speed, EvStats.Speed);

	return FinalStatus;
}

void APokeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bAttacking)
	{
		const FVector TargetLocation = CurrentTargetCharacter->GetActorLocation();
		const FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, 1.0f);
		SetActorLocation(NewLocation);
	}
	else
	{
		if (ensure(CurrentBattlePanel))
		{
			const FVector NewLocation = FMath::Lerp(GetActorLocation(), CurrentBattlePanel->GetActorLocation(), 1.0f);
			SetActorLocation(NewLocation);
		}
	}

}

void APokeCharacter::InitBaseStatus()
{
	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
	if (ensure(CharacterInfo))
	{
		BaseStats.HealthPoint = CharacterInfo->BaseStat_Attack;
		BaseStats.Attack = CharacterInfo->BaseStat_Attack;
		BaseStats.Defense = CharacterInfo->BaseStat_Defence;
		BaseStats.SpecialAttack = CharacterInfo->BaseStat_SPAtk;
		BaseStats.SpecialDefense = CharacterInfo->BaseStat_SPDef;
		BaseStats.Speed = CharacterInfo->BaseStat_Speed;
	}
}

int32 APokeCharacter::CalcFinalStatus(int32 InBaseStat, int32 InEvStat, bool bIsHP)
{
	int FinalStat = 0;

	if (!bIsHP)
	{
		FinalStat = (InBaseStat * 2 * ((float)Level * 0.01f)) + ((float)InEvStat * 0.5f);
	}
	else
	{
		FinalStat = (InBaseStat * 2 * ((float)Level * 0.01f)) + ((float)InEvStat * 0.5f) + Level;
	}

	return FinalStat;
}
