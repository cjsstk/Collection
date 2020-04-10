// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "PokeSortInfo.generated.h"

UENUM(BlueprintType)
enum class ESortCategory : uint8
{
	Level,
	Rank,
	Index,
	ID	// È¹µæ ¼ø
};

UENUM(BlueprintType)
enum class ESortAscending : uint8
{
	Ascending,
	Descending,
};


USTRUCT(BlueprintType)
struct FPokeSortInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESortCategory SortCategory = ESortCategory::ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESortAscending SortAscending = ESortAscending::Ascending;

};