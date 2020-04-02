// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameShopWidget.h"
#include "ShopContentWidgets.generated.h"



UCLASS()
class POKECOLLECTION_API UCharacterShopWidget : public UShopContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnOpen() override;

};


UCLASS()
class POKECOLLECTION_API UEquipmentShopWidget : public UShopContentWidget
{
	GENERATED_BODY()

public:
	virtual void OnOpen() override;

};
