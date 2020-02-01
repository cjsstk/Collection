// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCollectionHUD.h"

#include "Blueprint/UserWidget.h"

void APokeCollectionHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InGameMainWidgetClass.Get())
	{
		InGameMainWidget = CreateWidget<UInGameMainWidget>(GetWorld(), InGameMainWidgetClass, FName("InGameMainWidget"));
		if (ensure(InGameMainWidget))
		{
			InGameMainWidget->AddToViewport();
		}
	}
}