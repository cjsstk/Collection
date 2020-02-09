// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMainWidget.h"

#include "Button.h"

#include "PokeCollectionGameMode.h"

void UInGameMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TempStartButton)
	{
		TempStartButton->OnClicked.AddUniqueDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	}
}

void UInGameMainWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (TempStartButton)
	{
		TempStartButton->OnClicked.RemoveDynamic(this, &UInGameMainWidget::OnStartButtonClicked);
	}
}

void UInGameMainWidget::OnStartButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APokeCollectionGameMode* GameMode = Cast<APokeCollectionGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->BattleStart();
		}
	}

	SetVisibility(ESlateVisibility::Collapsed);
}
