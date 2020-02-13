// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCollectionHUD.h"

#include "Widgets/InGameAdventureWidget.h"

#include "Blueprint/UserWidget.h"
#include "WidgetLayoutLibrary.h"

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

	if (InGameAdventureWidgetClass.Get())
	{
		InGameAdventureWidget = CreateWidget<UInGameAdventureWidget>(GetWorld(), InGameAdventureWidgetClass, FName("InGameAdventureWidget"));
		InGameAdventureWidget->SetPrevWidget(InGameMainWidget);
	}
}

void APokeCollectionHUD::OpenInGameAdventureWidget()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	if (ensure(InGameAdventureWidget))
	{
		InGameAdventureWidget->AddToViewport();
		InGameAdventureWidget->OnOpen();
	}
}