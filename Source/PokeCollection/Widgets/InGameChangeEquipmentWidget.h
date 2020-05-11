// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "Widgets/EquipmentBoxWidget.h"
#include "InGameChangeEquipmentWidget.generated.h"


/**
 * 장비 변경 위젯에 나오는 장비 슬롯
 */
UCLASS()
class POKECOLLECTION_API UChangeEquipmentSlot : public UEquipmentSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void InitByID(int32 InContentID) override;
	virtual void OnSelectButtonClicked() override;

protected:
	int32 SlotEquipmentID = -1;

};

/** 장비를 해제 시키는 슬롯 */
UCLASS()
class POKECOLLECTION_API UExcludeEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnExcludeButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ExcludeButton = nullptr;
};

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UInGameChangeEquipmentWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* EquipmentScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* EquipmentGridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChangeEquipmentSlot> EquipmentBoxSlotClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UExcludeEquipmentSlot> ExcludeEquipmentSlotClass;

	//UPROPERTY(Transient)
	//class APokeCharacter* OwnerCharacter = nullptr;

	int32 ColumnNum = 10;

	int32 NeedExcludeSlot = 0;
};
