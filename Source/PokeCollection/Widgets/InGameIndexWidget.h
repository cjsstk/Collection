// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameIndexWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UCharacterIndexSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Init(int32 InCharacterKey);
	void RefreshSlot();

	UFUNCTION()
	void OnSelectCharacterButtonClicked();


private:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* ProfileImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectSlotButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText = nullptr;

	int32 CharacterKey = 0;
	bool bEnabled = false;
};


/**
 * Character Index Widget
 */
UCLASS()
class POKECOLLECTION_API UInGameIndexWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

private:
	void Refresh();

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CharacterScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* CharacterGridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterIndexSlot> CharacterIndexSlotClass;

	int32 ColumnNum = 7;

};
