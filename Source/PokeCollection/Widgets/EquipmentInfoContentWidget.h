// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "EquipmentInfoContentWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UStatusChangeByEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetChangeStatusText(int32 InExcludeStat, int32 InIncludeStat);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExcludeEquipmentStatText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* IncludeEquipmentStatText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChangeAmountText = nullptr;

};


/**
 * 캐릭터 장비 화면 위젯
 */
UCLASS()
class POKECOLLECTION_API UEquipmentInfoContentWidget : public UCharacterInfoContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

	UFUNCTION()
	void OnSlotButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SlotImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* EquipmentImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* StatusChangeBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* EmptyEquipmentTexture = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStatusChangeByEquipmentWidget> StatusChangeWidgetClass = nullptr;
};
