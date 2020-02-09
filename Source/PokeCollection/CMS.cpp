// Fill out your copyright notice in the Description page of Project Settings.


#include "CMS.h"
#include "UObject/ConstructorHelpers.h"

void CMS::LoadCMS()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/CMS/CharacterInfo"));
    if (ensure(DataTable.Succeeded()))
    {
        CharacterDataTable = DataTable.Object;
    }

}

FCharacterInfo CMS::GetCharacterDataTable(int32 CharacterKey)
{
    FCharacterInfo* CharacterInfo = CharacterDataTable->FindRow<FCharacterInfo>(FName(*(FString::FormatAsNumber(CharacterKey))), FString(""));

    return *CharacterInfo;
}
