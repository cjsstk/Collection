// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PokeCharacter.h"
#include "PaperFlipbook.h"
#include "CMS.generated.h"

using characterKey = int32;
const static characterKey INVALID_ITEMKEY = 0;

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APokeCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* CharacterSprite;
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
class POKECOLLECTION_API UCMSData : public UDataTable
{
	GENERATED_BODY()
	
public:


};
