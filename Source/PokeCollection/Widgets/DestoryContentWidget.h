// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameBoxWidget.h"
#include "Widgets/CharacterBoxWidget.h"
#include "DestoryContentWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySlotClicked, int32, CharacterID);

/** ���� �� �� ��Ÿ���� ĳ���� ���� */
UCLASS()
class POKECOLLECTION_API UDestoryCharacterSlot : public UCharacterSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() override;

	FOnDestroySlotClicked OnDestroySlotClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SelectedImage = nullptr;

	bool bSelected = false;

};

/**
 * ĳ���� ���� ����
 */
UCLASS()
class POKECOLLECTION_API UDestoryContentWidget : public UBoxContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
	virtual void RefreshSlot() override;

	UFUNCTION()
	void OnDestroyButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* DestroyButton = nullptr;

};
