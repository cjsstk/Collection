// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameCharacterInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UInGameCharacterInfoWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnOpen() override;

	void SetSelectedCharacterID(int32 InSelectedCharacterID) { SelectedCharacterID = InSelectedCharacterID; }
	
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterName = nullptr;

	int32 SelectedCharacterID = 0;

};
