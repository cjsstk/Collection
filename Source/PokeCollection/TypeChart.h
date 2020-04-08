// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM()
enum class EType : int32
{
	None,
	Normal,
	Fire,
	Water,
	Electric,
	Glass,
	Ice,
	Fight,
	Poison,
	Ground,
	Fly,
	Psychic,
	Bug,
	Rock,
	Ghost,
	Dragon,
	Dark,
	Steal,
	Fairy,
	Count,
};

struct CharacterType
{
public:
	EType Type1;

	EType Type2;
};

//namespace TypeChart
//{
//	static TArray<float> TypeCharts = {
//		1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1, 0.5f,    0,    1,    1, 0.5f,    1,
//		1, 0.5f, 0.5f,    1,    2,    2,    1,    1,    1,    1,    1,    2, 0.5f,    1, 0.5f,    1,    2,    1,
//		1,    2, 0.5f,    1, 0.5f,    1,    1,    1,    2,    1,    1,    1,    2,    1, 0.5f,    1,    1,    1,
//		1,    1,    2, 0.5f, 0.5f,    1,    1,    1,    0,    2,    1,    1,    1,    1, 0.5f,    1,    1,    1,
//		1, 0.5f,    2,    1, 0.5f,    1,    1, 0.5f,    2, 0.5f,    1, 0.5f,    2,    1, 0.5f,    1, 0.5f,    1,
//		1, 0.5f, 0.5f,    1,    2, 0.5f,    1,    1,    2,    2,    1,    1,    1,    1,    2,    1, 0.5f,    1,
//		2,    1,    1,    1,    1,    2,    1, 0.5f,    1, 0.5f, 0.5f, 0.5f,    2,    0,    1,    2,    2, 0.5f,
//		1,    1,    1,    1,    2,    1,    1, 0.5f, 0.5f,    1,    1,    1, 0.5f, 0.5f,    1,    1,    0,    2,
//		1,    2,    1,    2, 0.5f,    1,    1,    2,    1,    0,    1, 0.5f,    2,    1,    1,    1,    2,    1,
//		1,    1,    1, 0.5f,    2,    1,    2,    1,    1,    1,    1,    2, 0.5f,    1,    1,    1, 0.5f,    1,
//		1,    1,    1,    1,    1,    1,    2,    2,    1,    1, 0.5f,    1,    1,    1,    1,    0, 0.5f,    1,
//		1, 0.5f,    1,    1,    2,    1, 0.5f, 0.5f,    1, 0.5f,    2,    1,    1, 0.5f,    1,    2, 0.5f, 0.5f,
//		1,    2,    1,    1,    1,    2, 0.5f,    1, 0.5f,    2,    1,    2,    1,    1,    1,    1, 0.5f,    1,
//		0,    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,    1,    1,    2,    1, 0.5f,    1,    1,
//		1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,    1, 0.5f,    0,
//		1,    1,    1,    1,    1,    1, 0.5f,    1,    1,    1,    2,    1,    1,    2,    1, 0.5f,    1, 0.5f,
//		1, 0.5f, 0.5f, 0.5f,    1,    2,    1,    1,    1,    1,    1,    1,    2,    1,    1,    1, 0.5f,    2,
//		1, 0.5f,    1,    1,    1,    1,    2, 0.5f,    1,    1,    1,    1,    1,    1,    2,    2, 0.5f,    1
//	};
//
//	float GetTypeEffective(EType InAttackType, EType InDefenseType)
//	{
//		int32 EffectiveIndex = ((int32)InAttackType * (int32)EType::Count) + (int32)InDefenseType;
//		if (!TypeCharts.IsValidIndex(EffectiveIndex))
//		{
//			ensure(0);
//			return 0;
//		}
//
//		return TypeCharts[EffectiveIndex];
//	}
//};
