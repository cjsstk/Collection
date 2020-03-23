// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ABattleManager;

enum class EType;

namespace PokeCore
{
    class ABattleManager* GetBattleManager(const UWorld* WorldContext);

	static TArray<float> TypeCharts = {
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
	};

	float GetTypeEffective(EType InAttackType, EType InDefenseType);
};