﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCore.h"

#include "BattleManager.h"
#include "CMS.h"
#include "Net/HttpActor.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionGameMode.h"
#include "PokeSortInfo.h"
#include "Level/InGameLevelScriptActor.h"
#include "SortObjectInterface.h"
#include "TypeChart.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

class APokeCollectionCharacter* PokeCore::GetPokePlayer(const UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	return Cast<APokeCollectionCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContext, 0));
}

ABattleManager* PokeCore::GetBattleManager(const UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	AInGameLevelScriptActor* InGameLevel = Cast<AInGameLevelScriptActor>(WorldContext->GetLevelScriptActor());
	if (!InGameLevel)
	{
		return nullptr;
	}

	/*APokeCollectionGameMode* PokeGameMode = Cast<APokeCollectionGameMode>(WorldContext->GetAuthGameMode());
	if (!ensure(PokeGameMode))
	{
		return nullptr;
	}*/

	return InGameLevel->GetBattleManager();
}

AHttpActor* PokeCore::GetHttpActor(const UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	APokeCollectionGameMode* PokeGameMode = Cast<APokeCollectionGameMode>(WorldContext->GetAuthGameMode());
	if (!ensure(PokeGameMode))
	{
		return nullptr;
	}

	return PokeGameMode->GetHttpActor();
}

UMaterialInstanceDynamic* PokeCore::GetItemIcon(int32 InItemIconIndex, UObject* Outer)
{
	UMaterialInterface* ItemIconMaterial = CMS::GetItemIconMaterial();
	UMaterialInstanceDynamic* ItemIconMaterialInstance = nullptr;

	if (ItemIconMaterial)
	{
		ItemIconMaterialInstance = UMaterialInstanceDynamic::Create(ItemIconMaterial, Outer);
	}

	if (!ItemIconMaterialInstance)
	{
		ensure(0);
		return nullptr;
	}

	if (InItemIconIndex < 0)
	{
		return nullptr;
	}
	else
	{

		int32 ItemColumnIndex = InItemIconIndex % ItemIconColumnNum;
		int32 ItemRawIndex = InItemIconIndex / ItemIconColumnNum;

		ItemIconMaterialInstance->SetScalarParameterValue("ColumnIndexParam", ItemColumnIndex);
		ItemIconMaterialInstance->SetScalarParameterValue("RowIndexParam", ItemRawIndex);

		return ItemIconMaterialInstance;
	}

	return nullptr;
}

TArray<UMaterialInstanceDynamic*> PokeCore::GetTypeIcon(CharacterType InType, UObject* Outer)
{
	TArray<UMaterialInstanceDynamic*> TypeIcons;
	TypeIcons.Init(nullptr, 2);

	UMaterialInterface* TypeMaterial = CMS::GetTypeMaterial();

	UMaterialInstanceDynamic* Type1MaterialInstance = nullptr;
	UMaterialInstanceDynamic* Type2MaterialInstance = nullptr;

	if (!ensure(TypeMaterial))
	{
		return TypeIcons;
	}

	const FTypeInfo* Type1Info = CMS::GetTypeDataTable(InType.Type1);
	const FTypeInfo* Type2Info = CMS::GetTypeDataTable(InType.Type2);

	int32 Type1TextureNum = Type1Info ? Type1Info->TextureNum : -1;
	int32 Type2TextureNum = Type2Info ? Type2Info->TextureNum : -1;

	if (Type1TextureNum >= 0)
	{
		Type1MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, Outer);

		int32 Type1ColumnIndex = Type1TextureNum % CharacterTypeColumnNum;
		int32 Type1RawIndex = Type1TextureNum / CharacterTypeColumnNum;

		Type1MaterialInstance->SetScalarParameterValue("ColumnIndexParam", Type1ColumnIndex);
		Type1MaterialInstance->SetScalarParameterValue("RowIndexParam", Type1RawIndex);

		TypeIcons[0] = Type1MaterialInstance;
	}

	if (Type2TextureNum >= 0)
	{
		Type2MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, Outer);

		int32 Type2ColumnIndex = Type2TextureNum % CharacterTypeColumnNum;
		int32 Type2RawIndex = Type2TextureNum / CharacterTypeColumnNum;

		Type2MaterialInstance->SetScalarParameterValue("ColumnIndexParam", Type2ColumnIndex);
		Type2MaterialInstance->SetScalarParameterValue("RowIndexParam", Type2RawIndex);

		TypeIcons[1] = Type2MaterialInstance;
	}

	return TypeIcons;
}

void PokeCore::AddBattleLog(const UWorld* WorldContext, FString& NewBattleLog)
{
	ABattleManager* BM = GetBattleManager(WorldContext);
	if (!BM)
	{
		return;
	}

	BM->AddBattleLog(NewBattleLog);
}

FString PokeCore::GetStatusString(EStatus InStat)
{
	switch (InStat)
	{
	case EStatus::HealthPoint:
		return FString(TEXT("체력"));
		break;
	case EStatus::Attack:
		return FString(TEXT("공격"));
		break;
	case EStatus::Defense:
		return FString(TEXT("방어"));
		break;
	case EStatus::SpAttack:
		return FString(TEXT("특공"));
		break;
	case EStatus::SpDefense:
		return FString(TEXT("특방"));
		break;
	case EStatus::Speed:
		return FString(TEXT("스피드"));
		break;
	case EStatus::Count:
		break;
	default:
		break;
	}

	return FString();
}

void PokeCore::QuickSort(int32 Start, int32 End, TArray<class ISortObjectInterface*>& InObjects, FPokeSortInfo& CurrentSortInfo)
{
	if (Start >= End)
	{
		return;
	}

	ISortObjectInterface* Pivot = InObjects[(Start + End) / 2];
	int Left = Start;
	int Right = End;

	bool bAscending = CurrentSortInfo.SortAscending == ESortAscending::Ascending;

	while (Left <= Right)
	{
		if (bAscending)
		{
			while (InObjects[Left]->GetObjectSortValue(CurrentSortInfo.SortCategory) < Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Left++;
			}

			while (InObjects[Right]->GetObjectSortValue(CurrentSortInfo.SortCategory) > Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Right--;
			}
		}
		else
		{
			while (InObjects[Left]->GetObjectSortValue(CurrentSortInfo.SortCategory) > Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Left++;
			}

			while (InObjects[Right]->GetObjectSortValue(CurrentSortInfo.SortCategory) < Pivot->GetObjectSortValue(CurrentSortInfo.SortCategory))
			{
				Right--;
			}
		}

		if (Left <= Right)
		{
			InObjects.Swap(Left, Right);
			Left++;
			Right--;
		}
	}

	QuickSort(Start, Right, InObjects, CurrentSortInfo);
	QuickSort(Left, End, InObjects, CurrentSortInfo);
}

//float PokeCore::GetTypeEffective(EType InAttackType, EType InDefenseType)
//{
//	int32 EffectiveIndex = ((int32)InAttackType * (int32)EType::Count) + (int32)InDefenseType;
//	if (!TypeCharts.IsValidIndex(EffectiveIndex))
//	{
//		ensure(0);
//		return 0;
//	}
//
//	return TypeCharts[EffectiveIndex];
//}
