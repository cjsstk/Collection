// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentInfoPopUp.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UEquipmentInfoPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void InitInfo(int32 InEquipmentID);

	UFUNCTION()
	void OnBackgroundClicked();

	UFUNCTION()
	void OnExcludeButtonClicked();

	UFUNCTION()
	void OnChangeButtonClicked();

	UFUNCTION()
	void OnUpgradeButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* EquipmentImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EquipmentName = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EquipmentDesc = nullptr;

	//
	UPROPERTY(meta = (BindWidget))
	class UButton* ExcludeButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* ChangeButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* UpgradeButton = nullptr;

	int32 EquipmentID = -1;
};
