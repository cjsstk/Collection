// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/InGameBoxWidget.h"
#include "EquipmentBoxWidget.generated.h"



UCLASS()
class POKECOLLECTION_API UEquipmentBoxWidget : public UBoxContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

private:

};