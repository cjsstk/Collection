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

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ShopScrollBox = nullptr;

};


UCLASS()
class POKECOLLECTION_API UItemShopWidget : public UShopContentWidget
{
	GENERATED_BODY()

public:
	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ShopGridPanel = nullptr;

};
