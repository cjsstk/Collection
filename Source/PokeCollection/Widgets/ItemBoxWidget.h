// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameBoxWidget.h"
#include "ItemBoxWidget.generated.h"

/**
 * ������ �ڽ� ����
 */
UCLASS()
class POKECOLLECTION_API UItemSlot : public UBoxSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void InitByID(int32 InContentID) override;

	virtual void OnSelectButtonClicked() override;

	void SetItemStack(int32 InStack);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StackText = nullptr;

};

/**
 * ������ �ڽ� ����
 */
UCLASS()
class POKECOLLECTION_API UItemBoxWidget : public UBoxContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
	virtual void RefreshSlot() override;

};
