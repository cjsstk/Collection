// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PokeCharacter.h"
#include "CMS.generated.h"

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APokeCharacter> CharacterClass;

};

namespace CMS
{
	void LoadCMS();
	FCharacterInfo GetCharacterDataTable(int32 CharacterKey);

	static UDataTable* CharacterDataTable;
};

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UCMS : public UDataTable
{
	GENERATED_BODY()
	
};
