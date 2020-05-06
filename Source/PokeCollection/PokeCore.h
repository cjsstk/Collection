// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "UObject/NoExportTypes.h"

class APokeCollectionCharacter;
class ABattleManager;
class AHttpActor;

enum class EType;

namespace PokeCore
{
	class APokeCollectionCharacter* GetPokePlayer(const UWorld* WorldContext);
    class ABattleManager* GetBattleManager(const UWorld* WorldContext);
	class AHttpActor* GetHttpActor(const UWorld* WorldContext);

	const static FString DeviceId = FGenericPlatformMisc::GetDeviceId();

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
//	ID_Ascending,	// ȹ�� ��
//	ID_Descending,
//};