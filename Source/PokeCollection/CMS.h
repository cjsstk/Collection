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

USTRUCT(BlueprintType)
struct FStageInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> StageBackground = nullptr;
};

USTRUCT(BlueprintType)
struct FWidgetInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	
};

namespace CMS
{
	void LoadCMS();

	const FCharacterInfo* GetCharacterDataTable(characterKey CharacterKey);
	const FStageInfo* GetStageDataTable(int32 StageKey);

	static UDataTable* CharacterDataTable;
	static UDataTable* StageDataTable;
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
