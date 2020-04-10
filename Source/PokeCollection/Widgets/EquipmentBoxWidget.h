// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/InGameBoxWidget.h"
#include "EquipmentBoxWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UEquipmentSlot : public UBoxSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void OnSelectButtonClicked() override;

protected:


};


UCLASS()
class POKECOLLECTION_API UEquipmentBoxWidget : public UBoxContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
	virtual void SortContent(FPokeSortInfo InSortInfo) override;
	virtual void RefreshSlot() override;

private:

};
