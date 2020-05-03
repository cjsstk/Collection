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

void APokeCharacter::SetLevel(int32 NewLevel)
{ 
	Level = NewLevel;

	const FCharacterExperienceTable* CharacterExpData = CMS::GetCharacterExperienceTable(Level);

	MaxExp = CharacterExpData ? CharacterExpData->NeedExperienceForNextLevel : -1;
}

void APokeCharacter::TakeExperience(int32 InExp)
{
	CurrentExp += InExp;

	while (CurrentExp >= MaxExp)
	{
		CurrentExp -= MaxExp;

		SetLevel(Level + 1);
	}
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

	return CharacterInfo->CharacterSprite_Idle;
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

	FinalStatus = CalcFinalStatus(BaseStats, EvStats);

	return FinalStatus;
}

int32 APokeCharacter::GetObjectSortValue(ESortCategory InSortCategory) const
{
	int32 SortValue = 0;

	switch (InSortCategory)
	{
	case ESortCategory::Level:
		SortValue = Level;
		break;
	case ESortCategory::Rank:
		SortValue = (int32)GetCharacterRank();
		break;
	case ESortCategory::Index:
		SortValue = CharacterKey;
		break;
	case ESortCategory::ID:
		SortValue = CharacterID;
		break;
	default:
		break;
	}

	return SortValue;
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

		MyType.Type1 = CharacterInfo->Type1;
		MyType.Type2 = CharacterInfo->Type2;
	}
}

FStatus APokeCharacter::CalcFinalStatus(FStatus InBaseStat, FStatus InEvStat)
{
	FStatus FinalStat;

	FinalStat.HealthPoint = InBaseStat.HealthPoint * Level + InEvStat.HealthPoint + 1;
	FinalStat.Attack = InBaseStat.Attack * Level + InEvStat.Attack;
	FinalStat.Defense = InBaseStat.Defense * Level + InEvStat.Defense;
	FinalStat.SpecialAttack = InBaseStat.SpecialAttack * Level + InEvStat.SpecialAttack;
	FinalStat.SpecialDefense = InBaseStat.SpecialDefense * Level + InEvStat.SpecialDefense;
	FinalStat.Speed = InBaseStat.Speed + InEvStat.Speed;

	return FinalStat;
}
