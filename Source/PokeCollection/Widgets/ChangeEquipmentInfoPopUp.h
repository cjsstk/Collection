// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChangeEquipmentInfoPopUp.generated.h"

UCLASS()
class POKECOLLECTION_API UChangeEquipmentStatusSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStatusText(FString InStatusType, int32 InChangeStatus);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChangeStatusText = nullptr;

};

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UChangeEquipmentInfoPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void InitInfo(int32 InNextEquipmentID);

	UFUNCTION()
	void OnCancelButtonClicked();

	UFUNCTION()
	void OnChangeButtonClicked();

private:
	void InitCurrentEquipmentInfo(class UPokeEquipment* InCurrentEquipment);
	void InitNextEquipmentInfo(class UPokeEquipment* InNextEquipment);

	UPROPERTY(meta = (BindWidget))
	class UImage* CurrentEquipmentImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* NextEquipmentImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CurrentEquipmentInfo = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* NextEquipmentInfo = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* ChangeButton = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UChangeEquipmentStatusSlot> ChangeEquipmentStatusSlotClass;

	int32 CurrentEquipmentID = -1;
	int32 NextEquipmentID = -1;
};
