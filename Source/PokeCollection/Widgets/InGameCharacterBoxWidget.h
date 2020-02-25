// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameCharacterBoxWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UCharacterBoxSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProfileImage(UTexture2D* InProfileTexture);


private:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* ProfileImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectCharacterButton = nullptr;
};


/**
 * Character Box Widget
 */
UCLASS()
class POKECOLLECTION_API UInGameCharacterBoxWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CharacterScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* CharacterGridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterBoxSlot> CharacterBoxSlotClass;

	int32 ColumnNum = 7;

};
