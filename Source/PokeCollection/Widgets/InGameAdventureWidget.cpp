// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameAdventureWidget.h"

#include "Button.h"
#include "Components/WidgetSwitcher.h"
#include "Image.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"

void UInGameAdventureWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ensure(StageWidgets))
	{
		StageWidgets->ClearChildren();

		TArray<FStageInfo*> AllStageDatas;

		CMS::GetAllStageDataTable(AllStageDatas);

		for (FStageInfo* StageData : AllStageDatas)
		{
			if (StageData && StageData->StageWidgets.Get())
			{
				UStageWidget* StageWidget = CreateWidget<UStageWidget>(GetWorld(), StageData->StageWidgets.Get());
				if (!StageWidget)
				{
					continue;
				}

				StageWidgets->AddChild(StageWidget);
			}
		}

		APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
		if (ensure(Player))
		{
			StageWidgets->SetActiveWidgetIndex(Player->GetCurrentSelectedChapterNum() - 1);
		}
	}

	if (PrevStageButton)
	{
		PrevStageButton->OnClicked.AddUniqueDynamic(this, &UInGameAdventureWidget::OnPrevStageButtonClicked);
	}
	
	if (NextStageButton)
	{
		NextStageButton->OnClicked.AddUniqueDynamic(this, &UInGameAdventureWidget::OnNextStageButtonClicked);
	}
}

void UInGameAdventureWidget::OnOpen()
{
	Super::OnOpen();
}

void UInGameAdventureWidget::OnPrevStageButtonClicked()
{
	APokeCollectionCharacter* Player = GetPlayer();
	if (!Player)
	{
		return;
	}

	Player->SetCurrentSelectedChapterNum(Player->GetCurrentSelectedChapterNum() - 1);
	StageWidgets->SetActiveWidgetIndex(Player->GetCurrentSelectedChapterNum() - 1);
}

void UInGameAdventureWidget::OnNextStageButtonClicked()
{
	APokeCollectionCharacter* Player = GetPlayer();
	if (!Player)
	{
		return;
	}

	Player->SetCurrentSelectedChapterNum(Player->GetCurrentSelectedChapterNum() + 1);
	StageWidgets->SetActiveWidgetIndex(Player->GetCurrentSelectedChapterNum() - 1);
}
