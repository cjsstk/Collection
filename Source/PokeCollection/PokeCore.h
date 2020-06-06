// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "UObject/NoExportTypes.h"

class APokeCollectionCharacter;
class ABattleManager;
class AHttpActor;
class ISortObjectInterface;
class UMaterialInstanceDynamic;

enum class EType;
enum class EStatus;

struct FPokeSortInfo;

namespace PokeCore
{
	class APokeCollectionCharacter* GetPokePlayer(const UWorld* WorldContext);
    class ABattleManager* GetBattleManager(const UWorld* WorldContext);
	class AHttpActor* GetHttpActor(const UWorld* WorldContext);

	class UMaterialInstanceDynamic* GetItemIcon(int32 InItemIconIndex, UObject* Outer);

	void AddBattleLog(const UWorld* WorldContext, FString& NewBattleLog);

	const static FString DeviceId = FGenericPlatformMisc::GetDeviceId();

	FString GetStatusString(EStatus InStat);

	void QuickSort(int32 Start, int32 End, TArray<class ISortObjectInterface*>& InObjects, FPokeSortInfo& CurrentSortInfo);

	/*static TArray<float> TypeCharts = {
		1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1, 0.5f,    0,    1,    1, 0.5f,    1,
		1, 0.5f, 0.5f,    1,    2,    2,    1,    1,    1,    1,    1,    2, 0.5f,    1, 0.5f,    1,    2,    1,
		1,    2, 0.5f,    1, 0.5f,    1,    1,    1,    2,    1,    1,    1,    2,    1, 0.5f,    1,    1,    1,
		1,    1,    2, 0.5f, 0.5f,    1,    1,    1,    0,    2,    1,    1,    1,    1, 0.5f,    1,    1,    1,
		1, 0.5f,    2,    1, 0.5f,    1,    1, 0.5f,    2, 0.5f,    1, 0.5f,    2,    1, 0.5f,    1, 0.5f,    1,
		1, 0.5f, 0.5f,    1,    2, 0.5f,    1,    1,    2,    2,    1,    1,    1,    1,    2,    1, 0.5f,    1,
		2,    1,    1,    1,    1,    2,    1, 0.5f,    1, 0.5f, 0.5f, 0.5f,    2,    0,    1,    2,    2, 0.5f,
		1,    1,    1,    1,    2,    1,    1, 0.5f, 0.5f,    1,    1,    1, 0.5f, 0.5f,    1,    1,    0,    2,
		1,    2,    1,    2, 0.5f,    1,    1,    2,    1,    0,    1, 0.5f,    2,    1,    1,    1,    2,    1,
		1,    1,    1, 0.5f,    2,    1,    2,    1,    1,    1,    1,    2, 0.5f,    1,    1,    1, 0.5f,    1,
		1,    1,    1,    1,    1,    1,    2,    2,    1,    1, 0.5f,    1,    1,    1,    1,    0, 0.5f,    1,
		1, 0.5f,    1,    1,    2,    1, 0.5f, 0.5f,    1, 0.5f,    2,    1,    1, 0.5f,    1,    2, 0.5f, 0.5f,
		1,    2,    1,    1,    1,    2, 0.5f,    1, 0.5f,    2,    1,    2,    1,    1,    1,    1, 0.5f,    1,
		0,    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,    1,    1,    2,    1, 0.5f,    1,    1,
		1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,    1, 0.5f,    0,
		1,    1,    1,    1,    1,    1, 0.5f,    1,    1,    1,    2,    1,    1,    2,    1, 0.5f,    1, 0.5f,
		1, 0.5f, 0.5f, 0.5f,    1,    2,    1,    1,    1,    1,    1,    1,    2,    1,    1,    1, 0.5f,    2,
		1, 0.5f,    1,    1,    1,    1,    2, 0.5f,    1,    1,    1,    1,    1,    1,    2,    2, 0.5f,    1
	};*/

	//float GetTypeEffective(EType InAttackType, EType InDefenseType);
};

//UENUM()
//enum class ESortKinds
//{
//	Level_Ascending,
//	Level_Descending,
//	Rank_Ascending,
//	Rank_Descending,
//	Index_Ascending,
//	Index_Descending,
//	ID_Ascending,	// È¹µæ ¼ø
//	ID_Descending,
//};