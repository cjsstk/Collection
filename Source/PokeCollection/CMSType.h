// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

using characterKey = int32;
const static characterKey INVALID_CHARACTERKEY = 0;

using equipmentKey = int32;
const static equipmentKey INVALID_EQUIPMENTKEY = 0;

using battleStageKey = int32;
const static battleStageKey INVALID_BATTLESTAGEKEY = 0;

UENUM()
enum class ERank : uint8
{
	Normal,
	Rare,
	SRare,
	SSRare,
};

enum class EShopSlotType
{
	Character,
	Item,
};

struct FBattleReward
{
public:
	int32 ExperienceAmount;

	TArray<int32> GetCharacters;
};

static FLinearColor SSRareColor = FLinearColor::FromSRGBColor(FColor(255, 255, 102));
static FLinearColor SRareColor = FLinearColor::FromSRGBColor(FColor(204, 153, 255));
static FLinearColor RareColor = FLinearColor::FromSRGBColor(FColor(153, 255, 255));
static FLinearColor NormalColor = FLinearColor::FromSRGBColor(FColor(204, 204, 204));