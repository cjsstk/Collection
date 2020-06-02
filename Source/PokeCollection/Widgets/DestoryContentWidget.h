// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameBoxWidget.h"
#include "Widgets/CharacterBoxWidget.h"
#include "DestoryContentWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySlotClicked, int32, CharacterID);

/** 분해 할 때 나타나는 캐릭터 슬롯 */
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
 * 캐릭터 분해 위젯
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
