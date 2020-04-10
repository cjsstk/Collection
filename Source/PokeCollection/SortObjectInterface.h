// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CMSType.h"
#include "PokeSortInfo.h"
#include "SortObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USortObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Sort Obeject Interface
 */
class POKECOLLECTION_API ISortObjectInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetObjectSortValue(ESortCategory InSortCategory) const = 0;
};
